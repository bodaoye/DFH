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

chassis_t chassis;								//底盘结构体
uint16_t wall_distance = 80; //原来170
uint16_t wall_distance_aim = 120;
uint8_t Ctrl_Mode=1;							
uint8_t wall_flag=0;
uint32_t stop_time=0;							//记录保护模式制动停车时间
float debug[2]={45,20.3f}; 
uint8_t test = 0;

void switch_mode(uint8_t Next_Mode) //切换模式且将当前模式打印到串口助手
{
  Ctrl_Mode = Next_Mode;
  HAL_UART_Transmit(&huart1, (uint8_t *)&Ctrl_Mode, 1, 0xffff);
}


/*底盘结构体*/
chassis_t chassis;

void Chassis_Task(void)
{
	Ecd_Pid_Cal();			//位置环PID，根据小车目前所在的位置，判断该怎么行走，算出整车vx、vy、vw
	if(Ctrl_Mode <= 50)
	Speed_Pid_Cal();		//速度环PID，非保护模式下将四个麦轮的速度值调节到目标速度
}

int test11 = 0;
/**
  * @brief 位置式PID计算
  * @param input : void
  *        output: void
  * @note  根据小车当前所在位置，判断运动状态，给出车身的vx、vy、vw，再解算出四个轮子的速度。
  */
void Ecd_Pid_Cal(void)     //位置环PID计算
{
#if 0
            if(vision_header == 0xFF) chassis.vy = 120;	
            if(vision_header == 0x2c)
            {
                if(x_temp<=40||x_temp>=60){	
                chassis.vy = pid_calc(&vision_aim_y,x_temp,50);//看下多少
                }
                if(x_temp>40&&x_temp<60)
                {   
                    if(chassis.vy>0)chassis.vy -= 1;;//试一下去掉else if这句,看看是否可以准确对准黄树停车
                }
            }
            if(JG[0] < 10 )	Ramp_Stop(0);  
#endif
//           if(chassis.vy <= (SPEED-5) && JG[0] >= Move_Disdance_big )		
//            chassis.vy += 8;					//斜坡启动
//			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);	
//			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
//			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0); 
//    
  #if 1
    switch(Ctrl_Mode)
	{
		case Protect_Mode:		//保护模式
			chassis.vx=0;	chassis.vy=0;	chassis.vw=0;
			mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
			bsp_Stop();
			stop_time++;
		break;
        case Y_Mode0:					//开局模式
			if(chassis.vy <= (SPEED-5) && JG[0] >= Move_Disdance_big )		
            chassis.vy += 8;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[0] <= Stop_Disdance_big0 )	Ramp_Stop(0);    //斜坡停车  && chassis.vy >= 350
		break;
		case Y_Mode1:					//向前直行模式
			if(chassis.vy <= (SPEED-5) && JG[0] >= Move_Disdance_big )		
            chassis.vy += 8;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0);    //斜坡停车  && chassis.vy >= 350
		break;
		
		case Y_Mode2:					//向后直行模式
			if(chassis.vy >= -(SPEED-5) && JG[3] >= Move_Disdance_big )		chassis.vy-=8 ;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)				  chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[3] <= Stop_Disdance_big  )	Ramp_Stop(1);	//斜坡停车  && chassis.vy <= (-350)
		break;
			
		case Y_Mode3:					//向前低速直行模式	转弯跑法要用
			if(chassis.vy <= (Turn_Low_SPEED-5) )		chassis.vy = 320 ;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
//			if(JG[0] <= Stop_Disdance_big && Switch[F]==0 && Switch[B]==0)	Ramp1_Stop(0);				//斜坡停车  chassis.vy >= 350
        /* 看看是否会在第三点稳稳停下然后转弯，若会，则说明turn（-90）自带停车功能 ，若不会，则需要在turn（-90）之前加一句Ramp_Stop(0);*/
		break;
//			
		case Y_Mode4:					//向后低速直行模式	转弯跑法要用
			if(chassis.vy >= -(Turn_Low_SPEED-5))		chassis.vy = -320;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)				  chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
//			if(JG[3] <= Stop_Disdance_big )	Ramp1_Stop(1);		//斜坡停车
      /* 看看是否会在第五点稳稳停下然后转弯，若会，则说明turn（-90）自带停车功能 ，若不会，则需要在turn（-90）之前加一句Ramp_Stop(1);*/
		break;
		case Turn_Mode:				//转弯模式
			chassis.vx = 0;
			chassis.vy = 0;
			chassis.vw = pid_calc(&turn_pid,Angle_Error_WT,0);
		break;	
        
        case Enter_Mode:		//进垄模式  
          if(chassis.vy <= (SPEED-5)&& JG[0] >= Move_Disdance_big)		
            chassis.vy += 8;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0);    //斜坡停车  && chassis.vy >= 350
        break;
        case Exit_Mode:		//出垄模式
          if(chassis.vy <= (SPEED-5)&& JG[0] >= Stop_Disdance_big)		
            chassis.vy += 8;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(0);    //斜坡停车  && chassis.vy >= 350
        break;    
        		
		case Vision_Mode1:   //视觉向前低速直行模式
            if( chassis.vy <= (VISION_SPEED-8) && JG[3] >= Move_Disdance_big )		chassis.vy += 8;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	        chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[0] <=Stop_Disdance_big )	Ramp_Stop(0);	
            break;
    	case Vision_Mode2:   //视觉向后低速直行模式
            if( chassis.vy >= -(VISION_SPEED-8) && JG[0] >= Move_Disdance_big )		chassis.vy -= 8;					//斜坡启动
			chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
			if(ABS(Angle_Error_WT)>=5)	        chassis.vx=0;									//前后距离偏差过大，车头歪了，x方向不动
			else if(JG[4]>=Move_Disdance_small)					chassis.vx=0;									//离墙太远，在入垄或出垄，x方向不动
			else if(wall_flag==0)
			chassis.vx = pid_calc(&wall_L_pid,JG[4],wall_distance);		//根据左前激光距离值算出x方向的速度
			if(JG[0] <= Stop_Disdance_big )	Ramp_Stop(1);	
            break;                     
        case Vision_Aim_Mode://施肥模式
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
  * @brief 速度环PID计算
  * @param input : void
  *        output: void
  * @note  目的：封装函数，任务中直接调用该函数。
  */
void Speed_Pid_Cal(void)				
{
	mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
	PID_CAL();
	mecanum_run();
}




/**
  * @brief 斜坡停止函数
  * @param input : way，向前斜坡停止or向后斜坡停止，0为向前停车，1为向后停车
  *        output: void
  * @note  在底盘任务中调用，距离小于120cm时开始斜坡减速。
					
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
  * @brief 斜坡启动函数
  * @param input : void
  *        output: void
  * @note  程序开始任务调度前调用
  */
void Ramp_Start(void)
{
	uint16_t i;
	uint16_t j;
	for(i=0;i<(SPEED/8);i++)
	{
		chassis.vy += 8;
		chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
		mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
		PID_CAL();
		mecanum_run();
		HAL_Delay(5);
	}
	for(j=0;j<100;j++)
	{
		chassis.vw = pid_calc(&straight_pid,Angle_Error_WT,0);		//根据陀螺仪偏差算出w自旋方向速度
		mecanum_calc(chassis.vx,chassis.vy,chassis.vw,chassis.target_v);
		PID_CAL();
		mecanum_run();
		HAL_Delay(5);
	}
}
/**
  * @brief 底盘初始化函数，初始化定时器、PID结构体
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
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL); //开启定时器编码器模式

	HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim13,TIM_CHANNEL_1);

	PID_struct_init(&motorBR_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);
	PID_struct_init(&motorBL_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);
	PID_struct_init(&motorFR_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);
	PID_struct_init(&motorFL_speed_pid,POSITION_PID,PWM_LIMIT,8350,55,1.4f,0.0f);

	PID_struct_init(&wall_L_pid,POSITION_PID,500,100,4.0f,0.0f,2.0f);//p=3.5不够硬
	PID_struct_init(&wall_V_pid,POSITION_PID,400,100,2.2f,0.0f,0.0f);
	PID_struct_init(&straight_pid,POSITION_PID,PWM_LIMIT,8350,6.0f,0.0f,2.0f);
		
	PID_struct_init(&stop_pid,POSITION_PID,900,100,-1.5f,0.0f,-0.5f);
	PID_struct_init(&turn_pid,POSITION_PID,600,100,6.0f,0.0f,2.0f);
	PID_struct_init(&vision_aim_x,POSITION_PID,300,100,2.0f,0.001f,0.0f);
	PID_struct_init(&vision_aim_y,POSITION_PID,500,100,2.0f,0.0f,0.0f);
}
/**
  * @brief 麦轮解算函数
  * @param input : ?=+vx(mm/s)  ?=+vy(mm/s)  ccw=+vw(deg/s)
  *        output: every wheel speed(rpm)
  * @note  0=FL 1=FR 2=BL 3=BR
  */
void mecanum_calc(float vx, float vy, float vw, int16_t speed[])
{
  int16_t wheel_rpm[4];

    wheel_rpm[0] =    vx + vy - vw;      //旋转,原本
    wheel_rpm[1] = 	 -vx + vy + vw;
    wheel_rpm[2] =   -vx + vy - vw;
    wheel_rpm[3] =    vx + vy + vw;	


  for (uint8_t i = 0; i < 4; i++)
  { 
    int taget_limit = SPEED;
    if(wheel_rpm[i]>=taget_limit)	wheel_rpm[i]=taget_limit;  //原本是1500
    if(wheel_rpm[i]<=-taget_limit)	wheel_rpm[i]=-taget_limit;
  }

	memcpy(speed, wheel_rpm, 4*sizeof(int16_t));
}

/**
  * @brief 设定底盘电机pwm信号
  * @param input : void
  *        output: void
  * @note  目的：封装函数
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
  * @brief PID计算函数，计算底盘电机的pwm
  * @param input : void
  *        output: void
  * @note  目的：封装函数
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
     
    if(chassis.vy <= (SPEED-5)&& JG[0] >= 100)		//还要&&大激光数据，原本700  && tfmin_distance[1] >= 300
    chassis.vy += 8;					//斜坡启动
    chassis.vx =pid_calc(&wall_L_pid,JG[4],wall_distance);
    if(JG[0] <= 48)	Ramp_Stop(0);	
    Speed_Pid_Cal();
}





