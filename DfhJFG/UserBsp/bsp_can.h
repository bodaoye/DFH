/** 
  * @file bap_can.h
  * @version 2.0
  * @date Dec,2th 2020
  *
  * @brief  
  *
  * @author YY
  *
  */
#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#include "can.h"
/* CAN1 send and receive ID */
typedef enum
{
	CAN_3508_STIR_ID	= 0x201,

	CAN_FRIC_M3508_LEFT_ID   = 0x202,
	CAN_FRIC_M3508_RIGHT_ID  = 0x203,
	
  CAN_LIMIT_2006_ID  = 0x204,
  
	CAN_POWER_ID  		= 0x003,
} can1_id_e;

/* CAN2 send and receive ID */
typedef enum
{
	CAN_YAW_MOTOR_ID     		 = 0x206,
	CAN_PIT_MOTOR_ID    		 = 0x205,
	
	TIMU_PALSTANCE_ID 	 		 = 0x001,
	TIMU_ANGLE_ID   				 = 0x002,
} can2_id_e;

/* can receive motor parameter structure */
typedef struct
{
  uint16_t offset_ecd;
	uint16_t ecd;
  uint16_t last_ecd;
	int32_t  total_ecd;

  int16_t  speed_rpm;
  int16_t  given_current;

  int32_t  round_cnt;

  uint32_t msg_cnt;
} moto_measure_t;

/*moto received messages structure*/
typedef struct
{
	moto_measure_t fric[2];
	moto_measure_t stir;
	moto_measure_t pit;
	moto_measure_t yaw;
  moto_measure_t limit;
} MOTO_RecvMsg_t; 

extern MOTO_RecvMsg_t	moto_msg;
extern CAN_TxHeaderTypeDef Tx1Message;

void get_moto_offset(moto_measure_t* ptr);
void encoder_data_handler(moto_measure_t* ptr, uint8_t* rx_message);

void can_device_init(void);
void can1_send_message(int16_t TX_ID, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void can2_send_message(int16_t TX_ID, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);

#endif

