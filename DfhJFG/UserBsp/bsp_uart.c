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

uint8_t dma_dbus_rec_buf[DMA_DBUS_LEN];				
uint8_t TFminiPlusBuffArray_Front[TFMINIPLUS_BUFF_SIZE];
uint8_t TFminiPlusBuffArray_Back[TFMINIPLUS_BUFF_SIZE];
uint8_t WT53RArrayLeft[WT53R_BUFF_LEN];
uint8_t WT53RArrayRight[WT53R_BUFF_LEN];
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
 /* 大激光串口 */
  if(huart->Instance == USART3)//前大激光数据处理
  {
    vTfFrontGetData(TFminiPlusBuffArray_Front);
    memset(TFminiPlusBuffArray_Front,0,TFMINIPLUS_BUFF_SIZE);
    HAL_UART_Receive_DMA(huart, (uint8_t*)TFminiPlusBuffArray_Front, TFMINIPLUS_BUFF_SIZE);
  }
  if(huart->Instance == UART4)//后大激光数据处理
  {
    vTfBackGetData(TFminiPlusBuffArray_Back);
    memset(TFminiPlusBuffArray_Back,0,TFMINIPLUS_BUFF_SIZE);
    HAL_UART_Receive_DMA(huart, (uint8_t*)TFminiPlusBuffArray_Back, TFMINIPLUS_BUFF_SIZE);
  }
  /* 小激光串口 */
  if(huart->Instance == UART2)//左小激光数据处理
  {
    vTfBackGetData(TFminiPlusBuffArray_Back);
    memset(TFminiPlusBuffArray_Back,0,TFMINIPLUS_BUFF_SIZE);
    HAL_UART_Receive_DMA(huart, (uint8_t*)TFminiPlusBuffArray_Back, TFMINIPLUS_BUFF_SIZE);
  }
  if(huart->Instance == UART5)//右小激光数据处理
  {
    vTfBackGetData(TFminiPlusBuffArray_Back);
    memset(TFminiPlusBuffArray_Back,0,TFMINIPLUS_BUFF_SIZE);
    HAL_UART_Receive_DMA(huart, (uint8_t*)TFminiPlusBuffArray_Back, TFMINIPLUS_BUFF_SIZE);
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
  /* 激光串口初始化 */ 
  __HAL_UART_ENABLE_IT(&TFHEAD_HUART,UART_IT_IDLE);   /* 车头激光 */
  HAL_UART_Receive_DMA(&TFHEAD_HUART,TFminiPlusBuffArray_Front,TFMINIPLUS_BUFF_SIZE);
  __HAL_UART_ENABLE_IT(&TFTAIL_HUART,UART_IT_IDLE);   /* 车尾激光 */
  HAL_UART_Receive_DMA(&TFTAIL_HUART,TFminiPlusBuffArray_Back,TFMINIPLUS_BUFF_SIZE);
  
    __HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);   /* 车尾激光 */
  HAL_UART_Receive_DMA(&huart5,TFminiPlusBuffArray_Back,TFMINIPLUS_BUFF_SIZE);
}

