#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H

#include "stdint.h"

#define Protect_Mode    100		//����ģʽ
#define Y_Mode0         0
#define Y_Mode1         1			//��ǰֱ��ģʽ
#define Y_Mode2         2			//���ֱ��ģʽ
#define X_Mode1         3			//ƽ��ģʽ��ǰǽ
#define X_Mode2         4			//ƽ��ģʽ����ǽ
#define X_Mode3         5			//ƽ��ģʽ����ǽ
#define Turn_Mode 			6			//ת��ģʽ
#define Y_Mode3         7			//��ǰ����ֱ��ģʽ
#define Y_Mode4         8			//������ֱ��ģʽ
#define Vision_Mode1		9			//�Ӿ�ģʽ����ǰ��
#define Vision_Mode2		10		//�Ӿ�ģʽ�����
#define Vision_Aim_Mode	11		//�Ӿ���λģʽ
#define Enter_Mode	12		//��¢ģʽ
#define Exit_Mode  13  //��¢ģʽ


#define SPEED   			 		450		//����ٶȣ�����1580��ԭ��900
#define VISION_SPEED   		300			//����ʱ����ٶȣ��Ӿ�ʶ����������
#define Turn_Low_SPEED		350			//ת��ʱ����ǰ���ٶ�
#define Mode_BUFF_SIZE 		1000

typedef struct
{
	float    vx; 
    float    vy; 
    float    vw; 						//�����ٶ�
	int16_t  v[4];					//���ٶ�
	int16_t  target_v[4];		//��Ŀ���ٶ�
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
extern float Angle_Error_WT;						//�����ǵĽǶȲ�ֵ�����ڳ�ͷֱPID
extern float angle_set;							//Ŀ��Ƕȣ�ת90ʱ+90����
extern uint8_t Ctrl_Mode;							
extern uint8_t wall_flag;
extern uint16_t wall_distance;
extern uint32_t stop_time;							//��¼����ģʽ�ƶ�ͣ��ʱ��
extern uint8_t ModeBuffArray[Mode_BUFF_SIZE];

extern float debug[2];
#endif
