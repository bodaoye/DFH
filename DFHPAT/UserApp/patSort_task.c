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
#include "bsp_openmv.h"
#include "comm_task.h"
#define RUNTIME     100

#define smallPeriod 80
#define midPeriod 300
#define longPeriod 800

#define PAT1        1
#define PAT2        6
#define COLLECTION1 4
#define COLLECTION2 5
#define SORT1       3
#define SORT2       2

#define PAT1ServoAngleBegin    0
#define PAT1ServoAngleEnd      150
#define PAT2ServoAngleBegin    380
#define PAT2ServoAngleEnd      55
#define oneServoAngleBegin    0
#define SORT2ServoBegin  450
#define SORT2ServoForward  250
#define SORT2ServoBack     700      //RED
#define COLLECTION1ServoAngleEnd    450
#define COLLECTION1ServoAngleBegin  550
#define COLLECTION2ServoAngleBegin    420//380
#define COLLECTION2ServoAngleEnd  770
#define SORT1ServoBegin     500
#define SORT1ServoForward   700
#define SORT1ServooBack     300   //RED


/*
*   can_msg_send_task 
*/
patBoard_t pat;
chassisMessage_t chassisMessage;

uint16_t missColorFlag = 0;
uint16_t collect = 0;
uint8_t delayflag = 0;
uint16_t count = 0;
void color1_patSort_task(void const *argu) {
  pat.status = TASKMODE;
// chassis_rec_msg.openCollectFlag = 1;
	for(;;) {
    count++;
    /* 阻塞时间 */
      if(chassis_rec_msg.enableOpenFlag == 1) {
      collect = 1;
      } else {
      collect = 0;
      }
      
  if(openmvData.color1 == GREEN) {
    if(count>=3) {
      missColorFlag = openmvData.color1;
      count = 0;
    }
  }
  else if(openmvData.color1 == RED) {
      missColorFlag = openmvData.color1;
      count = 0;
  }
    else if(openmvData.color1 == YELLOW) {
      missColorFlag = openmvData.color1;
      count = 0;
  }
  osDelay(10);
 }
}

void color2_patSort_task(void const *argu) {
	for(;;) {
//    
//    if(collect == 1) { singleServerControl(COLLECTION1,COLLECTION1ServoAngleEnd,RUNTIME);osDelay(80);}//一号舵机}
//    else if (collect == 0) { singleServerControl(COLLECTION1,COLLECTION1ServoAngleBegin,RUNTIME);osDelay(80);}//一号舵机 osDelay(80);    
    if(missColorFlag == RED) {
      color1RED();
    } else if(missColorFlag == GREEN) {
      color1GREEN();
    } else if(missColorFlag == YELLOW) {
      color1YELLOW();
    }
      osDelay(10);
 }
 
}

void InitMode(void) {
  singleServerControl(PAT1,PAT1ServoAngleBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(PAT2,PAT2ServoAngleBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(COLLECTION1,COLLECTION1ServoAngleBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  if(chassis_rec_msg.openCollectFlag == 1 || chassis_rec_msg.enableOpenFlag == 1) {
    singleServerControl(COLLECTION2,COLLECTION2ServoAngleBegin,RUNTIME);//一号舵机
    osDelay(smallPeriod); 
  }
  singleServerControl(SORT1,SORT1ServoBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);   
  singleServerControl(SORT2,SORT2ServoBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);     
}

void OpenMode(void) {
  singleServerControl(PAT1,PAT1ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(PAT2,PAT2ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(COLLECTION1,COLLECTION1ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(COLLECTION2,COLLECTION2ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod); 
  singleServerControl(SORT1,SORT2ServoBack,RUNTIME);//一号舵机
  osDelay(smallPeriod);   
  singleServerControl(SORT2,SORT1ServooBack,RUNTIME);//一号舵机
  osDelay(smallPeriod);     
}

void color1RED(void) {
//  singleServerControl(COLLECTION1,COLLECTION1ServoAngleEnd,RUNTIME);//一号舵机
//  osDelay(smallPeriod);
  singleServerControl(SORT1,SORT2ServoBack,RUNTIME);//一号舵机
  osDelay(smallPeriod); 
  singleServerControl(PAT1,PAT1ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
}

void color1YELLOW(void) {
//  singleServerControl(COLLECTION1,COLLECTION1ServoAngleEnd,RUNTIME);//一号舵机
//  osDelay(smallPeriod);
  singleServerControl(SORT1,SORT2ServoForward,RUNTIME);//一号舵机
  osDelay(smallPeriod);  
  singleServerControl(PAT1,PAT1ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
}

void color1GREEN(void) {
//    singleServerControl(COLLECTION1,COLLECTION1ServoAngleBegin,RUNTIME);//一号舵机
//  osDelay(smallPeriod);
  singleServerControl(SORT1,SORT1ServoBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);

  singleServerControl(PAT1,PAT1ServoAngleBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
}

void color2RED(void) {
  singleServerControl(SORT2,SORT2ServoBack,RUNTIME);//一号舵机
  osDelay(smallPeriod);  
  singleServerControl(COLLECTION2,COLLECTION2ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(PAT2,PAT2ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
}

void color2GREEN(void) {
  singleServerControl(SORT2,SORT2ServoBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(COLLECTION2,COLLECTION2ServoAngleBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(PAT2,PAT2ServoAngleBegin,RUNTIME);//一号舵机
  osDelay(smallPeriod);
}

void color2YELLOW(void) {
  singleServerControl(SORT2,SORT2ServoForward,RUNTIME);//一号舵机
  osDelay(smallPeriod);  
  singleServerControl(COLLECTION2,COLLECTION2ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
  singleServerControl(PAT2,PAT2ServoAngleEnd,RUNTIME);//一号舵机
  osDelay(smallPeriod);
}

void color1CollectOpen(void) {
  if(chassis_rec_msg.openCollectFlag == 0 || chassis_rec_msg.enableOpenFlag == 1) {
    singleServerControl(COLLECTION1,COLLECTION1ServoAngleBegin,RUNTIME);//一号舵机
    osDelay(smallPeriod); 
  }
}