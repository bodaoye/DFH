#include "chassis_task.h"
#include "cmsis_os.h"
#include "bsp_WT53R.h"
#include "bsp_TFMiniPlus.h"
#include "bsp_T_imu.h"
#include "bsp_limit.h"
#include "pid.h"

#include "modeSwitch_task.h"


/*----------------------------------------------------------------------------------------------------------------------*/
chassis_t       chassis;
/* 保存传感器底层传来的数据 */
chassisSenor_t chassisSenorData;
/* 大激光的最短距离  ！调 */
const uint16_t miniBigLaserDis = 300; 
/* 当前角度 与 下一时刻角度 */
int currentAngle, targetAngle;
/* 当前状态角标 */
chassisState_e chassisCurrentState;
/* 事件角标 */
chassisAction_e chassisNextAction;	
/* 底盘各部分状态机 */
chassisState_t chassisState;
/*----------------------------------------------------------------------------------------------------------------------*/
/**
	* @brief      底盘任务初始化
	* @author         
	* @param[in]  
	* @retval     
	*/
void chassis_param_init(void)
{
	for(int i=0; i<4; i++)
	{
		PID_struct_init(&pid_chassis_2006_spd[i], POSITION_PID, 16000, 0,
									  4.8f, 0.0f, 0.0f);
	}
}

/* 更新传感器数据 */
void getSenorData(void) {
		/* 获取小激光数据 */
	chassisSenorData.leftSmallLazer = wt53r_distance[0];
		/* 获取大激光数据 */
	chassisSenorData.headBigLazer = tfmin_distance[0];
		/* 获取陀螺仪数据 */
	chassisSenorData.imuYaw = imu_data.yaw;
	chassisSenorData.imuWy 	= imu_data.wy;
		/* 获取光电开关数据 */
	chassisSenorData.leftKey 	= limitStatus[0] ;
	chassisSenorData.rightKey = limitStatus[1];
	chassisSenorData.headKey 	= limitStatus[2];
	chassisSenorData.tailKey 	= limitStatus[3];
}

/**
  初始化底盘FSM的状态变量
		* chassisCurrentState 底盘当前状态
		* chassisNextAction		底盘执行状态
		*	currentAngle				底盘当前角度
		* targetAngle					底盘下一时刻角度
*/
void chassisStateInit(void) {
		chassisCurrentState = __idle;	// 初始化状态为空闲
		chassisNextAction	 = 	 idle;	//初始化事件为加速
		currentAngle = 0;		//初始化当前角度为0
		targetAngle = 0;		//初始化目标角度为0
		chassisState.chassisIdleState = idleTurnToIdle;
		chassisState.chassisGostraightState = goStraightTurnToIdle;
}
/**
  查表
  改返回值决定了指针指向
*/
chassisState_e lookUpTable[4][4] = {
	[__stop] 				 = {	__goStraight, __stop, __rotate, 			__idle	},
	[__goStraight]	 = {	__goStraight, __stop, __idle, 				__idle	},
	[__rotate] 			 = {	__goStraight,	__idle, __idle, 				__idle	},
	[__idle] 				 = {	__goStraight,	__idle,	__idle,					__idle	},
};

/*-------------------------------------------------------------------------------------------------------------------*/
/**
	*	状态函数实现 
	*	当前状态下的行为
	* 返回当前角度和目标角度
*/
chassisAction_e event_idle(int *currentAngle, int *targetAngle) {
	/* 根据传感器更新状态机 */
	if(chassisSenorData.leftKey == 0 && chassisSenorData.rightKey == 0) { 				//使用光电反馈判断开机
		chassisState.chassisIdleState = idleTurnToStraight;
	} else if (chassisSenorData.leftKey == 1 || chassisSenorData.rightKey == 1) {	//使用光电反馈判断是否处于不准开机的空闲模式
		chassisState.chassisIdleState = idleTurnToIdle;
	} else {																																			//使用光电反馈判断是否处于不准开机的空闲模式
		chassisState.chassisIdleState = idleTurnToIdle;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisIdleState) {
		case idleTurnToStraight: {
			return speedUp;
			break;
		}
		case idleTurnToStop: {
			return speedDown;
			break;
		}
		case idleTurnToRotate:{
			return rotate;
			break;
		}
		case idleTurnToIdle:{
			return speedDown;
			break;
		}
	}
}

chassisAction_e event_stop(int *currentAngle, int *targetAngle) {
	/* 根据传感器更新状态机 */
	if(chassisSenorData.leftKey == 0 && chassisSenorData.rightKey == 0) {						//使用轮速反馈判断是否完全刹车
		chassisState.chassisStopState = stopTurnToStop;
	} else if (chassisSenorData.leftKey == 1 || chassisSenorData.rightKey == 1) {		//使用轮速反馈判断是否完全刹车
		chassisState.chassisStopState = stopTurnToRotate;
	} else {
		chassisState.chassisStopState = stopTurnToIdle;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisStopState) {
		case stopTurnToStraight: {
			return speedUp;
			break;
		}
		case stopTurnToStop: {
			return speedDown;
			break;
		}
		case stopTurnToRotate:{
			return rotate;
			break;
		}
		case stopTurnToIdle:{
			return speedDown;
			break;
		}
	}
}

chassisAction_e event_goStraight(int *currentAngle, int *targetAngle) {
	/* 根据传感器更新状态机 */
	if(chassisSenorData.headBigLazer > miniBigLaserDis) {									//使用大激光反馈判断是否加速
		chassisState.chassisGostraightState = goStraightTurnToStraight;
	} else if (chassisSenorData.headBigLazer <= miniBigLaserDis ) {				//使用大激光反馈判断是否刹车
		chassisState.chassisGostraightState = goStraightTurnToStop;
	} else {
		chassisState.chassisGostraightState = idleTurnToStraight;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisGostraightState) {
		case goStraightTurnToStraight: {
			return speedUp;
			break;
		}
		case goStraightTurnToStop: {
			return speedDown;
			break;
		}
		case goStraightTurnToRotate:{
			return rotate;
			break;
		}
		case goStraightTurnToIdle:{
			return speedDown;
			break;
		}
	}
}

chassisAction_e event_rotate(int *currentAngle, int *targetAngle) {
	/* 根据传感器更新状态机 */
	if(*currentAngle != *targetAngle ) { 												//根据角度对比确定是否需要继续转弯
		chassisState.chassisRotateState = rotateTurnToRotate;
	} else if (currentAngle ==targetAngle) {										//根据角度对比确定是否需要继续转弯
		chassisState.chassisRotateState = rotateTurnToStraight;
	} else {
		chassisState.chassisRotateState = rotateTurnToStop;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisRotateState) {
		case rotateTurnToStraight: {
			return speedUp;
			break;
		}
		case rotateTurnToStop: {
			return speedDown;
			break;
		}
		case rotateTurnToRotate:{
			return rotate;
			break;
		}
		case rotateTurnToIdle:{
			return speedDown;
			break;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------*/

/**
 	状态函数指针声明,指针指向了哪个函数，就执行哪种行为
*/
chassisAction_e (* afEventHandler[])(int *currentAngle, int *targetAngle) = { event_stop, event_goStraight, event_rotate,event_idle };

void chassis_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();

		chassisAction_e (* eventFunction)(int *currentAngle, int *targetAngle);
    
    for (;;) {
			/*
				FSM流程：0. 当前状态初始化
			
								 获取函数指针 指向的状态函数
								 事件函数内部   通过条件判断    返回当前事件    或执行当前状态
								 事件    和     当前状态     合成的二维数组   确定了    唯一的下一个状态或维持当前状态 
			*/
			switch(ctrl_mode) {
        case PROTECT_MODE: {
          memset(chassis.current_2006, 0, sizeof(chassis.current_2006));
          break;
        }
        case STANDARD_MODE: {
          eventFunction		 		=  	afEventHandler[chassisCurrentState];				//将状态函数指针交给定义的临时指针
          chassisNextAction 	=  	eventFunction(&currentAngle, &targetAngle);	//执行当前状态并获取下一状态的角标
          chassisCurrentState = 	lookUpTable[chassisCurrentState][chassisNextAction];//更新当前状态角标
           /* 麦轮解算 */
          mecanum_calc(chassis.spd_input.vx, chassis.spd_input.vy, chassis.spd_input.vw, chassis.wheel_spd_input);
          /* PID计算电机控制电流 */
          chassis_pid_calcu();
          break;
        }
      }
      memcpy(motor_cur.chassis_2006_cur, chassis.current_2006, sizeof(chassis.current_2006));
			osDelayUntil(&thread_wake_time, 1);
    }
}

/**
  * @brief 麦轮解算函数
  * @param input : ?=+vx(mm/s)  ?=+vy(mm/s)  ccw=+vw(deg/s)
  *        output: every wheel speed(rpm)
  * @note  1=FL 2=FR 3=BL 4=BR
  */
void mecanum_calc(float vx, float vy, float vw, float speed[])
{
	float wheel_rpm[4];
	wheel_rpm[0] =    + vx  +vy + vw;	 	//+	
	wheel_rpm[1] = 	 +vx - vy + vw; 	//-
	wheel_rpm[2] =    -vx  - vy + vw;		//-
	wheel_rpm[3] =  -vx + vy + vw;		//+
	memcpy(speed, wheel_rpm, 4*sizeof(float));
	
}

/**
  * @brief      底盘PID
  * @author         
  * @param[in]  
  * @retval     
  */
void chassis_pid_calcu(void) {
	for(uint8_t i=0; i<4; i++) {
		/* 底盘电机速度环 */
		chassis.wheel_spd_fdb[i] = moto_msg.chassis_2006[i].speed_rpm;
		chassis.current_2006[i]  = (int16_t)pid_calc(&pid_chassis_3508_spd[i], chassis.wheel_spd_fdb[i], chassis.wheel_spd_ref[i]);

	}
}