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
/* ���洫�����ײ㴫�������� */
chassisSenor_t chassisSenorData;
/* �󼤹����̾���  ���� */
const uint16_t miniBigLaserDis = 300; 
/* ��ǰ�Ƕ� �� ��һʱ�̽Ƕ� */
int currentAngle, targetAngle;
/* ��ǰ״̬�Ǳ� */
chassisState_e chassisCurrentState;
/* �¼��Ǳ� */
chassisAction_e chassisNextAction;	
/* ���̸�����״̬�� */
chassisState_t chassisState;
/*----------------------------------------------------------------------------------------------------------------------*/
/**
	* @brief      ���������ʼ��
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

/* ���´��������� */
void getSenorData(void) {
		/* ��ȡС�������� */
	chassisSenorData.leftSmallLazer = wt53r_distance[0];
		/* ��ȡ�󼤹����� */
	chassisSenorData.headBigLazer = tfmin_distance[0];
		/* ��ȡ���������� */
	chassisSenorData.imuYaw = imu_data.yaw;
	chassisSenorData.imuWy 	= imu_data.wy;
		/* ��ȡ��翪������ */
	chassisSenorData.leftKey 	= limitStatus[0] ;
	chassisSenorData.rightKey = limitStatus[1];
	chassisSenorData.headKey 	= limitStatus[2];
	chassisSenorData.tailKey 	= limitStatus[3];
}

/**
  ��ʼ������FSM��״̬����
		* chassisCurrentState ���̵�ǰ״̬
		* chassisNextAction		����ִ��״̬
		*	currentAngle				���̵�ǰ�Ƕ�
		* targetAngle					������һʱ�̽Ƕ�
*/
void chassisStateInit(void) {
		chassisCurrentState = __idle;	// ��ʼ��״̬Ϊ����
		chassisNextAction	 = 	 idle;	//��ʼ���¼�Ϊ����
		currentAngle = 0;		//��ʼ����ǰ�Ƕ�Ϊ0
		targetAngle = 0;		//��ʼ��Ŀ��Ƕ�Ϊ0
		chassisState.chassisIdleState = idleTurnToIdle;
		chassisState.chassisGostraightState = goStraightTurnToIdle;
}
/**
  ���
  �ķ���ֵ������ָ��ָ��
*/
chassisState_e lookUpTable[4][4] = {
	[__stop] 				 = {	__goStraight, __stop, __rotate, 			__idle	},
	[__goStraight]	 = {	__goStraight, __stop, __idle, 				__idle	},
	[__rotate] 			 = {	__goStraight,	__idle, __idle, 				__idle	},
	[__idle] 				 = {	__goStraight,	__idle,	__idle,					__idle	},
};

/*-------------------------------------------------------------------------------------------------------------------*/
/**
	*	״̬����ʵ�� 
	*	��ǰ״̬�µ���Ϊ
	* ���ص�ǰ�ǶȺ�Ŀ��Ƕ�
*/
chassisAction_e event_idle(int *currentAngle, int *targetAngle) {
	/* ���ݴ���������״̬�� */
	if(chassisSenorData.leftKey == 0 && chassisSenorData.rightKey == 0) { 				//ʹ�ù�練���жϿ���
		chassisState.chassisIdleState = idleTurnToStraight;
	} else if (chassisSenorData.leftKey == 1 || chassisSenorData.rightKey == 1) {	//ʹ�ù�練���ж��Ƿ��ڲ�׼�����Ŀ���ģʽ
		chassisState.chassisIdleState = idleTurnToIdle;
	} else {																																			//ʹ�ù�練���ж��Ƿ��ڲ�׼�����Ŀ���ģʽ
		chassisState.chassisIdleState = idleTurnToIdle;
	}
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
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
	/* ���ݴ���������״̬�� */
	if(chassisSenorData.leftKey == 0 && chassisSenorData.rightKey == 0) {						//ʹ�����ٷ����ж��Ƿ���ȫɲ��
		chassisState.chassisStopState = stopTurnToStop;
	} else if (chassisSenorData.leftKey == 1 || chassisSenorData.rightKey == 1) {		//ʹ�����ٷ����ж��Ƿ���ȫɲ��
		chassisState.chassisStopState = stopTurnToRotate;
	} else {
		chassisState.chassisStopState = stopTurnToIdle;
	}
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
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
	/* ���ݴ���������״̬�� */
	if(chassisSenorData.headBigLazer > miniBigLaserDis) {									//ʹ�ô󼤹ⷴ���ж��Ƿ����
		chassisState.chassisGostraightState = goStraightTurnToStraight;
	} else if (chassisSenorData.headBigLazer <= miniBigLaserDis ) {				//ʹ�ô󼤹ⷴ���ж��Ƿ�ɲ��
		chassisState.chassisGostraightState = goStraightTurnToStop;
	} else {
		chassisState.chassisGostraightState = idleTurnToStraight;
	}
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
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
	/* ���ݴ���������״̬�� */
	if(*currentAngle != *targetAngle ) { 												//���ݽǶȶԱ�ȷ���Ƿ���Ҫ����ת��
		chassisState.chassisRotateState = rotateTurnToRotate;
	} else if (currentAngle ==targetAngle) {										//���ݽǶȶԱ�ȷ���Ƿ���Ҫ����ת��
		chassisState.chassisRotateState = rotateTurnToStraight;
	} else {
		chassisState.chassisRotateState = rotateTurnToStop;
	}
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
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
 	״̬����ָ������,ָ��ָ�����ĸ���������ִ��������Ϊ
*/
chassisAction_e (* afEventHandler[])(int *currentAngle, int *targetAngle) = { event_stop, event_goStraight, event_rotate,event_idle };

void chassis_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();

		chassisAction_e (* eventFunction)(int *currentAngle, int *targetAngle);
    
    for (;;) {
			/*
				FSM���̣�0. ��ǰ״̬��ʼ��
			
								 ��ȡ����ָ�� ָ���״̬����
								 �¼������ڲ�   ͨ�������ж�    ���ص�ǰ�¼�    ��ִ�е�ǰ״̬
								 �¼�    ��     ��ǰ״̬     �ϳɵĶ�ά����   ȷ����    Ψһ����һ��״̬��ά�ֵ�ǰ״̬ 
			*/
			switch(ctrl_mode) {
        case PROTECT_MODE: {
          memset(chassis.current_2006, 0, sizeof(chassis.current_2006));
          break;
        }
        case STANDARD_MODE: {
          eventFunction		 		=  	afEventHandler[chassisCurrentState];				//��״̬����ָ�뽻���������ʱָ��
          chassisNextAction 	=  	eventFunction(&currentAngle, &targetAngle);	//ִ�е�ǰ״̬����ȡ��һ״̬�ĽǱ�
          chassisCurrentState = 	lookUpTable[chassisCurrentState][chassisNextAction];//���µ�ǰ״̬�Ǳ�
           /* ���ֽ��� */
          mecanum_calc(chassis.spd_input.vx, chassis.spd_input.vy, chassis.spd_input.vw, chassis.wheel_spd_input);
          /* PID���������Ƶ��� */
          chassis_pid_calcu();
          break;
        }
      }
      memcpy(motor_cur.chassis_2006_cur, chassis.current_2006, sizeof(chassis.current_2006));
			osDelayUntil(&thread_wake_time, 1);
    }
}

/**
  * @brief ���ֽ��㺯��
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
  * @brief      ����PID
  * @author         
  * @param[in]  
  * @retval     
  */
void chassis_pid_calcu(void) {
	for(uint8_t i=0; i<4; i++) {
		/* ���̵���ٶȻ� */
		chassis.wheel_spd_fdb[i] = moto_msg.chassis_2006[i].speed_rpm;
		chassis.current_2006[i]  = (int16_t)pid_calc(&pid_chassis_3508_spd[i], chassis.wheel_spd_fdb[i], chassis.wheel_spd_ref[i]);

	}
}