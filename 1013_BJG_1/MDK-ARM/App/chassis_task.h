#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H

#include "stdint.h"

#define Protect_Mode    100		//保护模式
#define Y_Mode0         0
#define Y_Mode1         1			//向前直行模式
#define Y_Mode2         2			//向后直行模式
#define X_Mode1         3			//平移模式贴前墙
#define X_Mode2         4			//平移模式贴后墙
#define X_Mode3         5			//平移模式贴后墙
#define Turn_Mode 			6			//转弯模式
#define Y_Mode3         7			//向前低速直行模式
#define Y_Mode4         8			//向后低速直行模式
#define Vision_Mode1		9			//视觉模式（向前）
#define Vision_Mode2		10		//视觉模式（向后）
#define Vision_Aim_Mode	11		//视觉对位模式
#define Enter_Mode	12		//进垄模式
#define Exit_Mode  13  //出垄模式


#define SPEED   			 		450		//最快速度，满速1580，原本900
#define VISION_SPEED   		300			//除草时最大速度，视觉识别后减速满速
#define Turn_Low_SPEED		350			//转弯时横向前进速度
#define Mode_BUFF_SIZE 		1000

typedef struct
{
	float    vx; 
    float    vy; 
    float    vw; 						//整车速度
	int16_t  v[4];					//轮速度
	int16_t  target_v[4];		//轮目标速度
	int      pwm[4];				//pwm
}chassis_t;
void Ramp1_Stop(uint8_t way);
void printf_mode(void);
void switch_mode(uint8_t Next_Mode);
void Chassis_Task(void);
void Chassis_Init(void);
void mecanum_calc(float vx, float vy, float vw, int16_t speed[]);
void Speed_Pid_Cal(void);
void Ecd_Pid_Cal(void);
void mecanum_run(void);
void PID_CAL(void);
void Ramp_Start(void);
void Ramp_Stop(uint8_t way);
float Ramp_Input(float input,float target,float ramp);
void Debug_Function(void);
extern chassis_t chassis;
extern float Angle_Error_WT;						//陀螺仪的角度差值，调节车头直PID
extern float angle_set;							//目标角度，转90时+90即可
extern uint8_t Ctrl_Mode;							
extern uint8_t wall_flag;
extern uint16_t wall_distance;
extern uint32_t stop_time;							//记录保护模式制动停车时间
extern uint8_t ModeBuffArray[Mode_BUFF_SIZE];

extern float debug[2];
#endif
