#include "modeSwitch_task.h"
#include "cmsis_os.h"

ctrl_mode_e 	ctrl_mode;

void modeSwitch_task(void const *argu) {
	
    uint32_t thread_wake_time = osKernelSysTick();
	
    for (;;) {
				
        osDelayUntil(&thread_wake_time, 1);
    }
}
