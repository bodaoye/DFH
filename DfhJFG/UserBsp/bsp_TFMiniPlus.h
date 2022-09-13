#ifndef __TF_MINI_PLUS_H
#define __TF_MINI_PLUS_H

#include "stm32f4xx.h"
#include "usart.h"


/*TFminiplus 处理后的数据*/
extern uint16_t tfmin_distance[2];

void vTfFrontGetData(uint8_t *buff);
void vTfBackGetData(uint8_t *buff);

#endif

