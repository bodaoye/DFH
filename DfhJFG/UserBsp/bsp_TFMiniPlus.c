/**
  *************************************************************************
  * @brief  TFminni Plus大激光测距模块驱动
  * @author ZZJ
  * @date   2020/10/08
  * @note   前后各一个，前对应USART3，后对应UART4
  *         波特率：115200  帧率：250
  *         统一放入"bsp_uart.c"的串口设备中初始化
  *************************************************************************
  */
#include "bsp_TFMiniPlus.h"


uint16_t tfmin_distance[2];


/**
  * @brief  TFminiPlus 数据解算
  * @param  uint8_t *buff
  * @retval 距离信息cm
  */
void vTfFrontGetData(uint8_t *buff)
{
	uint16_t distance,strength;
	if(buff[0]==0x59&&buff[1]==0x59) //校验帧头
	{
		distance=buff[3]<<8 | buff[2];
		strength=buff[5]<<8 | buff[4];
		if(strength>100&&strength!=65535) //在强度范围内才为有效数据
		{
			tfmin_distance[1] = distance;  //手动乘10，单位化为 mm，个位无意义
		}
	}
}

/**
  * @brief  TFminiPlus 数据解算
  * @param  uint8_t *buff
  * @retval 距离信息cm
  */
void vTfBackGetData(uint8_t *buff)
{
	uint16_t distance,strength;
	if(buff[0]==0x59&&buff[1]==0x59) //校验帧头
	{
		distance=buff[3]<<8 | buff[2];
		strength=buff[5]<<8 | buff[4];
		if(strength>100&&strength!=65535) //在强度范围内才为有效数据
		{
			tfmin_distance[0] = distance * 10;
		}
	}
}

