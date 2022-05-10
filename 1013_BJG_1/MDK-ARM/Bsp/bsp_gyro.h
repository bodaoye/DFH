#ifndef __BSP_GYRO_H
#define __BSP_GYRO_H

#include "stm32f4xx.h"
#include "usart.h"

void WT101_Init(void);
void Get_Angle(void);

extern float angle_get;									//陀螺仪yaw角度
extern uint8_t angle_flag;   		
extern float Angle_Error_WT;						//陀螺仪的角度差值，调节车头直PID
extern float angle_set;							//目标角度，转90时+90即可		

#endif
