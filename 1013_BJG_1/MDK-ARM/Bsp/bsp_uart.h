 #ifndef		_BSP_USART_H_
#define		_BSP_USART_H_
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "bsp_sjg.h"
int fgetc(FILE *f);
void Usart_SendString(uint8_t *str);
void USER_UART_IDLECallback(UART_HandleTypeDef *huart);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);

#endif





