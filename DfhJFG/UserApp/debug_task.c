#include "debug_task.h"
#include "bsp_debug.h"
#include "cmsis_os.h"

uint8_t debug_wave = 0; /* 用以选择打印的数据类型 */
//弱函数的强定义
void DataWavePkg(void) {
    /* 视觉调试专用部分  */
    switch (debug_wave) {
			
    }
}

void debug_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    for (;;) {
//        DataWave(&huart4);	
        osDelayUntil(&thread_wake_time, 1);
    }
}

