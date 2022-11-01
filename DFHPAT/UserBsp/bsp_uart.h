#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "usart.h"

#define 	  DBUS_HUART        huart1
#define   	COLOR1_HUART      huart5		
#define     CHASSIS_HUART     huart6
#define  	DMA_DBUS_LEN		18
#define COLOR1_BUFFER_LEN 3
#define CHASSIS_BUFFER_LEN 4
void USER_UART_Init(void);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_UART_IDLECallback(UART_HandleTypeDef *huart);

#endif

