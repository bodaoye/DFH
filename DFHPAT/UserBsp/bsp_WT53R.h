#ifndef __BSP__WT53R__H__
#define __BSP__WT53R__H__

#include "stm32f4xx.h"
#include "usart.h"

#define char_d_start  25 //四字节整数的开始

/*TFminiplus 处理后的数据*/
extern uint16_t wt53r_distance[2];
void getWT53Rdis(uint8_t *buff,uint8_t num);
void Tof_Read_Data(uint8_t *Tof_Data);
#endif

