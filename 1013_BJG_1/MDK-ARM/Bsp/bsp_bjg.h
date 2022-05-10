#ifndef __TFMINIPLUS_H
#define __TFMINIPLUS_H

#include "stm32f4xx.h"
#include "usart.h"


/*TFminiplus 数据缓存大小*/
#define TFMINIPLUS_BUFF_SIZE 						50

#define BW_TFLidar_DATA_Len             9
#define BW_TFLidar_DATA_HEAD            0x59
/*TFminiplus 数据缓存*/
extern uint8_t TFminiPlusBuffArray_Front[TFMINIPLUS_BUFF_SIZE];
extern uint8_t TFminiPlusBuffArray_Back[TFMINIPLUS_BUFF_SIZE];

/*TFminiplus 处理后的数据*/
extern uint16_t tfmin_distance[2];

void vTFminiPlusInit(void);
void vTfFrontGetData(uint8_t *buff);
void vTfBackGetData(uint8_t *buff);

void USART1_RX_Proc(uint8_t *buf);
#endif

