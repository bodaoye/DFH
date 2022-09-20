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

typedef struct {
		/* 小激光 */
	uint16_t leftSmallLazer;
		/* 大激光 */
	uint16_t headBigLazer;
	uint16_t tailBigLazer;
		/* 陀螺仪 */
	float imuYaw;
	float imuWy;
		/* 光电开关 */
	uint8_t leftKey;
	uint8_t rightKey;
	uint8_t headKey;
	uint8_t	tailKey;
}chassisSenor_t;

typedef struct {
	/* 停止运动状态机 */
	uint8_t chassisStopState;
	/* 直行状态机 */
	uint8_t chassisGostraightState;
	/* 旋转状态机 */
	uint8_t chassisRotateState;
	/* 空闲状态机 */
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
void chassisStateInit(void);
void chassis_task(void const *argu);
void getSenorData(void);

#endif

