/** 
  * @file bap_can.h
  * @version 2.0
  * @date Dec,2th 2020
  *
  * @brief  底层can通讯的配置，包括发送数据帧，CAN滤波组，接受中断回调函数
  *
  *	@author JFG
  *
  */
#include "bsp_can.h"
#include "string.h"
#include "bsp_T_imu.h"

MOTO_RecvMsg_t	moto_msg;

CAN_TxHeaderTypeDef Tx1Message;
CAN_RxHeaderTypeDef Rx1Message;
CAN_TxHeaderTypeDef Tx2Message;
CAN_RxHeaderTypeDef Rx2Message;

uint8_t CAN1_Rx_data[8];
uint8_t CAN1_Tx_data[8];
uint8_t CAN2_Rx_data[8];
uint8_t CAN2_Tx_data[8];

uint32_t message1;

/**
  * @brief     CAN接受中断回调函数
  * @param     CAN_Rx_data ：CAN节点反馈的数据帧
  * @attention 
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == &hcan1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &Rx1Message, CAN1_Rx_data);
		switch (Rx1Message.StdId)
		{
			case CAN_3508_STIR_ID:/*拨盘电机 8Pin端子*/
			{
				moto_msg.stir.msg_cnt++ <= 50 ? get_moto_offset(&moto_msg.stir) : encoder_data_handler(&moto_msg.stir, CAN1_Rx_data);
				break;
			}
			case CAN_FRIC_M3508_LEFT_ID:/* 2Pin端子 */
			case CAN_FRIC_M3508_RIGHT_ID:
      {
				static uint8_t j;
				j = Rx1Message.StdId - CAN_FRIC_M3508_LEFT_ID;
				encoder_data_handler(&moto_msg.fric[j], CAN1_Rx_data);
				break;
			}
      case CAN_LIMIT_2006_ID: 
      {
        encoder_data_handler(&moto_msg.limit, CAN1_Rx_data);
      }
			default:
			{
				break;
			}
		}
		__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	}

	else if(hcan == &hcan2)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &Rx2Message, CAN2_Rx_data);
		switch (Rx2Message.StdId)
		{
			case CAN_YAW_MOTOR_ID:/*大端子*/
			{
				encoder_data_handler(&moto_msg.yaw, CAN2_Rx_data);
				break;
			}
			case CAN_PIT_MOTOR_ID:/*小端子*/
			{    
				encoder_data_handler(&moto_msg.pit, CAN2_Rx_data);
				break;
			}
			case TIMU_PALSTANCE_ID:/* 陀螺仪 */
			case TIMU_ANGLE_ID:
			{
				if(imu_data.data_ready_flag == DATA_NOT_READY) 
					imu_data.data_ready_flag = DATA_READY;
				T_imu_calcu(Rx2Message.StdId, CAN2_Rx_data);
				break;
			}
			default:
			{
				break;
			}
		}
		__HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
}

/**
  * @brief     get motor initialize offset value
  * @param     ptr: Pointer to a moto_measure_t structure
  * @retval    None
  * @attention this function should be called after system can init
  */
void get_moto_offset(moto_measure_t* ptr)
{
	ptr->ecd        = (uint16_t)(CAN1_Rx_data[0] << 8 | CAN1_Rx_data[1]);
	ptr->offset_ecd = ptr->ecd;
}

/**
  * @brief     get motor rpm and calculate motor round_count/total_encoder/total_angle
  * @param     ptr: Pointer to a moto_measure_t structure
  * @attention this function should be called after get_moto_offset() function
  */
void encoder_data_handler(moto_measure_t* ptr, uint8_t* rx_message)
{
	/* 转子转速 */
	ptr->speed_rpm     = (int16_t)(rx_message[2] << 8 | rx_message[3]);
	ptr->given_current = (int16_t)(rx_message[4] << 8 | rx_message[5]);

	/* 减速前编码值 */
	ptr->last_ecd = ptr->ecd;
	ptr->ecd      = (uint16_t)(rx_message[0] << 8 | rx_message[1]);

	/* 转子旋转圈数 */
	if (ptr->ecd - ptr->last_ecd >= 4096)
		ptr->round_cnt--;
	else if (ptr->ecd - ptr->last_ecd <= -4096)
		ptr->round_cnt++;

	/* 开机后总编码值 */
	ptr->total_ecd = ptr->round_cnt * 8192 + ptr->ecd - ptr->offset_ecd;
}

/**
  * @brief  send calculated current to motor
  * @param  CAN1 motor current
  */
void can1_send_message(int16_t TX_ID, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
	uint8_t FreeTxNum = 0; 
	
	Tx1Message.StdId = TX_ID;
	Tx1Message.IDE 	 = CAN_ID_STD;
	Tx1Message.RTR   = CAN_RTR_DATA;
  Tx1Message.DLC   = 0x08;

	CAN1_Tx_data[0] = iq1 >> 8;
	CAN1_Tx_data[1] = iq1;
	CAN1_Tx_data[2] = iq2 >> 8 ;
	CAN1_Tx_data[3] = iq2;
	CAN1_Tx_data[4] = iq3 >> 8;
	CAN1_Tx_data[5] = iq3;
	CAN1_Tx_data[6] = iq4 >> 8;
	CAN1_Tx_data[7] = iq4;
	
	/* 查询发送邮箱是否为空 */
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
	while(FreeTxNum == 0) 
	{  
    FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
  }
	
	HAL_CAN_AddTxMessage(&hcan1, &Tx1Message, CAN1_Tx_data, (uint32_t*)message1);
}

/**
  * @brief  send calculated current to motor
  * @param  CAN2 motor current
  */
void can2_send_message(int16_t TX_ID, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
	uint8_t FreeTxNum = 0; 
	
	Tx2Message.StdId = TX_ID;
	Tx2Message.IDE   = CAN_ID_STD;
	Tx2Message.RTR   = CAN_RTR_DATA;
  Tx2Message.DLC    = 0x08;
	
	CAN2_Tx_data[0] = iq1 >> 8;
	CAN2_Tx_data[1] = iq1;
	CAN2_Tx_data[2] = iq2 >> 8 ;
	CAN2_Tx_data[3] = iq2;
	CAN2_Tx_data[4] = iq3 >> 8;
	CAN2_Tx_data[5] = iq3;
	CAN2_Tx_data[6] = iq4 >> 8;
	CAN2_Tx_data[7] = iq4;
	
	/* 查询发送邮箱是否为空 */
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);  
	while(FreeTxNum == 0) 
	{  
    FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);  
  }
	
	HAL_CAN_AddTxMessage(&hcan2, &Tx2Message, CAN2_Tx_data, (uint32_t*)CAN_TX_MAILBOX0);
}

/**
  * @brief  init the can transmit and receive
  * @param  None
  */
void can_device_init(void)
{
  /* can filter config */
  CAN_FilterTypeDef  can_filter;

  can_filter.FilterBank           = 0;
  can_filter.FilterMode           = CAN_FILTERMODE_IDMASK;
  can_filter.FilterScale          = CAN_FILTERSCALE_32BIT;
  can_filter.FilterIdHigh         = 0x0000;
  can_filter.FilterIdLow          = 0x0000;
  can_filter.FilterMaskIdHigh     = 0x0000;
  can_filter.FilterMaskIdLow      = 0x0000;
  can_filter.FilterFIFOAssignment = CAN_FilterFIFO0;
  can_filter.SlaveStartFilterBank = 0;  
  can_filter.FilterActivation     = ENABLE;

  HAL_CAN_ConfigFilter(&hcan1, &can_filter);
  while (HAL_CAN_ConfigFilter(&hcan1, &can_filter) != HAL_OK);

	can_filter.FilterBank           = 14;
  HAL_CAN_ConfigFilter(&hcan2, &can_filter);  
	while (HAL_CAN_ConfigFilter(&hcan2, &can_filter) != HAL_OK);

	HAL_Delay(100);

	/* start the can transmit and receive */
	HAL_CAN_Start(&hcan1);
 	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
}

