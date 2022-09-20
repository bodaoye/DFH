/**
  * @file debug_task.h
  * @version 2.0
  * @date Jan,30th 2021
	*
  * @brief
	*
  *	@author JFG
  *
  */
#ifndef __DEBUG_TASK_H__
#define __DEBUG_TASK_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

void debug_task(void const *argu);
void DataWave(UART_HandleTypeDef* huart);

#endif

