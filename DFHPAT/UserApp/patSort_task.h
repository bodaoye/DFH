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
#ifndef __PATSORT_TASK_H__
#define __PATSORT_TASK_H__

#include "stm32f4xx_hal.h"
#include "stdint.h"


typedef enum{
  INITMODE,
  OPENMODE,
  TASKMODE
  
}patStatus_e;

typedef struct patBoard{
  uint8_t status;
} patBoard_t;

typedef struct chassisMessage{
  uint8_t flag;
} chassisMessage_t;

void color1_patSort_task(void const *argu);
void color2_patSort_task(void const *argu);
void InitMode(void);
void OpenMode(void);


void color1RED(void);
void color1GREEN(void);
void color1YELLOW(void);
void color2GREEN(void);
void color2RED(void);
void color2YELLOW(void);
#endif

