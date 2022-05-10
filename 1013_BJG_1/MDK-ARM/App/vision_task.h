#ifndef __VISION_TASK_H
#define __VISION_TASK_H

#include "stdint.h"

#define  DJ_SET_PWM  		TIM14->CCR1
#define  DJ_PWM_NORMAL	270
#define  DJ_PWM_CLIP		280
#define  DJ_PWM_PUT			250
#define  DJ_PWM_RELAX		120
#define  GEAR_ECD				33000
#define  BELT_ECD				36000

void Servo_Work(void);
void aim_yellow(void);
void Vision_Task(void);
void Gimbal_Stop(void);
void Get_Vision_Msg(void);
void Vision_Task_Init(void);
void Gimbal_Work(uint8_t way);
void Gimbal_Cascade_PID(int16_t target);


typedef struct
{
	int  ecd;				//0为齿轮电机  1为同步带电机
	int  v;
	int  target_ecd;
	int  error_ecd;
	int  pwm;
}gimbal_t;

typedef struct
{
	float distance_x;
	float distance_y;
}vision_t;

extern gimbal_t gimbal;
extern vision_t vision;
extern float  target_ecd;





#endif







