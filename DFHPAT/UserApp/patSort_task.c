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
/*
*   can_msg_send_task 
*/
int test = 0;
void patSort_task(void const *argu) {
	for(;;) {
    singleServerControl(2,0,100);
//    singleServerControl(6,100,1000);
    osDelay(100);

	}
}



