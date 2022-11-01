 /** 
  * @file bsp_uart.c
  * @version 2.0
  * @date July,30th,2020
	*
  * @brief  底层串口配置
	*
  *	@author JFG
  *
  */                                                                                               
#include "bsp_uart.h"
#include "stdlib.h"
#include "string.h"
#include "bsp_TFMiniPlus.h"
#include "remote_msg.h"
#include "bsp_WT53R.h"
#include "bsp_openmv.h"
#include "comm_task.h"

uint8_t dma_dbus_rec_buf[DMA_DBUS_LEN];
uint8_t chassis_buff[CHASSIS_BUFFER_LEN];				
uint8_t corlor1_buff[COLOR1_BUFFER_LEN];

 /**
  * @brief  各个串口功能函数
  * @param 	UART_HandleTypeDef *huart
  * @retval 无
  */
void USER_UART_IDLECallback(UART_HandleTypeDef *huart)
{
 /* DBUS串口 */
  if(huart->Instance == USART1)//遥控器
  {
	rc_callback_handler(&rc, dma_dbus_rec_buf);
	memset(dma_dbus_rec_buf, 0, DMA_DBUS_LEN);
	HAL_UART_Receive_DMA(&DBUS_HUART, dma_dbus_rec_buf, DMA_DBUS_LEN);
  }
  if(huart->Instance == UART5)//前大激光数据处理
  {
    openmvGetData(corlor1_buff);
    memset(corlor1_buff,0,COLOR1_BUFFER_LEN);
    HAL_UART_Receive_DMA(huart, (uint8_t*)corlor1_buff, COLOR1_BUFFER_LEN);
  }
    if(huart->Instance == USART6)//前大激光数据处理
  {
    getChassisData(chassis_buff);
     memset(chassis_buff,0,CHASSIS_BUFFER_LEN);
    HAL_UART_Receive_DMA(huart, (uint8_t*)chassis_buff, CHASSIS_BUFFER_LEN);
  }
}

/**
  * @brief 串口空闲中断（需在it.c中每个串口的中断中调用该函数）
  * @param UART_HandleTypeDef *huart
  * @retval 无
  */
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   //判断是否是空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);                     //清除空闲中断标志（否则会一直不断进入中断）
		HAL_UART_DMAStop(huart);															//停止本次DMA运输
		USER_UART_IDLECallback(huart);                        //调用串口功能函数
	}
}

/**
* @brief 串口初始化:使能串口空闲中断,开启串口DMA接收
* @param  无
* @retval 无
*/
void USER_UART_Init(void)
{
  /* 遥控器串口初始化 */
	__HAL_UART_ENABLE_IT(&DBUS_HUART, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&DBUS_HUART, dma_dbus_rec_buf, DMA_DBUS_LEN);
  /* COLOR1串口初始化 */ 
  __HAL_UART_ENABLE_IT(&COLOR1_HUART,UART_IT_IDLE);   /* COLOR1 */
  HAL_UART_Receive_DMA(&COLOR1_HUART,corlor1_buff,COLOR1_BUFFER_LEN);
  /* 底盘版初始化 */
  __HAL_UART_ENABLE_IT(&CHASSIS_HUART,UART_IT_IDLE);   /* COLOR1 */
  HAL_UART_Receive_DMA(&CHASSIS_HUART,chassis_buff,CHASSIS_BUFFER_LEN);

}

