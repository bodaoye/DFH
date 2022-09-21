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



motor_current_t   motor_cur;

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
//				can1_send_supercap()  ;
				/* rudder current */
//				can2_send_message(0x1ff, motor_cur.chassis_6020_cur[0], motor_cur.chassis_6020_cur[1], 0, 0);
//				can1_send_message(0x1ff, 0, 0, motor_cur.chassis_6020_cur[2], motor_cur.chassis_6020_cur[3]);
				/* 0: Left  Front motor current
					 1: Left  Back  motor current
					 2: Right Front motor current
					 3: Right Back  motor current  */	
				can2_send_message(0x200, motor_cur.chassis_2006_cur[0], motor_cur.chassis_2006_cur[1], motor_cur.chassis_2006_cur[2], motor_cur.chassis_2006_cur[3]);
			}
		}
	}
}



