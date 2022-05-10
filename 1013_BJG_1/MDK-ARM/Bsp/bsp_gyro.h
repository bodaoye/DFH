#ifndef __BSP_GYRO_H
#define __BSP_GYRO_H

#include "stm32f4xx.h"
#include "usart.h"

void WT101_Init(void);
void Get_Angle(void);

extern float angle_get;									//������yaw�Ƕ�
extern uint8_t angle_flag;   		
extern float Angle_Error_WT;						//�����ǵĽǶȲ�ֵ�����ڳ�ͷֱPID
extern float angle_set;							//Ŀ��Ƕȣ�ת90ʱ+90����		

#endif
