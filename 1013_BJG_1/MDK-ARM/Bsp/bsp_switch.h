#ifndef __SWITCH_H
#define __SWITCH_H

#include "stdint.h"
#include "bitband.h"

//#include "mode_switch.h"

/*光电开关数量*/
#define SWITCH_SIZE 4  


/*开关状态数组*/
extern uint8_t Switch[SWITCH_SIZE];


void vGetSwitchStatus(void);

#endif
