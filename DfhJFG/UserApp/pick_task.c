#include "pick_task.h"
#include "bsp_debug.h"
#include "cmsis_os.h"
#include "bsp_ServerDriver.h"

/* 分拣开关 */

/* 拍打杆开关 */

/* 收集盒开关 */

void pick_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    for (;;) {
	
     osDelayUntil(&thread_wake_time, 1);
    }
}

