#ifndef __BSP__LIMIT__H__
#define __BSP__LIMIT__H__

#include "stm32f4xx.h"
#include "gpio.h"

#define LimitNum 4

extern uint8_t limitStatus[LimitNum];

enum {
	L_left = 0,
	L_right = 1,
	L_head = 2,
	L_behind = 3
};

void getLimitStatus(uint8_t *status);

#endif
