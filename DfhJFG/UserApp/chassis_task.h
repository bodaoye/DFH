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
	uint8_t leftKey;
	uint8_t rightKey;
	uint8_t headKey;
	uint8_t	tailKey;
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
	idleTurnToStop,
	idleTurnToRotate,
	idleTurnToIdle,
	goStraightTurnToStraight,
	goStraightTurnToStop,
	goStraightTurnToRotate,
	goStraightTurnToIdle,
	stopTurnToStraight,
	stopTurnToStop,
	stopTurnToRotate,
	stopTurnToIdle,
	rotateTurnToStraight,
	rotateTurnToStop,
	rotateTurnToRotate,
	rotateTurnToIdle
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

  float   	wheel_spd_input[4]; //���ֽ���������������
	float   	wheel_spd_ref[4];		//�ٶ��ط������������Ŀ��
	float   	wheel_spd_fdb[4];		//������ٷ���
  int16_t		current_2006[4];
  
	float     wheel_max;
}chassis_t;
void chassis_param_init(void);
void chassisStateInit(void);
void chassis_task(void const *argu);
void getSenorData(void);
void mecanum_calc(float vx, float vy, float vw, float speed[]);
void chassis_pid_calcu(void);

#endif

