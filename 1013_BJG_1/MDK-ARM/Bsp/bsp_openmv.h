#ifndef __BSP_OPENMV_H
#define __BSP_OPENMV_H

#include "stdint.h"
#include "usart.h"


#define VISION_BUFF_SIZE 20



/*数据有效性标志*/
extern volatile uint8_t vision_data_flag;
/*数据误差*/
extern volatile float x_VisionError; 
extern volatile float y_VisionError; 
/*数据接收DMA缓存*/
extern uint8_t VisionBuffArray[VISION_BUFF_SIZE];
extern int vision_header;
extern int16_t y_temp;
extern int16_t x_temp;
void VisionInit(void);
void vVisionGetData(uint8_t* buff);

#endif

