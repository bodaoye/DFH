/**
  * @file debug_task.h
  * @version 2.0
  * @date Jan,30th 2021
	*
  * @brief
	*
  *	@author YY
  *
  */
#ifndef __CHASSIS_TASK_H__
#define __CHASSIS_TASK_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include "bsp_limit.h"
#define MAXSPEED 13000
#define CURRENT_ANGLE_ABS_DATA 0.5
#define TOWARD_ANGLE1  90
#define TOWARD_ANGLE2  270
#define TOWARD_ANGLE3  180

#define FIND_ANGLE1    0
#define FIND_ANGLE2    0
#define FIND_ANGLE3    360

typedef struct {
		/* С���� */
	uint16_t leftSmallLazer;
		/* �󼤹� */
	uint16_t headBigLazer;
	uint16_t tailBigLazer;
		/* ������ */
	float imuYaw;
	float imuWy;
		/* ��翪�� */
	uint8_t leftHeadKey;
	uint8_t rightHeadKey;
	uint8_t leftTailKey;
	uint8_t	rightTailKey;
	
	int16_t  speed_rpm[4];
  
}chassisSenor_t;

typedef struct {
	/* ֹͣ�˶�״̬�� */
	uint8_t chassisStopState;
	/* ֱ��״̬�� */
	uint8_t chassisGostraightState;
	/* ��ת״̬�� */
	uint8_t chassisRotateState;
	/* ����״̬�� */
	uint8_t chassisIdleState;
  /* ��ǰ�Ƕ� �� ��һʱ�̽Ƕ� */
  float currentAngle;
  float targetAngle;
  float angleError;
  /* Ŀ����ǽ���� */
  float targedis;
}chassisState_t;

/*FSM Data*/
typedef enum
{
	__stop = 0, 
	__goStraight = 1,
	__rotate = 2,
	__idle
}chassisState_e;

/*FSM Data*/
typedef enum
{
	idleTurnToStraight = 0,
	idleTurnToStop = 1,
	idleTurnToRotate = 2,
	idleTurnToIdle = 3,
	goStraightTurnToStraight = 4,
	goStraightTurnToStop = 5,
	goStraightTurnToRotate = 6,
	goStraightTurnToIdle = 7,
	stopTurnToStraight = 8,
	stopTurnToStop = 9,
	stopTurnToRotate = 10,
	stopTurnToIdle = 11,
	rotateTurnToStraight = 12,
	rotateTurnToStop = 13,
	rotateTurnToRotate = 14,
	rotateTurnToIdle = 15
}chassisIdleState_e;

/*FSM EVENT*/
typedef enum
{
	speedUp = 0, 
	speedDown = 1,
	rotate = 2,
	idle
}chassisAction_e;

typedef struct 
{
	float vx;
	float vy;
	float vw;
}spd_t;

/* chassis parameter structure */
typedef struct
{
	spd_t     spd_input; //(����/ң����)������������ٶ�

  float     chassis_angle_fdb;
  float     chassis_angle_ref;
  float     chassis_angle_error;
  
  float     chassis_stop_dis_fdb;
  float     chassis_stop_dis_ref;
  float     chassis_stop_dis_error;
  
  float     chassis_stickdis_fdb;
  float     chassis_stickdis_ref;
  float     chassis_stickdis_error;
  
  float   	wheel_spd_input[4]; //���ֽ���������������
	float   	wheel_spd_ref[4];		//�ٶ��ط������������Ŀ��
	float   	wheel_spd_fdb[4];		//������ٷ���
  
  int16_t		current_2006[4];
  
	float     wheel_max;
}chassis_t;

extern chassisState_e chassisCurrentState;
extern chassisAction_e chassisNextAction;
extern chassisSenor_t chassisSenorData;	
extern chassis_t       chassis;
extern chassisState_t chassisState;

chassisAction_e event_idle(float *currentAngle, float *targetAngle);
chassisAction_e event_stop(float *currentAngle, float *targetAngle);
chassisAction_e event_goStraight(float *currentAngle, float *targetAngle);
chassisAction_e event_rotate(float *currentAngle, float *targetAngle);
void chassis_param_init(void);
void chassisStateInit(void);
void chassis_task(void const *argu);
void getSenorData(void);
void mecanum_calc(float vx, float vy, float vw, float speed[]);
void chassis_pid_calcu(void);
float chassTrunAnyAngle(float *targetAngle);
float chassStickToAnyDistance(float *dis);
int getFarmData(uint8_t *Head, uint8_t *Tail,limitKey_t *key1);
void farmLimitUpdate(limitKey_t *key1);
void stop(int step);
void gogogo(void);

#endif

