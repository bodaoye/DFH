/**
  *************************************************************************
  * @brief  TFminni Plus Large laser ranging module driver
  * @author ZZJ
  * @date   2020/10/08
  * @note 
  *       	Baud rate：115200  frame rate：250
  *************************************************************************
  */
#include "bsp_bjg.h"
#include "bsp_sjg.h"

uint16_t tfmin_distance[2];


uint8_t TFminiPlusBuffArray_Front[TFMINIPLUS_BUFF_SIZE] = {0};
uint8_t TFminiPlusBuffArray_Back[TFMINIPLUS_BUFF_SIZE] = {0};

/**
  * @brief  TFminiPlus Data solution
  * @param  uint8_t *buff
  * @retval Distance data (cm)
  */
void vTfFrontGetData(uint8_t *buff)
{
	uint16_t distance,strength;
	if(buff[0]==0x59&&buff[1]==0x59) //check frame header
	{
		distance=buff[3]<<8 | buff[2];
		strength=buff[5]<<8 | buff[4];
		if(strength>100&&strength!=65535) //Only valid data within the intensity range
		{
 			JG[0] = distance;  //前激光
		}
	}
}


///**
//  * @brief  TFminiPlus Data solution
//  * @param  uint8_t *buff
//  * @retval Distance data (cm)
//  */
void vTfBackGetData(uint8_t *buff)
{
	uint16_t distance,strength;
	if(buff[0]==0x59&&buff[1]==0x59) //check the frame header
	{
		distance=buff[3]<<8 | buff[2];
		strength=buff[5]<<8 | buff[4];
		if(strength>100&&strength!=65535) //only the valid data within the intensity range
		{
			JG[3] = distance/10;//后激光
		}
	}
}

/**
  * @brief  TFminiPlus
  * @note   Called in the serial port peripheral initialization function
  */
void vTFminiPlusInit(void)
{
	
		HAL_UART_Receive_DMA(&huart3,TFminiPlusBuffArray_Front,TFMINIPLUS_BUFF_SIZE);
    HAL_UART_Receive_DMA(&huart4,TFminiPlusBuffArray_Back,TFMINIPLUS_BUFF_SIZE);
	
	  __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);

}
