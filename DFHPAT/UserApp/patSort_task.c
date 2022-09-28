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
#define RUNTIME     100

#define PAT1        1
#define PAT2        6
#define COLLECTION1 4
#define COLLECTION2 5
#define SORT1       3
#define SORT2       2

#define sixServoAngleBegin    380
#define sixServoAngleEnd      50
#define oneServoAngleBegin    0
#define oneServoAngleEnd      380
#define oneServoAngleBegin    0
#define twoServoAngleBeginx  500
#define twoServoAngleForward  0
#define twoServoAngleBack     0
#define fourServoAngleEnd    380
#define fourServoAngleBegin  580
#define fiveServoAngleBegin    400//380
#define fiveServoAngleEnd  500
#define threeServoBeginx     0
#define threeServoFordward   0
#define threeServoBack       0


/*
*   can_msg_send_task 
*/
patBoard_t pat;


uint16_t test = 380,id = 1;
void patSort_task(void const *argu) {
	for(;;) {
InitMode();
  switch(pat.status) {
    case 0: {
 //     InitMode();
      break;
    }
    case 1: {
      
    }
  
  }
    osDelay(100);
	}
}

void InitMode(void) {
  singleServerControl(PAT1,oneServoAngleBegin,RUNTIME);//一号舵机
  osDelay(100);
  singleServerControl(PAT2,oneServoAngleEnd,RUNTIME);
  osDelay(100);
  singleServerControl(COLLECTION1,fourServoAngleBegin,RUNTIME);
  osDelay(100);
  singleServerControl(COLLECTION2,fiveServoAngleBegin,RUNTIME);
  osDelay(100);
  singleServerControl(SORT2,twoServoAngleBeginx,RUNTIME);
  osDelay(100);
//  singleServerControl(SORT2,);

}

void __1PatPreMode(void) {
  
}

void __2PatPreMode(void) {

}

void __1PAT(void) {

}

void __2PAT(void) {

}