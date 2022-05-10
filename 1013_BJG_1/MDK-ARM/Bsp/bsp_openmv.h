#ifndef __BSP_OPENMV_H
#define __BSP_OPENMV_H

#include "stdint.h"
#include "usart.h"


#define VISION_BUFF_SIZE 20



/*������Ч�Ա�־*/
extern volatile uint8_t vision_data_flag;
/*�������*/
extern volatile float x_VisionError; 
extern volatile float y_VisionError; 
/*���ݽ���DMA����*/
extern uint8_t VisionBuffArray[VISION_BUFF_SIZE];
extern int vision_header;
extern int16_t y_temp;
extern int16_t x_temp;
void VisionInit(void);
void vVisionGetData(uint8_t* buff);

#endif

