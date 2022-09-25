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
  REMOTE_MODE = 0x02
} ctrl_mode_e;

typedef struct {
	uint8_t key;
	uint8_t keyflag;
}key_t;

extern ctrl_mode_e 	ctrl_mode;
extern key_t         rc_ch5;
void modeSwitch_task(void const *argu);
void remote_unlock(void);
static void sw1_mode_handler(void);
static void sw2_mode_handler(void);
void get_sw_mode(void);
#endif

