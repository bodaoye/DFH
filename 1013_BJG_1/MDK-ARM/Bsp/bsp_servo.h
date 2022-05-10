#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H


#include "stdint.h"
#include "tim.h"
#include "global_def.h"


#define SERVO_STOP 90

void servo_init(void);
void servo_set(int8_t angle);


#endif


