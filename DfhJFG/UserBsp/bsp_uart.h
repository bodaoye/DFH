#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "usart.h"

#define 	  DBUS_HUART        huart1
#define   	TFHEAD_HUART      huart4		
#define   	TFTAIL_HUART		  huart3
#define     WT53RLEFT_HUART   huart6
#define     WT53RRIGHT_HUART  huart2
#define   	VISION_HUART		  huart5

#define  	DMA_DBUS_LEN		18
/*TFminiplus 数据缓存大小*/
#define TFMINIPLUS_BUFF_SIZE 50
#define WT53R_BUFF_LEN 50

void USER_UART_Init(void);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_UART_IDLECallback(UART_HandleTypeDef *huart);

#endif

