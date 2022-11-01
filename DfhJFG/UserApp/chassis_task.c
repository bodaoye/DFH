#include "chassis_task.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "bsp_WT53R.h"
#include "bsp_TFMiniPlus.h"
#include "bsp_T_imu.h"
#include "remote_msg.h"
#include "pid.h"
#include "math_calcu.h"
#include "KalmanFilter.h"
#include "modeSwitch_task.h"
#include "comm_task.h"
//#define     __Debug
//#define   __ADPID
#define   _NoDebug
#define   __REMOTE
/**
  ������¢֮���ϸ�ڰѿأ�
  1. ��һ��¢��������
  2. ��ҵ��Ҫ���٣��漰�����پ���ĸı�
  3. ��¢Ҫֹͣ
*/
/* 1. ��һ���ռ�����  2. ȫ���ռ��� */
extern TaskHandle_t can_msg_send_task_t;
/*----------------------------------------------------------------------------------------------------------------------*/
chassis_t       chassis;
/* ���洫�����ײ㴫�������� */
chassisSenor_t chassisSenorData;
/* �󼤹����̾���  ���� *//* �ɱ�ļ��پ���  ����¢����ҵ����� */
uint16_t miniBigLaserDis     = 113;
int maxSpeed =  MAXSPEED;
/* ���¢������ */
uint16_t lastBigLaserDis = 12;
/* ɲ�������Ų��� */
int stepUp = 20,stepDown = 20;//12
/* Ѱ¢�ٶ� */
int speedFindField = 3000;
/* Ѱ¢�����Ա�־λ */
int findFlagTest =0;
/* ��һ��¢��֮ǰ�ı�־λ */
int first_90_flag = 0;
int stop_flag = 0;
const uint16_t miniSpm		   = 100;
uint16_t mindis        =  110;
/* ��ǰ״̬�Ǳ� */
chassisState_e chassisCurrentState;
/* �¼��Ǳ� */
chassisAction_e chassisNextAction;	
/* ���̸�����״̬�� */
chassisState_t chassisState;
/* ֹͣ��־λ */
stopKeyFlag_t stopKeyFlag;
visionCtr_t visionCtr;
/* ����״̬���ص����� */
chassisAction_e (* eventFunction)(float *currentAngle, float *targetAngle);


/**
  ���
  �ķ���ֵ������ָ��ָ��
*/
chassisState_e lookUpTable[4][4] = {
	[__stop] 				 = {	__goStraight,	 __stop, 	__rotate, 				__idle	},
	[__goStraight]	 = {	__goStraight,	 __stop, 	__idle, 				  __idle	},
	[__rotate] 			 = {	__goStraight,	 __idle,	__rotate, 				__idle	},
	[__idle] 				 = {	__goStraight,	 __idle,	__idle,					  __idle	},
};

/**
 	״̬����ָ������,ָ��ָ�����ĸ���������ִ��������Ϊ
*/
chassisAction_e (* afEventHandler[])(float *currentAngle, float *targetAngle) = { event_stop, event_goStraight, event_rotate,event_idle };

/*----------------------------------------------------------------------------------------------------------------------*/
/**
	* @brief      ���������ʼ��
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

  /* ���¹�¢��Ϣ
    ������ֹͣ��  CANSTOP 1
    ����ֹͣ��    CANTSTOP 0
  */
int getFarmData(uint8_t *Head, uint8_t *Tail,limitKey_t *key1) {
  if(*Head == 1) {
		key1 -> firstTrigger = 1;//��¼��һ�����ص�״̬
	}
  if(*Tail == 1) {
		key1 -> secondTrigger = 1;//��¼�ڶ������ص�״̬
	}
	/* ��������翪�ص�״̬�����һ������״̬ */ 
	if((*Head == 0 && *Tail == 0) && (key1 -> firstTrigger == 1) && (key1 -> secondTrigger == 1) ) { //�����ض�û�б��ڵ���û�о������ϰ�
		key1->keyflag = 1;///* ���Ѿ�ͨ�����״̬���м�¼ */��¼Ϊ�Ѿ�����
	}
  
  	/*�Գ�����Ŀ���״̬����ɨ�� */
	if(key1->keyflag == 1) {//����Ѿ��������ҳ��󿪹��ٴιرգ�ֹͣ��ǰ
		key1 -> key = 0;
		key1 -> keyflag = 1;
		return CANSTOP;//�л�����һ״̬
	}
	if(key1->keyflag == 0) {
		return CANTSTOP;
	} 
}

/*�ҵ�¢�������¹�¢�����̱�ǡ���־λ*/
void farmLimitUpdate(limitKey_t *key1) {
	key1 -> firstTrigger = 0;
	key1 -> secondTrigger = 0;
	key1 -> keyflag = 0;
}

/* ���´��������� */
void getSenorData(void) {
  
  getLimitStatus(limitStatus);
		/* ��ȡС�������� */
	chassisSenorData.leftSmallLazer = wt53r_distance[0];
		/* ��ȡ�󼤹����� */
	chassisSenorData.headBigLazer_init = tfmin_distance[1];
  chassisSenorData.headBigLazer_kal = Kalman1FilterCalc(&kalman_bigLazer,chassisSenorData.headBigLazer_init);
		/* ��ȡ���������� */
	chassisSenorData.imuYaw = imu_data.yaw;
	chassisSenorData.imuWy 	= imu_data.wy;
  /* ��ǰ�Ƕ�״̬�ľ���״̬ */
  chassisState.angleError = chassisSenorData.imuYaw - chassisState.targetAngle;
  if( fabs(chassisState.angleError) < CURRENT_ANGLE_ABS_DATA) {
    chassisState.currentAngle = chassisState.targetAngle;//���µ�ǰ�Ƕ�״̬
  }
		/* ��ȡ��翪������ */
	chassisSenorData.leftHeadKey   	= limitStatus[L_Left_Head] ;
	chassisSenorData.rightHeadKey   = limitStatus[L_Right_Head];
	chassisSenorData.leftTailKey  	= limitStatus[L_Left_Tail];
	chassisSenorData.rightTailKey 	= limitStatus[L_Right_Tail];
	for(uint8_t i=0; i<4; i++) {
		/* ���̵���ٶȻ� */                                                                 
	chassisSenorData.speed_rpm[i] = moto_msg.chassis_2006[i].speed_rpm;                                                           
	}
  //270�ᵼ�º���Ĺ�翪���󴥣�����С������ǽ����
  if( (chassisState.currentAngle == TOWARD_ANGLE2 && tfmin_distance[1]>=50) || (chassisState.currentAngle == TOWARD_ANGLE1 && tfmin_distance[1]>=50) ) {
    visionCtr.enableOpenALL = 1;
    mindis = 100;
  } else {
    mindis = 110;
    visionCtr.enableOpenALL = 0;
  }
  
  if(chassisState.currentAngle == FIND_ANGLE2)
    {
      if(visionCtr.enableOpenALL == 0) visionCtr.enableOpenALL = 0;
      if(visionCtr.enableOpenALL == 1) visionCtr.enableOpenALL = 0;
    }
  
  chassisState.targedis               = mindis;

}
/**
  ��ʼ������FSM��״̬����
		* chassisCurrentState ���̵�ǰ״̬
		* chassisNextAction		����ִ��״̬
		*	currentAngle				���̵�ǰ�Ƕ�
		* targetAngle					������һʱ�̽Ƕ�
*/
void chassisStateInit(void) {
		chassisCurrentState                 = __idle;	// ��ʼ��״̬Ϊ����
		chassisNextAction	                  = idle;	//��ʼ���¼�Ϊ����
		chassisState.currentAngle           = 180;		//��ʼ����ǰ�Ƕ�Ϊ0
		chassisState.targetAngle            = 180;		//��ʼ��Ŀ��Ƕ�Ϊ0
		chassisState.targedis               = mindis;		//��ʼ��Ŀ��Ƕ�Ϊ0
		chassisState.chassisIdleState       = idleTurnToIdle;
		chassisState.chassisGostraightState = goStraightTurnToIdle;
    limitFindKey.key                    = 0;
    limitFindKey.keyflag                = 0;
    stopKeyFlag.end = 0;
    visionCtr.openCollect1 = 0;//Ĭ������ռ�����
    visionCtr.enableOpenALL = 0;//Ĭ��ȫ������
}



void chassis_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();

    
    for (;;) {
      getSenorData();
			/*
				FSM���̣�0. ��ǰ״̬��ʼ��
			
								 ��ȡ����ָ�� ָ���״̬����
								 �¼������ڲ�   ͨ�������ж�    ���ص�ǰ�¼�    ��ִ�е�ǰ״̬
								 �¼�    ��     ��ǰ״̬     �ϳɵĶ�ά����   ȷ����    Ψһ����һ��״̬��ά�ֵ�ǰ״̬ 
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
//        chassisSenorData.headBigLazer_kal = Kalman1FilterCalc(&kalman_bigLazer,chassisSenorData.headBigLazer_init);//�󼤹������˲�
        #ifdef __Debug
          eventFunction		 		=  	afEventHandler[chassisCurrentState];				//��״̬����ָ�뽻���������ʱָ��
          chassisNextAction 	=  	eventFunction(&chassisState.currentAngle, &chassisState.targetAngle);	//ִ�е�ǰ״̬����ȡ��һ״̬��
//          chassisCurrentState = 	lookUpTable[chassisCurrentState][chassisNextAction];//���µ�ǰ״̬�Ǳ�
        #endif
        #ifdef _NoDebug
          eventFunction		 		=  	afEventHandler[chassisCurrentState];				//��״̬����ָ�뽻���������ʱָ��
          chassisNextAction 	=  	eventFunction(&chassisState.currentAngle, &chassisState.targetAngle);	//ִ�е�ǰ״̬����ȡ��һ״̬��
          chassisCurrentState = 	lookUpTable[chassisCurrentState][chassisNextAction];//���µ�ǰ״̬�Ǳ�
        #endif
          break;
        }
        case REMOTE_MODE:{
        #ifdef __ADPID
				chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);// ���ڹ�翪��û�м�⵽�ϰ����û�н���¢����������ǽ
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
      /* PID���������Ƶ��� */
      chassis_pid_calcu();
      memcpy(motor_cur.chassis_2006_cur, chassis.current_2006, sizeof(chassis.current_2006));
      osSignalSet(can_msg_send_task_t, CHASSIS_MOTOR_MSG_SEND);
			osDelayUntil(&thread_wake_time, 1);
    }
}


/*-------------------------------------------------------------------------------------------------------------------*/
/**
	*	״̬����ʵ�� 
	*	��ǰ״̬�µ���Ϊ
	* ���ص�ǰ�ǶȺ�Ŀ��Ƕ�
*/
chassisAction_e event_idle(float *currentAngle, float *targetAngle) {
	/* ���ݴ���������״̬�� */
	if(chassisSenorData.leftHeadKey == 0 && chassisSenorData.leftTailKey == 0) { 				//ʹ�ù�練���жϿ���
		chassisState.chassisIdleState = idleTurnToStraight;
	}
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
	switch(chassisState.chassisIdleState) {
		case idleTurnToStraight: {
//      chassis.spd_input.vx = chassTrunAnyAngle(&chassisState.targedis);// ���ڹ�翪��û�м�⵽�ϰ����û�н���¢����������ǽ
      gogogo(stepUp);
//    chassis.spd_input.vy = MAXSPEED;
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
	/* ���ݴ���������״̬�� */
	if( ifStop() == 0) 
	{						//ʹ�����ٷ����ж��Ƿ���ȫɲ��
		chassisState.chassisStopState = stopTurnToStop;
	} else {		//ʹ�����ٷ����ж��Ƿ���ȫɲ��
		chassisState.chassisStopState = stopTurnToRotate;
	}
  if(stopKeyFlag.end == 1) chassisState.chassisStopState = stopTurnToStop;;
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
	switch(chassisState.chassisStopState) {
		case stopTurnToStraight: {
		  return speedUp;
		  break;
	}
		case stopTurnToStop: {
    
     stop(stepDown);
      
       if(chassisSenorData.leftSmallLazer<=mindis) {
          if(chassisState.currentAngle == 90 || chassisState.currentAngle == 270 || chassisState.currentAngle == 360)
             chassis.spd_input.vx = 0; 
          else chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);
       } else {
        chassis.spd_input.vx = 0;
       }
       if(stopKeyFlag.end == 1) chassis.spd_input.vx = 0;
        chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
			return speedDown;
		}
		case stopTurnToRotate:{
      chassis.spd_input.vx = 0;//����
      if(chassisState.currentAngle == 180)
          chassisState.targetAngle = 90;//ֹͣ����ת
      if(chassisState.currentAngle == 90)
          chassisState.targetAngle = 0;//ֹͣ����ת
      if(chassisState.currentAngle == 0)
          chassisState.targetAngle = 270;
      if(chassisState.currentAngle == 270)
          chassisState.targetAngle = 360;
      if(chassisState.currentAngle == 360)
          chassisState.targetAngle = 90;
      //��ת�Ƕȸ��£���֪��Ϊʲôԭ�����������ֹ������һ�Ρ�
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
/* ֱ�����漰��ϸ�ڣ�
  1. ��¢������
 */
 if((*currentAngle == TOWARD_ANGLE1)  || *currentAngle == TOWARD_ANGLE2) {//Ѱ��¢������
  maxSpeed = 6000;
  miniBigLaserDis = 41;
 } else {
  maxSpeed = MAXSPEED;
  miniBigLaserDis = 113;
 }
	/* ���ݴ���������״̬�� */
	if(chassisSenorData.headBigLazer_kal >=  miniBigLaserDis && *currentAngle != FIND_ANGLE1) { //�󼤹��⵽ǰ�����кܳ�һ�ξ���
    if(*currentAngle == TOWARD_ANGLE1 || *currentAngle == TOWARD_ANGLE2 || *currentAngle == TOWARD_ANGLE3){  //��ǰ����Ҫ��¢����ֱ�м���
        farmLimitUpdate(&limitFindKey);//������Ҫ��¢����
        chassisState.chassisGostraightState = goStraightTurnToStraightP;//����ֱ��
    }
    
    
   if(*currentAngle == FIND_ANGLE2) {    //��ǰ�Ƕ���Ҫ��¢����������־����
        findFlagTest = getFarmData(&chassisSenorData.leftHeadKey,&chassisSenorData.leftTailKey,&limitFindKey);
      if(getFarmData(&chassisSenorData.leftHeadKey,&chassisSenorData.leftTailKey,&limitFindKey) == 0) {
        chassisState.chassisGostraightState = goStraightTurnToStraightN;//����ֱ��
      } else {                            //�ҵ�¢��ɲ��
        if(*currentAngle == *targetAngle)
          chassisState.chassisGostraightState = goStraightTurnToStop;
      }
      if(chassisSenorData.rightHeadKey == 0) {
        stopKeyFlag.end = 1;
        if(chassisSenorData.rightTailKey == 0)
          chassisState.chassisGostraightState = goStraightTurnToStop;
     }
  }


  
  } else if(*currentAngle == FIND_ANGLE1) {//ʹ�ô󼤹ⷴ���ж��Ƿ�ɲ��
     if(chassisSenorData.rightHeadKey == 0 && chassisSenorData.rightTailKey == 0 && chassisSenorData.leftHeadKey == 0 && chassisSenorData.leftTailKey == 0) {
      stopKeyFlag.end = 1;
      chassisState.chassisGostraightState = goStraightTurnToStop;
    }
    first_90_flag = 1;//��ʾ��һ��90��¢������
    visionCtr.openCollect1 = 1;//�����Ӿ�����չ����ɫ2�ĸ���
    if(chassisSenorData.headBigLazer_kal >= lastBigLaserDis && *currentAngle == FIND_ANGLE1) {   //������С�������ƣ���ֹ�޷��߳����һ��¢��
      if(getFarmData(&chassisSenorData.rightHeadKey,&chassisSenorData.rightTailKey,&limitFindKey) == 0) { //��ǰ�Ƕ���Ҫ��¢����������־����
          chassisState.chassisGostraightState = goStraightTurnToStraightN;//����ֱ��
      } else {       //�ҵ�¢��ɲ��
          if(*currentAngle == *targetAngle)
            chassisState.chassisGostraightState = goStraightTurnToStop;
      }  
    } else {
      chassisState.chassisGostraightState = goStraightTurnToStop;
    }

  }


  else {
      chassisState.chassisGostraightState = goStraightTurnToStop;    
  }
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
	switch(chassisState.chassisGostraightState) {
		case goStraightTurnToStraightP: { //¢������ֱ��
			if(chassisSenorData.leftHeadKey == 0 || chassisSenorData.leftTailKey == 0) { 				//ʹ�ù�練���жϿ���
				chassis.spd_input.vx = 0;// ���ڹ�翪��û�м�⵽�ϰ����û�н���¢����������ǽ
			} else  {																																	//ʹ�ù�練���ж��Ƿ��ڲ�׼�����Ŀ���ģʽ
				chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);// ���ڹ�翪��û�м�⵽�ϰ����û�н���¢����������ǽ
			}
			gogogo(stepUp);
			chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
			return speedUp;
			break;
		}
    case goStraightTurnToStraightN: {//¢������ֱ��				
   
			if(chassisSenorData.leftHeadKey == 0 || chassisSenorData.leftTailKey == 0) { 				//ʹ�ù���ж��Ƿ�������ǽ
				chassis.spd_input.vx = 0;// ���ڹ�翪��û�м�⵽�ϰ����û�н���¢����������ǽ
			} else  {																																	//ʹ�ù�練���ж��Ƿ��ڲ�׼�����Ŀ���ģʽ
				chassis.spd_input.vx = chassStickToAnyDistance(&chassisState.targedis);// ���ڹ�翪��û�м�⵽�ϰ����û�н���¢����������ǽ
			}
			chassis.spd_input.vy = speedFindField;
			chassis.spd_input.vw = chassTrunAnyAngle(&chassisState.currentAngle);
    if(chassisSenorData.rightHeadKey == 0 && chassisSenorData.rightTailKey == 0 && chassisSenorData.leftHeadKey == 0 && chassisSenorData.leftTailKey == 0) {
      chassis.spd_input.vx = 0;
      chassis.spd_input.vy = 0;
      chassis.spd_input.vw = 0;
      return speedDown;
    }
			return speedUp;
			break;   
    }
		case goStraightTurnToStop: {
    
			return speedDown;
			break;
		}
		case goStraightTurnToRotate:{
      chassis.spd_input.vx = 0;//����
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
	/* ���ݴ���������״̬�� */
	if(*currentAngle != *targetAngle ) { 												//���ݽǶȶԱ�ȷ���Ƿ���Ҫ����ת��
		chassisState.chassisRotateState = rotateTurnToRotate;
	} else {										//���ݽǶȶԱ�ȷ���Ƿ���Ҫ����ת��
		chassisState.chassisRotateState = rotateTurnToStraight;
	}
	/* ����״̬�����·����� ���ҷ��� ִ�нǱ� */
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
    chassis.wheel_spd_ref[i] = chassis.wheel_spd_input[i];                                                                     //�ڻ���Ŀ��ֵ����Ϊ  ���ٵ����루ת/���ӣ�
		chassis.wheel_spd_fdb[i] = moto_msg.chassis_2006[i].speed_rpm;                                                             //�ڻ��ķ���ֵ����Ϊ   ���ת��  ��ת/����׵���������
		chassis.current_2006[i]  = (int16_t)pid_calc(&pid_chassis_2006_spd[i], chassis.wheel_spd_fdb[i], chassis.wheel_spd_ref[i]);//����PID���м���

	}
}

float chassTrunAnyAngle(float *targetAngle) {
  static float middleOut = 0;
  //����Ŀ��ֵ
  chassis.chassis_angle_ref = *targetAngle;
  //���·���ֵ
  chassis.chassis_angle_fdb = chassisSenorData.imuYaw;
  //��������
  chassis.chassis_angle_error = circle_error(&chassis.chassis_angle_ref, &chassis.chassis_angle_fdb,360);
  middleOut = -(int16_t)pid_calc(&pid_chassis_angle_pos, chassis.chassis_angle_fdb, chassis.chassis_angle_fdb + chassis.chassis_angle_error);
  return middleOut;
}

float chassStickToAnyDistance(float *dis) {
  static float middleOut = 0;
  //����Ŀ��ֵ
  chassis.chassis_stickdis_ref = *dis;
  //���·���ֵ
  chassis.chassis_stickdis_fdb = chassisSenorData.leftSmallLazer;
  
  middleOut = (int16_t)pid_calc(&pid_chassis_stickdis_pos, chassis.chassis_stickdis_fdb, chassis.chassis_stickdis_ref );
  return middleOut;
}

float chassStopDistance(float *dis) {
  static float middleOut = 0;
  //����Ŀ��ֵ
  chassis.chassis_stop_dis_ref = *dis;
  //���·���ֵ
  chassis.chassis_stop_dis_fdb = chassisSenorData.headBigLazer_kal;
  
  middleOut = (int16_t)pid_calc(&pid_chassis_stop_pos, chassis.chassis_stop_dis_fdb, chassis.chassis_stop_dis_ref );
  return middleOut;
}

void stop(int step) {
  chassis.spd_input.vy-=6;
  if(chassis.spd_input.vy < 0) { chassis.spd_input.vy = 0; }
}

void gogogo(int step) {
  chassis.spd_input.vy = chassis.spd_input.vy + step;
  if(chassis.spd_input.vy > maxSpeed) { chassis.spd_input.vy = maxSpeed; }
}

int ifStop(void) {

  static int stop_flag = 0;
  static int num = 0;
      num = abs(chassisSenorData.speed_rpm[0])+ 
			abs(chassisSenorData.speed_rpm[1]) +
			abs(chassisSenorData.speed_rpm[2]) +
			abs(chassisSenorData.speed_rpm[3]);
   if(num>miniSpm) return 0;
   else return 1;
}