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
  
}patStatus_e;

typedef struct patBoard{
  uint8_t status;
} patBoard_t;
void patSort_task(void const *argu);
void InitMode(void);
void __2PatPreMode(void);
void __1PatPreMode(void);
void __1PAT(void);
void __2PAT(void);

#endif

