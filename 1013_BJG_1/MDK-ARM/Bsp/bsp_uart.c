#include "bsp_uart.h"


/**
┌───────────────────────────────────────────────────────────────────
│  * @brief    usart idle interrupt                      						│
├───────────────────────────────────────────────────────────────────
│  * @param     huart: the identifier of usart        					  	│
├───────────────────────────────────────────────────────────────────
│  * @retval    None                                                │
├───────────────────────────────────────────────────────────────────
│  * @attention every usart intermit should transfer the function		│
└───────────────────────────────────────────────────────────────────
*/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart){
	if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)){ //判断是否是空闲中断
		__HAL_UART_CLEAR_IDLEFLAG(huart);                     //清除空闲中断标志（否则会一直不断进入中断）
		HAL_UART_DMAStop(huart);															//停止本次DMA运输
		USER_UART_IDLECallback(huart);                        //调用串口功能回调函数
	}
}


