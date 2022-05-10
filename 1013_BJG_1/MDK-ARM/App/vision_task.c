#include "vision_task.h"
#include "usart.h"
#include "tim.h"
#include "cmsis_os.h"
#include "chassis_task.h"
#include "path_task.h"
#include "bsp_sjg.h"
#include "pid.h"
#include "string.h"
#include "bsp_servo.h"
#include "bsp_openmv.h"

#define AIM_TIME		1200		//对位时间


gimbal_t gimbal;
vision_t vision;
uint8_t last_ctrl_mode;
uint8_t  gimbal_status = 0;
//施肥状态位     0为收回状态    1为视觉识别到黄树   2为对准黄树停车、准备施肥   3为舵机伸缩施肥完毕
uint8_t  grass_total = 0;	 //记录已施肥数目,共10个

void Vision_Task_Init(void)
{
    servo_init();                                                                       //开启定时器5的PWM
    HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);		                                //云台电机
	PID_struct_init(&gimbal_speed_pid,POSITION_PID,10000,10000,55.0f,1.4f,0.0f);		//云台电机速度环PID
    PID_struct_init(&gimbal_position_pid,POSITION_PID,10000,10000,0.05f,0.0f,0.0f);		//云台电机位置环PID     
}
/**
  * @brief 视觉任务函数
  * @param input : void
  *        output: void
  * @note  根据openmv传回不同的帧头数据去执行不同的任务，0x2c代表黄树，0xfe代表绿树，0xff代表无识别到树
  */
int test_s = 0;
void Vision_Task(void)
{
    #if 0
    if((Ctrl_Mode==Vision_Mode1 || Ctrl_Mode==Vision_Mode2) && gimbal_status==0 && vision_header == 0x2c && grass_total < 10)	
    {
        gimbal_status = 1;//识别到黄树，需要施肥
        last_ctrl_mode = Ctrl_Mode;//保存上一次模式
        switch_mode(Vision_Aim_Mode);//切换为施肥模式
    }
    else if(gimbal_status==1 && chassis.vx==0 && chassis.vy==0){//需要实际验证是否车子真的停下并对准黄树了
        gimbal_status = 2;//已经对准黄树
    }
    else if(gimbal_status == 2){
        Servo_Work();//舵机工作
        grass_total++;//已施肥数目
        gimbal_status = 3;//施肥完毕
    }
    else if(gimbal_status == 3){
        switch_mode(last_ctrl_mode);//切换为原模式
        gimbal_status = 0;
    }
    
    if(vision_header == 0xFE)//识别到绿树,亮灯
    {
        //无需停车,亮绿灯1秒
    }
    #endif
//    if(vision_header == 0x2c)
//    {
//         HAL_GPIO_Writ 			          //反转
//        HAL_Delay(110);
//        Gimbal_Stop();
//        HAL_Delay(600);
        
        HAL_GPIO_WritePin(IN41_GPIO_Port,IN41_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(IN42_GPIO_Port,IN42_Pin,GPIO_PIN_SET);	
        TIM14->CCR1=9000;					//伸出	
        HAL_Delay(135);
        Gimbal_Stop();    
        HAL_Delay(450);
        
        servo_set(85);//85伸出 45缩回
        HAL_Delay(450);//具体时间需要等施肥结构改版后测试 300太快
        servo_set(45);
        HAL_Delay(550);
        
        HAL_GPIO_WritePin(IN41_GPIO_Port,IN41_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(IN42_GPIO_Port,IN42_Pin,GPIO_PIN_RESET);	
        TIM14->CCR1=9000;						//收回
        HAL_Delay(100);
        Gimbal_Stop();


       
//             set_gimbal_pwm(4,9000);
        HAL_Delay(90000);
//    }
}
/**
  * @brief 对准施肥树苗函数
  * @param input : void
  *        output: void
  * @note  目的:封装函数
           在视觉任务中调用，使车子停在合适的施肥位置。
  */
void aim_yellow(void)
{
    if(x_temp<=40||x_temp>=60)
    {
        chassis.vy = pid_calc(&vision_aim_y,x_temp,50);
    }
    else if(x_temp>40&&x_temp<60)
    {   
        if(chassis.vy > 0) chassis.vy -= 1;//试一下去掉else if这句,看看是否可以准确对准黄树停车
    }
}

/**
  * @brief 云台停止函数
  * @param input : void
  *        output: void
  * @note  在云台电机伸缩任务中调用，施肥结构到达理想位置便调用，使其停在合适位置。
  */
void Gimbal_Stop(void)
{  
	gimbal.pwm = 0;
	TIM14->CCR1= 0;
}



/**
  * @brief 舵机工作函数
  * @param input : void
  *        output: void
  * @note  识别到黄树时调用
  */
void Servo_Work(void)
{
/*舵机控制施肥结构*/    
    servo_set(85);//85伸出 45缩回
    HAL_Delay(450);//具体时间需要等施肥结构改版后测试 300太快
    servo_set(45);
}

/**
  * @brief 云台电机伸缩函数
  * @param input : way, 向右伸出或向左收回, 0为向右伸出, 1为向左收回
  *        output: void
  * @note  识别到黄树时调用
  */

void Gimbal_Work(uint8_t way)
{
    if(way == 0)//电机伸出
	{
		set_gimbal_pwm(4,-9000);
        HAL_Delay(115);
        Gimbal_Stop();
//        HAL_Delay(100);//看看需不需要,如果不需要可以删掉这句
    
	}
	else if(way == 1)//电机收回	
	{
		set_gimbal_pwm(4,9000);
        HAL_Delay(90);
        Gimbal_Stop();
//        HAL_Delay(100);//看看需不需要,如果不需要可以删掉这句
	}

}
/**
  * @brief 云台串级PID函数
  * @param input : target,根据伸出距离需要多少,传入对应位置环PID的target值
  *        output: void
  * @note  目的:封装函数
           在云台电机伸缩任务中调用
  */
void Gimbal_Cascade_PID(int16_t target)
{
    gimbal.v = TIM8->CNT;
    if(gimbal.v  > 35000) gimbal.v -=65535; 
    if(gimbal.v  < -35000)gimbal.v +=65535;
    TIM8->CNT=0;
    
    gimbal.ecd += (gimbal.v *360/30/13);
    if(gimbal.ecd  > 35000) gimbal.ecd -=65535; 
    if(gimbal.ecd  < -35000)gimbal.ecd +=65535;
     
    gimbal.target_ecd = target;
    gimbal.error_ecd = circle_error(gimbal.target_ecd,gimbal.ecd,65535);    
    

    gimbal.pwm = -1.0*pid_calc(&gimbal_position_pid,gimbal.error_ecd,0);
    DJ_SET_PWM = pid_calc(&gimbal_speed_pid,gimbal.v,gimbal.pwm); 
    
    set_gimbal_pwm(4,DJ_SET_PWM);
}



