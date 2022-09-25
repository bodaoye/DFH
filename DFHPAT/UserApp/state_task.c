#include "modeSwitch_task.h"
#include "remote_msg.h"
#include "cmsis_os.h"


void state_task(void const *argu) {
	
    uint32_t thread_wake_time = osKernelSysTick();
	
   for (;;) {
    
		osDelay(5);
    osDelayUntil(&thread_wake_time, 1);
   }
}
