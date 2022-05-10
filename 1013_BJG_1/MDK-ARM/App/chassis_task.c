#include "chassis_task.h"
#include "bsp_motor.h"
#include "pid.h"
#include "string.h"
#include "tim.h"
#include "bsp_gyro.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "bsp_sjg.h"
#include "bsp_bjg.h"
#include "Collect_Init.h"
#include "filter.h"
#include "bsp_switch.h"
#include "path_task.h"
#include "vision_task.h"
#include "bsp_openmv.h"
#define F 0
#define B 1

chassis_t chassis;								//���̽ṹ��
uint16_t wall_distance = 80; //ԭ��170
uint16_t wall_distance_aim = 120;
uint8_t Ctrl_Mode=1;							
uint8_t wall_flag=0;
uint32_t stop_time=0;							//��¼����ģʽ�ƶ�ͣ��ʱ��
float debug[2]={45,20.3f}; 
uint8_t test = 0;

void switch_mode(uint8_t Next_Mode) //�л�ģʽ�ҽ���ǰģʽ��ӡ����������
{
  Ctrl_Mode = Next_Mode;
  HAL_UART_Transmit(&huart1, (uint8_t *)&Ctrl_Mode, 1, 0xffff);
}


/*���̽ṹ��*/
chassis_t chassis;

void Chassis_Task(void)
{
	Ecd_Pid_Cal();			//λ�û�PID������С��Ŀǰ���ڵ�λ�ã��жϸ���ô���ߣ��������vx��vy��vw
	if(Ctrl_Mode <= 50)
	Speed_Pid_Cal();		//�ٶȻ�PID���Ǳ���ģʽ�½��ĸ����ֵ��ٶ�ֵ���ڵ�Ŀ���ٶ�
}

int test11 = 0;
/**
  * @brief λ��ʽPID����
  * @param input : void
  *        output: void
  * @note  ����С����ǰ����λ�ã��ж��˶�״̬�����������vx��vy��vw���ٽ�����ĸ����ӵ��ٶȡ�
  */
void Ecd_Pid_Cal(void)     //λ�û�PID����
{
#if 0
            if(vision_header == 0xFF) chassis.vy = 120;	
            if(vision_header == 0x2c)
            {
                if(x_temp<=40||x_temp>=60){	
                chassis.vy = pid_calc(&vision_aim_y,x_temp,50);//���¶���
                }
                if(x_temp>40&&x_temp<60)
                {   
                    if(chassis.vy>0)chassis.vy -= 1;;//��һ��ȥ��else if���,�����Ƿ����׼ȷ��׼����ͣ��
                }
            }
            if(JG[0] < 10 )	Ramp_Stop(0);  
#endif
//           if(chassis.vy <= (SPEED-5) && JG[0] >= Move_Disdance_big )		
//            chassis.vy += 8;					//б������
//			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);	
//			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
//			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0); 
//    
  #if 1
    switch(Ctrl_Mode)
	{
		case Protect_Mode:		//����ģʽ
			chassis.vx=0;	chassis.vy=0;	chassis.vw=0;
			mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
			bsp_Stop();
			stop_time++;
		break;
        case Y_Mode0:					//����ģʽ
			if(chassis.vy <= (SPEED-5) && JG[0] >= Move_Disdance_big )		
            chassis.vy += 8;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[0] <= Stop_Disdance_big0 )	Ramp_Stop(0);    //б��ͣ��  && chassis.vy >= 350
		break;
		case Y_Mode1:					//��ǰֱ��ģʽ
			if(chassis.vy <= (SPEED-5) && JG[0] >= Move_Disdance_big )		
            chassis.vy += 8;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0);    //б��ͣ��  && chassis.vy >= 350
		break;
		
		case Y_Mode2:					//���ֱ��ģʽ
			if(chassis.vy >= -(SPEED-5) && JG[3] >= Move_Disdance_big )		chassis.vy-=8 ;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)				  chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[3] <= Stop_Disdance_big  )	Ramp_Stop(1);	//б��ͣ��  && chassis.vy <= (-350)
		break;
			
		case Y_Mode3:					//��ǰ����ֱ��ģʽ	ת���ܷ�Ҫ��
			if(chassis.vy <= (Turn_Low_SPEED-5) )		chassis.vy = 320 ;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
//			if(JG[0] <= Stop_Disdance_big && Switch[F]==0 && Switch[B]==0)	Ramp1_Stop(0);				//б��ͣ��  chassis.vy >= 350
        /* �����Ƿ���ڵ���������ͣ��Ȼ��ת�䣬���ᣬ��˵��turn��-90���Դ�ͣ������ �������ᣬ����Ҫ��turn��-90��֮ǰ��һ��Ramp_Stop(0);*/
		break;
//			
		case Y_Mode4:					//������ֱ��ģʽ	ת���ܷ�Ҫ��
			if(chassis.vy >= -(Turn_Low_SPEED-5))		chassis.vy = -320;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)				  chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
//			if(JG[3] <= Stop_Disdance_big )	Ramp1_Stop(1);		//б��ͣ��
      /* �����Ƿ���ڵ��������ͣ��Ȼ��ת�䣬���ᣬ��˵��turn��-90���Դ�ͣ������ �������ᣬ����Ҫ��turn��-90��֮ǰ��һ��Ramp_Stop(1);*/
		break;
		case Turn_Mode:				//ת��ģʽ
			chassis.vx = 0;
			chassis.vy = 0;
			chassis.vw = pid_calc(&turn_pid,Angle_Error_WT,0);
		break;	
        
        case Enter_Mode:		//��¢ģʽ  
          if(chassis.vy <= (SPEED-5)&& JG[0] >= Move_Disdance_big)		
            chassis.vy += 8;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0);    //б��ͣ��  && chassis.vy >= 350
        break;
        case Exit_Mode:		//��¢ģʽ
          if(chassis.vy <= (SPEED-5)&& JG[0] >= Stop_Disdance_big)		
            chassis.vy += 8;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0);    //б��ͣ��  && chassis.vy >= 350
        break;    
        		
		case Vision_Mode1:   //�Ӿ���ǰ����ֱ��ģʽ
            if( chassis.vy <= (VISION_SPEED-8) && JG[3] >= Move_Disdance_big )		chassis.vy += 8;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	        chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[0] <=Stop_Disdance_big )	Ramp_Stop(0);	
            break;
    	case Vision_Mode2:   //�Ӿ�������ֱ��ģʽ
            if( chassis.vy >= -(VISION_SPEED-8) && JG[0] >= Move_Disdance_big )		chassis.vy -= 8;					//б������
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
			if(ABS(Angle_Error_WT)>=5)	        chassis.vx=0;									//ǰ�����ƫ����󣬳�ͷ���ˣ�x���򲻶�
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//��ǽ̫Զ������¢���¢��x���򲻶�
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//������ǰ�������ֵ���x������ٶ�
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(1);	
            break;                     
        case Vision_Aim_Mode://ʩ��ģʽ
            chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance_aim);
            chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);
            aim_yellow();
        default:
            chassis.vx=0;	chassis.vy=0;	chassis.vw=0;
			mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
			bsp_Stop();
		break;

    } 
      #endif
}

/**
  * @brief �ٶȻ�PID����
  * @param input : void
  *        output: void
  * @note  Ŀ�ģ���װ������������ֱ�ӵ��øú�����
  */
void Speed_Pid_Cal(void)				
{
	mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
	PID_CAL();
	mecanum_run();
}




/**
  * @brief б��ֹͣ����
  * @param input : way����ǰб��ֹͣor���б��ֹͣ��0Ϊ��ǰͣ����1Ϊ���ͣ��
  *        output: void
  * @note  �ڵ��������е��ã�����С��120cmʱ��ʼб�¼��١�
					
  */
void Ramp_Stop(uint8_t way)	
{
	if(way == 0)
	{
		if(chassis.vy >0)chassis.vy -= 8;
	}
	else if(way == 1)				
	{
		chassis.vy += 8;
	}
}
/**
  * @brief б����������
  * @param input : void
  *        output: void
  * @note  ����ʼ�������ǰ����
  */
void Ramp_Start(void)
{
	uint16_t i;
	uint16_t j;
	for(i=0;i<(SPEED/8);i++)
	{
		chassis.vy += 8;
		chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
		mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
		PID_CAL();
		mecanum_run();
		HAL_Delay(5);
	}
	for(j=0;j<100;j++)
	{
		chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//����������ƫ�����w���������ٶ�
		mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
		PID_CAL();
		mecanum_run();
		HAL_Delay(5);
	}
}
/**
  * @brief ���̳�ʼ����������ʼ����ʱ����PID�ṹ��
  * @param input : void
  *        output: void0.
0
00.-
  * @note  
  */
void Chassis_Init(void)
{
	HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL); //������ʱ��������ģʽ

	HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim13,TIM_CHANNEL_1);

	PID_struct_init(&motorBR_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);
	PID_struct_init(&motorBL_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);
	PID_struct_init(&motorFR_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);
	PID_struct_init(&motorFL_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);

	PID_struct_init(&wall_L_pid,POSITION_PID,500,100,4.0f,0.0f,2.0f);//p=3.5����Ӳ
	PID_struct_init(&wall_V_pid,POSITION_PID,400,100,2.2f,0.0f,0.0f);
	PID_struct_init(&straight_pid,POSITION_PID,PWM_LIMIT,8350,6.0f,0.0f,2.0f);
		
	PID_struct_init(&stop_pid,POSITION_PID,900,100,-1.5f,0.0f,-0.5f);
	PID_struct_init(&turn_pid,POSITION_PID,600,100,6.0f,0.0f,2.0f);
	PID_struct_init(&vision_aim_x,POSITION_PID,300,100,2.0f,0.001f,0.0f);
	PID_struct_init(&vision_aim_y,POSITION_PID,500,100,2.0f,0.0f,0.0f);
}
/**
  * @brief ���ֽ��㺯��
  * @param input : ?=+vx(mm/s)  ?=+vy(mm/s)  ccw=+vw(deg/s)
  *        output: every wheel speed(rpm)
  * @note  0=FL 1=FR 2=BL 3=BR
  */
void mecanum_calc(float vx, float vy, float vw, int16_t speed[])
{
  int16_t wheel_rpm[4];

    wheel_rpm[0] =    vx + vy - vw;      //��ת,ԭ��
    wheel_rpm[1] = 	 -vx + vy + vw;
    wheel_rpm[2] =   -vx + vy - vw;
    wheel_rpm[3] =    vx + vy + vw;	


  for (uint8_t i = 0; i < 4; i++)
  { 
    int taget_limit = SPEED;
    if(wheel_rpm[i]>=taget_limit)	wheel_rpm[i]=taget_limit;  //ԭ����1500
    if(wheel_rpm[i]<=-taget_limit)	wheel_rpm[i]=-taget_limit;
  }

	memcpy(speed, wheel_rpm, 4*sizeof(int16_t));
}

/**
  * @brief �趨���̵��pwm�ź�
  * @param input : void
  *        output: void
  * @note  Ŀ�ģ���װ����
  */
void mecanum_run(void)
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		set_pwm(i,chassis.pwm[i]);
	}
}

/**
  * @brief PID���㺯����������̵����pwm
  * @param input : void
  *        output: void
  * @note  Ŀ�ģ���װ����
  */
void PID_CAL(void)
{

	chassis.v[FR]=TIM3->CNT;
	TIM3->CNT=0;
	chassis.v[BR]=(TIM1->CNT);
	TIM1->CNT=0;
	chassis.v[BL]=-(TIM2->CNT);
	TIM2->CNT=0;
	chassis.v[FL]=-(TIM4->CNT);
	TIM4->CNT=0;   
	chassis.pwm[BR]=pid_calc(&motorBR_speed_pid,chassis.v[0],chassis.target_v[0]);
	chassis.pwm[BL]=pid_calc(&motorBL_speed_pid,chassis.v[1],chassis.target_v[1]);
	chassis.pwm[FR]=pid_calc(&motorFR_speed_pid,chassis.v[2],chassis.target_v[2]);
	chassis.pwm[FL]=pid_calc(&motorFL_speed_pid,chassis.v[3],chassis.target_v[3]);

}

float Ramp_Input(float input,float target,float ramp)
{
	if(input<target)
	{
		input+=ramp;
		if(input>target)	input=target;
	}
	else if(input>target)
	{
		input-=ramp;
		if(input<target)	input=target;
	}
	return input;
}

void Debug_Function(void){
     
    if(chassis.vy <= (SPEED-5)&& JG[0] >= 100)		//��Ҫ&&�󼤹����ݣ�ԭ��700  && tfmin_distance[1] >= 300
    chassis.vy += 8;					//б������
    chassis.vx =pid_calc(&wall_L_pid,JG[4],wall_distance);
    if(JG[0] <= 48)	Ramp_Stop(0);	
    Speed_Pid_Cal();
}





