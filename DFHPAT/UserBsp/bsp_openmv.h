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
#ifndef __BSP_OPENMV_H__
#define __BSP_OPENMV_H__

#include "stm32f4xx_hal.h"
#include "stdint.h"



typedef struct {
  uint8_t color1;
  uint8_t color2;
}openmvData_t;

enum {
 GREEN = 0,
  RED = 1,
 YELLOW
 
  
  
};

void openmvGetData(uint8_t *Data);

extern openmvData_t openmvData;
#endif

