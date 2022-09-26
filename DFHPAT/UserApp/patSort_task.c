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

#include "patSort_task.h"
#include "cmsis_os.h"
#include "string.h"
#include "remote_msg.h"
#include "usart.h"
#include "bsp_ServerDriver.h"
#define sixServo 6
#define sixServoAngleBegin  380
#define sixServoAngleEnd    50
#define oneServoAngleBegin  0
#define oneServoAngleEnd    380
#define oneServoAngleBegin  0
#define twoServoAngleBeginx     500
#define twoServoAngleForward  0
#define twoServoAngleBack     0
#define foutServoAngleEnd    250
#define fourServoAngleBegin  500
#define fiveServoAngleEnd    380
#define fiveServoAngleBegin  500


/*
*   can_msg_send_task 
*/
uint16_t test = 380,id = 1;
void patSort_task(void const *argu) {
	for(;;) {
//  test++;
    singleServerControl(id,test,1);
//    singleServerControl(6,100,1000);
    osDelay(100);

	}
}



