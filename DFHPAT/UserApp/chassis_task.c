#include "chassis_task.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "bsp_WT53R.h"
#include "bsp_TFMiniPlus.h"
#include "bsp_T_imu.h"
#include "remote_msg.h"
#include "pid.h"
#include "math_calcu.h"

#include "modeSwitch_task.h"
#include "comm_task.h"

//#define     __Debug
//#define   __ADPID
#define   _NoDebug
#define   __REMOTE

extern TaskHandle_t can_msg_send_task_t;
/*----------------------------------------------------------------------------------------------------------------------*/
chassis_t       chassis;
/* 保存传感器底层传来的数据 */
chassisSenor_t chassisSenorData;
/* 大激光的最短距离  ！调 */
uint16_t miniBigLaserDis = 50;
const uint16_t miniSpm		   = 500;
const uint16_t mindis        =  60;
/* 当前状态角标 */
chassisState_e chassisCurrentState;
/* 事件角标 */
chassisAction_e chassisNextAction;	
/* 底盘各部分状态机 */
chassisState_t chassisState;
/* 有限状态机回调函数 */
chassisAction_e (* eventFunction)(float *currentAngle, float *targetAngle);

/**
  查表
  改返回值决定了指针指向
*/
chassisState_e lookUpTable[4][4] = {
	[__stop] 				 = {	__goStraight,	 __stop, 	__rotate, 				__idle	},
	[__goStraight]	 = {	__goStraight,	 __stop, 	__idle, 				  __idle	},
	[__rotate] 			 = {	__goStraight,	 __idle,	__rotate, 				__idle	},
	[__idle] 				 = {	__goStraight,	 __idle,	__idle,					  __idle	},
};

/**
 	状态函数指针声明,指针指向了哪个函数，就执行哪种行为
*/
chassisAction_e (* afEventHandler[])(float *currentAngle, float *targetAngle) = { event_stop, event_goStraight, event_rotate,event_idle };

/*----------------------------------------------------------------------------------------------------------------------*/
/**
	* @brief      底盘任务初始化
	* @author         
	* @param[in]  
	* @retval     
	*/
void chassis_param_init(void)
{
		PID_struct_init(&pid_chassis_2006_spd[0], POSITION_PID, 16000, 0,
									  12.0f, 0.1f, 0.0f);
                    		PID_struct_init(&pid_chassis_2006_spd[1], POSITION_PID, 16000, 0,
									  12.0f, 0.1f, 0.0f);
                    		PID_struct_init(&pid_chassis_2006_spd[2], POSITION_PID, 16000, 0,
									  12.0f, 0.1f, 0.0f);
                    		PID_struct_init(&pid_chassis_2006_spd[3], POSITION_PID, 16000, 0,
									  12.0f, 0.1f, 0.0f);

  PID_struct_init(&pid_chassis_angle_pos, POSITION_PID, 16000, 0,
									  240.0f, 0.0f, 0.0f);
  PID_struct_init(&pid_chassis_stickdis_pos, POSITION_PID, 23000, 0,
									  35.0f, 0.0f, 1.0f); 
  PID_struct_init(&pid_chassis_stop_pos, POSITION_PID, MAXSPEED, 0,
									  10.0f, 0.0f, 1.0f);                     
                    
}

  /* 更新过垄信息 */
int getFarmData(uint8_t *Head, uint8_t *Tail,limitKey_t *key1) {
  	/* 将两个光电开关的状态抽象成一个开关状态 */ 
	if((*Head == 0 && *Tail == 0)) { //两开关都没有被遮挡且没有经历过障碍
		key1->key = 0;//抽象开关关闭，继续执行
	} else {
		key1->key = 1;//抽象开关打开，继续执行
		key1->keyflag = 1;///* 对已经通过后的状态进行记录 */记录为已经经过
	}
	/*对抽象过的开关状态进行扫描 */
	if(key1->key == 0 && key1->keyflag == 1) {//如果已经经过而且抽象开关再次关闭，停止向前
		key1 -> key = 0;
		key1 -> keyflag = 1;
		return 1;//切换到下一状态
	}
	if(key1->key == 1 && key1->keyflag == 1) {
		return 0;
	}
	if(key1->key == 0 && key1->keyflag == 0) {
		return 0;
	} 
}
/*找到垄道，更新过垄【过程标记】标志位*/
void farmLimitUpdate(limitKey_t *key1) {
  	key1 -> key = 0;
		key1 -> keyflag = 0;
}

/* 更新传感器数据 */
void getSenorData(void) {
  getLimitStatus(limitStatus);
		/* 获取小激光数据 */
	chassisSenorData.leftSmallLazer = wt53r_distance[0];
		/* 获取大激光数据 */
	chassisSenorData.headBigLazer = tfmin_distance[1];
		/* 获取陀螺仪数据 */
	chassisSenorData.imuYaw = imu_data.yaw;
	chassisSenorData.imuWy 	= imu_data.wy;
  /* 当前角度状态的绝对状态 */
  chassisState.angleError = chassisSenorData.imuYaw - chassisState.targetAngle;
  if( fabs(chassisState.angleError) < CURRENT_ANGLE_ABS_DATA) {
    chassisState.currentAngle = chassisState.targetAngle;//更新当前角度状态
  }
		/* 获取光电开关数据 */
	chassisSenorData.leftHeadKey   	= limitStatus[L_Left_Head] ;
	chassisSenorData.rightHeadKey   = limitStatus[L_Right_Head];
	chassisSenorData.leftTailKey  	= limitStatus[L_Left_Tail];
	chassisSenorData.rightTailKey 	= limitStatus[L_Right_Tail];
	for(uint8_t i=0; i<4; i++) {
		/* 底盘电机速度环 */                                                                 
	chassisSenorData.speed_rpm[i] = moto_msg.chassis_2006[i].speed_rpm;                                                           
	}
}
/**
  初始化底盘FSM的状态变量
		* chassisCurrentState 底盘当前状态
		* chassisNextAction		底盘执行状态
		*	currentAngle				底盘当前角度
		* targetAngle					底盘下一时刻角度
*/
void chassisStateInit(void) {
		chassisCurrentState                 = __idle;	// 初始化状态为空闲
		chassisNextAction	                  = idle;	//初始化事件为加速
		chassisState.currentAngle           = 180;		//初始化当前角度为0
		chassisState.targetAngle            = 180;		//初始化目标角度为0
		chassisState.targedis               = mindis;		//初始化目标角度为0
		chassisState.chassisIdleState       = idleTurnToIdle;
		chassisState.chassisGostraightState = goStraightTurnToIdle;
    limitFindKey.key                    = 0;
    limitFindKey.keyflag                = 0;
}



void chassis_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();

    
    for (;;) {
      getSenorData();
			/*
				FSM流程：0. 当前状态初始化
			
								 获取函数指针 指向的状态函数
								 事件函数内部   通过条件判断    返回当前事件    或执行当前状态
								 事件    和     当前状态     合成的二维数组   确定了    唯一的下一个状态或维持当前状态 
			*/
			switch(ctrl_mode) {
        case PROTECT_MODE: {
          chassis.spd_input.vx = 0;
          chassis.spd_input.vy = 0;
          chassis.spd_input.vw = 0;
          memset(chassis.current_2006, 0, sizeof(chassis.current_2006));
          break;
        }
        case STANDARD_MODE: {
        #ifdef __Debug
          eventFunction		 		=  	afEventHandler[chassisCurrentState];				//将状态函数指针交给定义的临时指针
          chassisNextAction 	=  	eventFunction(&chassisState.currentAngle, &chassisState.targetAngle);	//执行当前状态并获取下一状态的
//          chassisCurrentState = 	lookUpTable[chassisCurrentState][chassisNextAction];//更新当前状态角标
        #endif
        #ifdef _NoDebug
          eventFunction		 		=  	afEventHandler[chassisCurrentState];				//将状态函数指针交给定义的临时指针
          chassisNextAction 	=  	eventFunction(&chassisState.currentAngle, &chassisState.targetAngle);	//执行当前状态并获取下一状态的
          chassisCurrentState = 	lookUpTable[chassisCurrentState][chassisNextAction];//更新当前状态角标
        #endif
          break;
        }
        case REMOTE_MODE:{
        #ifdef __ADPID
				chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);// 由于光电开关没有检测到障碍物，故没有进入垄道，不会贴墙
//          chassis.spd_input.vy = rc.ch4 * 10;
        chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
         #endif
        #ifdef __REMOTE
          chassis.spd_input.vx = rc.ch3 * 10;
          chassis.spd_input.vy = rc.ch4 * 10;
          chassis.spd_input.vw = rc.ch1 * 10;
         #endif
         
        }
      }
      mecanum_calc(chassis.spd_input.vx, chassis.spd_input.vy, chassis.spd_input.vw, chassis.wheel_spd_input);
      /* PID计算电机控制电流 */
      chassis_pid_calcu();
      memcpy(motor_cur.chassis_2006_cur, chassis.current_2006, sizeof(chassis.current_2006));
      osSignalSet(can_msg_send_task_t, CHASSIS_MOTOR_MSG_SEND);
			osDelayUntil(&thread_wake_time, 1);
    }
}


/*-------------------------------------------------------------------------------------------------------------------*/
/**
	*	状态函数实现 
	*	当前状态下的行为
	* 返回当前角度和目标角度
*/
chassisAction_e event_idle(float *currentAngle, float *targetAngle) {
	/* 根据传感器更新状态机 */
	if(chassisSenorData.leftHeadKey == 0 && chassisSenorData.leftTailKey == 0) { 				//使用光电反馈判断开机
		chassisState.chassisIdleState = idleTurnToStraight;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisIdleState) {
		case idleTurnToStraight: {
//      chassis.spd_input.vx = chassTrunAnyAngle(&chassisState.targedis);// 由于光电开关没有检测到障碍物，故没有进入垄道，不会贴墙
      gogogo();
      chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
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

chassisAction_e event_stop(float *currentAngle, float *targetAngle) {
	/* 根据传感器更新状态机 */
	if(	abs(chassisSenorData.speed_rpm[0]) >= miniSpm && 
			abs(chassisSenorData.speed_rpm[1]) >= miniSpm &&
			abs(chassisSenorData.speed_rpm[2]) >= miniSpm &&
			abs(chassisSenorData.speed_rpm[3]) >= miniSpm ) 
	{						//使用轮速反馈判断是否完全刹车
		chassisState.chassisStopState = stopTurnToStop;
	} else {		//使用轮速反馈判断是否完全刹车
		chassisState.chassisStopState = stopTurnToRotate;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisStopState) {
		case stopTurnToStraight: {
		  return speedUp;
		  break;
		}
		case stopTurnToStop: {
       stop(3000);
       chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);;
       chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
			return speedDown;
		}
		case stopTurnToRotate:{

      if(chassisState.currentAngle == 180)
          chassisState.targetAngle = 90;//停止后旋转
      if(chassisState.currentAngle == 90)
          chassisState.targetAngle = 0;//停止后旋转
      if(chassisState.currentAngle == 0)
          chassisState.targetAngle = 270;
      if(chassisState.currentAngle == 270)
          chassisState.targetAngle = 360;
      if(chassisState.currentAngle == 360)
          chassisState.targetAngle = 90;
      //旋转角度更新，不知道为什么原因这个函数不止运行了一次。
			return rotate;
			break;
		}
		case stopTurnToIdle:{
			return speedDown;
			break;
		}
	}
}

chassisAction_e event_goStraight(float *currentAngle, float *targetAngle) {
	/* 根据传感器更新状态机 */
	if(chassisSenorData.headBigLazer >=  miniBigLaserDis )  //大激光检测到前方还有很长一段距离
         //而且是肆无忌惮加速
  {      //使用大激光反馈判断是否加速
    if(*currentAngle == TOWARD_ANGLE1 || *currentAngle == TOWARD_ANGLE2 || *currentAngle == TOWARD_ANGLE3){  //不需要找垄道，直行加速
        farmLimitUpdate(&limitFindKey);//接下来要找垄道了
        chassisState.chassisGostraightState = goStraightTurnToStraight;
    }
    if(*currentAngle == FIND_ANGLE1 || *currentAngle == FIND_ANGLE2 || *currentAngle == FIND_ANGLE3) {    //需要找垄道，开启标志查找
      if(getFarmData(&chassisSenorData.rightHeadKey,&chassisSenorData.rightTailKey,&limitFindKey) == 0) {
        chassisState.chassisGostraightState = goStraightTurnToStraight;
      } else {
        if(*currentAngle == *targetAngle)
          chassisState.chassisGostraightState = goStraightTurnToStop;
      }
    }   
	} else {				                                                      //使用大激光反馈判断是否刹车
		if(*currentAngle == *targetAngle)
      chassisState.chassisGostraightState = goStraightTurnToStop;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisGostraightState) {
		case goStraightTurnToStraight: {
			if(chassisSenorData.leftHeadKey == 0 || chassisSenorData.leftTailKey == 0) { 				//使用光电反馈判断开机
				chassis.spd_input.vx = 0;// 由于光电开关没有检测到障碍物，故没有进入垄道，不会贴墙
			} else  {																																	//使用光电反馈判断是否处于不准开机的空闲模式
				chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);// 由于光电开关没有检测到障碍物，故没有进入垄道，不会贴墙
			}
			gogogo();
			chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
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

chassisAction_e event_rotate(float *currentAngle, float *targetAngle) {
	/* 根据传感器更新状态机 */
	if(*currentAngle != *targetAngle ) { 												//根据角度对比确定是否需要继续转弯
		chassisState.chassisRotateState = rotateTurnToRotate;
	} else {										//根据角度对比确定是否需要继续转弯
		chassisState.chassisRotateState = rotateTurnToStraight;
	}
	/* 根据状态机更新服务函数 并且返回 执行角标 */
	switch(chassisState.chassisRotateState) {
		case rotateTurnToStraight: {
			return speedUp; ;
			break;
		}
		case rotateTurnToStop: {
			return speedDown;
			break;
		}
		case rotateTurnToRotate:{
    	chassis.spd_input.vx = 0;
			chassis.spd_input.vy = 0;           
      chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.targetAngle);
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
    chassis.wheel_spd_ref[i] = chassis.wheel_spd_input[i];                                                                     //内环的目标值更新为  轮速的输入（转/分钟）
		chassis.wheel_spd_fdb[i] = moto_msg.chassis_2006[i].speed_rpm;                                                             //内环的反馈值更新为   电机转速  （转/封面椎间盘买个）
		chassis.current_2006[i]  = (int16_t)pid_calc(&pid_chassis_2006_spd[i], chassis.wheel_spd_fdb[i], chassis.wheel_spd_ref[i]);//传入PID进行计算

	}
}

float chassTrunAnyAngle(float *targetAngle) {
  static float middleOut = 0;
  //更新目标值
  chassis.chassis_angle_ref = *targetAngle;
  //更新反馈值
  chassis.chassis_angle_fdb = chassisSenorData.imuYaw;
  //环形误差处理
  chassis.chassis_angle_error = circle_error(&chassis.chassis_angle_ref, &chassis.chassis_angle_fdb,360);
  middleOut = -(int16_t)pid_calc(&pid_chassis_angle_pos, chassis.chassis_angle_fdb, chassis.chassis_angle_fdb + chassis.chassis_angle_error);
  return middleOut;
}

float chassStickToAnyDistance(float *dis) {
  static float middleOut = 0;
  //更新目标值
  chassis.chassis_stickdis_ref = *dis;
  //更新反馈值
  chassis.chassis_stickdis_fdb = chassisSenorData.leftSmallLazer;
  
  middleOut = (int16_t)pid_calc(&pid_chassis_stickdis_pos, chassis.chassis_stickdis_fdb, chassis.chassis_stickdis_ref );
  return middleOut;
}

float chassStopDistance(float *dis) {
  static float middleOut = 0;
  //更新目标值
  chassis.chassis_stop_dis_ref = *dis;
  //更新反馈值
  chassis.chassis_stop_dis_fdb = chassisSenorData.headBigLazer;
  
  middleOut = (int16_t)pid_calc(&pid_chassis_stop_pos, chassis.chassis_stop_dis_fdb, chassis.chassis_stop_dis_ref );
  return middleOut;
}

void stop(int step) {
  chassis.spd_input.vy -= step;
  if(chassis.spd_input.vy < 0) { chassis.spd_input.vy = 0; }
}

void gogogo(void) {
  chassis.spd_input.vy += 6;
  if(chassis.spd_input.vy > MAXSPEED) { chassis.spd_input.vy = MAXSPEED; }
}