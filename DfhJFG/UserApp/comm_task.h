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
#define VISION_LEN 4
/* motor current parameter structure */
typedef struct
{
  /* chassis motor current */
  int16_t chassis_2006_cur[4];
}motor_current_t;

typedef __packed struct {
  uint8_t SOF;
  uint8_t openCollectFlag;//只能打开一个
  uint8_t enableOpenFlag;//可以全部打开
  uint8_t EOF;
} visionMessage_t;


extern motor_current_t motor_cur;
extern visionMessage_t visionMessage;
void can_msg_send_task(void const *argu);
void msg_fdb_task(void const *argu);
void supercap_msg_send_task(void const *argu);

void vision_send_task(void const *argu);

#endif

