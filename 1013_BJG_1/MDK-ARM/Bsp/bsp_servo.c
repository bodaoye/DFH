/**
  *************************************************************************
  * @brief  180°舵机驱动
  * @author ZZJ
  * @date   2020/9/27
  * @note   20000微秒周期，500到2500微秒的高电平
  *         2000微秒对应180度
  * 对于360°连续舵机：
  *         等于 90° 停止10 3                              
  *         大于 90° 逆时针转，反之亦反
  *************************************************************************
  */
#include "bsp_servo.h"

//uint32_t temp = 15;
uint32_t temp = 0;
void servo_set(int8_t angle)
{ 
        temp = (angle%180)/180.0f * 20 + 5;
    
        __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_4,temp);
    __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_3,temp);
    __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_2,temp);
    __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_1,temp);
}


void servo_init(void)
{
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
}
