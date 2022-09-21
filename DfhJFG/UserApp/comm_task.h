/** 
  * @file comm_task.h
  * @version 2.0
  * @date Dec,26th 2020
	*
  * @brief
	*
  *	@author YY
  *
  */
#ifndef __COMM_TASK_H__
#define __COMM_TASK_H__

#include "stm32f4xx_hal.h"

#define CHASSIS_MOTOR_MSG_SEND    ( 1 << 0 )

/* motor current parameter structure */
typedef struct
{
  /* chassis motor current */
  int16_t chassis_2006_cur[4];
}motor_current_t;


extern motor_current_t motor_cur;

void can_msg_send_task(void const *argu);
void msg_fdb_task(void const *argu);
void supercap_msg_send_task(void const *argu);

#endif

