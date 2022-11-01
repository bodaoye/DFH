#ifndef __BSP__LIMIT__H__
#define __BSP__LIMIT__H__

#include "stm32f4xx.h"
#include "gpio.h"

#define LimitNum 4



enum {
	L_Left_Head = 0,
	L_Right_Head = 1,
	L_Left_Tail = 2,
	L_Right_Tail = 3
};
typedef struct {
	uint8_t firstTrigger;
	uint8_t secondTrigger;
	uint8_t key;
	uint8_t keyflag;
}limitKey_t;
extern uint8_t limitStatus[LimitNum];
extern limitKey_t limitFindKey;
void getLimitStatus(uint8_t *status);

#endif
