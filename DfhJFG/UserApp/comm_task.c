/** 
  * @file comm_task.c
  * @version 2.0
  * @date Dec,26th 2020
	*
  * @brief  交互任务文件
	*
  *	@author YY
  *
  */
#include "comm_task.h"
#include "bsp_can.h"
#include "cmsis_os.h"
#include "pid.h"
#include "string.h"
#include "remote_msg.h"
#include "usart.h"
#include "chassis_task.h"

visionMessage_t visionMessage;
motor_current_t   motor_cur;
uint8_t vision_msg_buff[VISION_LEN] = {0};
//  uint8_t supercap_control_msg[7] = {0};
  
/*
*   can_msg_send_task 
*/
void
can_msg_send_task(void const *argu) {
	osEvent event;
	for(;;) {
		event = osSignalWait(CHASSIS_MOTOR_MSG_SEND, osWaitForever);

		if (event.status == osEventSignal) {
			if (event.value.signals & CHASSIS_MOTOR_MSG_SEND) {
				/* 0: Left  Front motor current
					 1: Left  Back  motor current
					 2: Right Front motor current
					 3: Right Back  motor current  */	
				can1_send_message(0x200, motor_cur.chassis_2006_cur[0], motor_cur.chassis_2006_cur[1], motor_cur.chassis_2006_cur[2], motor_cur.chassis_2006_cur[3]);
			}
		}
	}
}

void vision_send_task(void const *argu) {
  uint32_t mode_wake_time = osKernelSysTick();
  for(;;) {
   visionMessage.SOF = 0x11;
   visionMessage.openCollectFlag = visionCtr.openCollect1;
   visionMessage.enableOpenFlag = visionCtr.enableOpenALL;
   visionMessage.EOF = 0x2b;
   memcpy(vision_msg_buff, &visionMessage, VISION_LEN);
   HAL_UART_Transmit_DMA(&huart5, vision_msg_buff, VISION_LEN);
   osDelayUntil(&mode_wake_time,1);
  }

}

