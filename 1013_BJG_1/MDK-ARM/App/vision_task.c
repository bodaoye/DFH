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

#define AIM_TIME		1200		//��λʱ��


gimbal_t gimbal;
vision_t vision;
uint8_t last_ctrl_mode;
uint8_t  gimbal_status = 0;
//ʩ��״̬λ     0Ϊ�ջ�״̬    1Ϊ�Ӿ�ʶ�𵽻���   2Ϊ��׼����ͣ����׼��ʩ��   3Ϊ�������ʩ�����
uint8_t  grass_total = 0;	 //��¼��ʩ����Ŀ,��10��

void Vision_Task_Init(void)
{
    servo_init();                                                                       //������ʱ��5��PWM
    HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);		                                //��̨���
	PID_struct_init(&gimbal_speed_pid,POSITION_PID,10000,10000,55.0f,1.4f,0.0f);		//��̨����ٶȻ�PID
    PID_struct_init(&gimbal_position_pid,POSITION_PID,10000,10000,0.05f,0.0f,0.0f);		//��̨���λ�û�PID     
}
/**
  * @brief �Ӿ�������
  * @param input : void
  *        output: void
  * @note  ����openmv���ز�ͬ��֡ͷ����ȥִ�в�ͬ������0x2c���������0xfe����������0xff������ʶ����
  */
int test_s = 0;
void Vision_Task(void)
{
    #if 0
    if((Ctrl_Mode==Vision_Mode1 || Ctrl_Mode==Vision_Mode2) && gimbal_status==0 && vision_header == 0x2c && grass_total < 10)	
    {
        gimbal_status = 1;//ʶ�𵽻�������Ҫʩ��
        last_ctrl_mode = Ctrl_Mode;//������һ��ģʽ
        switch_mode(Vision_Aim_Mode);//�л�Ϊʩ��ģʽ
    }
    else if(gimbal_status==1 && chassis.vx==0 && chassis.vy==0){//��Ҫʵ����֤�Ƿ������ͣ�²���׼������
        gimbal_status = 2;//�Ѿ���׼����
    }
    else if(gimbal_status == 2){
        Servo_Work();//�������
        grass_total++;//��ʩ����Ŀ
        gimbal_status = 3;//ʩ�����
    }
    else if(gimbal_status == 3){
        switch_mode(last_ctrl_mode);//�л�Ϊԭģʽ
        gimbal_status = 0;
    }
    
    if(vision_header == 0xFE)//ʶ������,����
    {
        //����ͣ��,���̵�1��
    }
    #endif
//    if(vision_header == 0x2c)
//    {
//         HAL_GPIO_Writ 			          //��ת
//        HAL_Delay(110);
//        Gimbal_Stop();
//        HAL_Delay(600);
        
        HAL_GPIO_WritePin(IN41_GPIO_Port,IN41_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(IN42_GPIO_Port,IN42_Pin,GPIO_PIN_SET);	
        TIM14->CCR1=9000;					//���	
        HAL_Delay(135);
        Gimbal_Stop();    
        HAL_Delay(450);
        
        servo_set(85);//85��� 45����
        HAL_Delay(450);//����ʱ����Ҫ��ʩ�ʽṹ�İ����� 300̫��
        servo_set(45);
        HAL_Delay(550);
        
        HAL_GPIO_WritePin(IN41_GPIO_Port,IN41_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(IN42_GPIO_Port,IN42_Pin,GPIO_PIN_RESET);	
        TIM14->CCR1=9000;						//�ջ�
        HAL_Delay(100);
        Gimbal_Stop();


       
//             set_gimbal_pwm(4,9000);
        HAL_Delay(90000);
//    }
}
/**
  * @brief ��׼ʩ�����纯��
  * @param input : void
  *        output: void
  * @note  Ŀ��:��װ����
           ���Ӿ������е��ã�ʹ����ͣ�ں��ʵ�ʩ��λ�á�
  */
void aim_yellow(void)
{
    if(x_temp<=40||x_temp>=60)
    {
        chassis.vy = pid_calc(&vision_aim_y,x_temp,50);
    }
    else if(x_temp>40&&x_temp<60)
    {   
        if(chassis.vy > 0) chassis.vy -= 1;//��һ��ȥ��else if���,�����Ƿ����׼ȷ��׼����ͣ��
    }
}

/**
  * @brief ��ֹ̨ͣ����
  * @param input : void
  *        output: void
  * @note  ����̨������������е��ã�ʩ�ʽṹ��������λ�ñ���ã�ʹ��ͣ�ں���λ�á�
  */
void Gimbal_Stop(void)
{  
	gimbal.pwm = 0;
	TIM14->CCR1= 0;
}



/**
  * @brief �����������
  * @param input : void
  *        output: void
  * @note  ʶ�𵽻���ʱ����
  */
void Servo_Work(void)
{
/*�������ʩ�ʽṹ*/    
    servo_set(85);//85��� 45����
    HAL_Delay(450);//����ʱ����Ҫ��ʩ�ʽṹ�İ����� 300̫��
    servo_set(45);
}

/**
  * @brief ��̨�����������
  * @param input : way, ��������������ջ�, 0Ϊ�������, 1Ϊ�����ջ�
  *        output: void
  * @note  ʶ�𵽻���ʱ����
  */

void Gimbal_Work(uint8_t way)
{
    if(way == 0)//������
	{
		set_gimbal_pwm(4,-9000);
        HAL_Delay(115);
        Gimbal_Stop();
//        HAL_Delay(100);//�����費��Ҫ,�������Ҫ����ɾ�����
    
	}
	else if(way == 1)//����ջ�	
	{
		set_gimbal_pwm(4,9000);
        HAL_Delay(90);
        Gimbal_Stop();
//        HAL_Delay(100);//�����費��Ҫ,�������Ҫ����ɾ�����
	}

}
/**
  * @brief ��̨����PID����
  * @param input : target,�������������Ҫ����,�����Ӧλ�û�PID��targetֵ
  *        output: void
  * @note  Ŀ��:��װ����
           ����̨������������е���
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



