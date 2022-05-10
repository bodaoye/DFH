#ifndef __bsp_motor_H
#define __bsp_motor_H

#include "stdint.h"
#include "gpio.h"

#define PWM_LIMIT   8350    //PWM·¶Î§£º0-8350  

void set_pwm(uint8_t moto_number,int pwm);
void set_gimbal_pwm(uint8_t moto_number,int pwm);
void bsp_Stop(void);
void speed_filter(void);
#endif


