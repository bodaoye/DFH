/**
  *************************************************************************
  * @brief  WT53TR 维特智能小激光
  * @author JFG
  * @date   2022年9月13日15:29:15
  * @note   
  *************************************************************************
  */

#include “bsp_WT53R.h”

uint16_t wt53r_distance[2];

/**
  * @brief  tof 回传数据解算
  * @param  uint8_t *buff 串口缓存区指针
  * @retval 距离信息cm
  */
  */
void vWT53RFrontGetData(char *buff)
{
	uint16_t distance,strength;
	if(buff[0]==0x59&&buff[1]==0x59) //校验帧头
	{
		distance = buff[3]<<8 | buff[2];//合并高低八位
		strength = buff[5]<<8 | buff[4];//合并高低八位
		if(strength>100&&strength!=65535) //在强度范围内才为有效数据
		{
			wt53r_distance[0] = distance;  //手动乘10，单位化为 mm，个位无意义
		}
	}
}