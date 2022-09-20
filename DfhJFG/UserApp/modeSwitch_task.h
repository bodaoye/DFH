/**
  * @file debug_task.h
  * @version 2.0
  * @date Jan,30th 2021
	*
  * @brief
	*
  *	@author YY
  *
  */
#ifndef __MODESWITCH_TASK_H__
#define __MODESWITCH_TASK_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum
{
  PROTECT_MODE	= 0x00,    //保护模式	     	
  STANDARD_MODE	= 0x01,    //标准模式
} ctrl_mode_e;

extern ctrl_mode_e 	ctrl_mode;

void modeSwitch_task(void const *argu);

#endif

