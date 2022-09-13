/**
	* @file bsp_T_imu.c
	* @version 1.0
	* @date 2020.1.4
  *
  * @brief  Taurus陀螺仪内测版
  *
  *	@author YY
  *
  */
#include "bsp_T_imu.h"
#include "bsp_can.h"
#include "bsp_uart.h"

float 	palstance_buffer[2];
float 	angle_buffer[2];

Taurus_imu_data_t   imu_data;

void T_imu_calcu(uint32_t can_id,uint8_t * CAN_Rx_data)
{
	switch(can_id)
  {
		/* 角速度 */
		case TIMU_PALSTANCE_ID:
		{
			memcpy(palstance_buffer,CAN_Rx_data,8);
			imu_data.wy = palstance_buffer[0];
			imu_data.wz = palstance_buffer[1];
			break;
		}
		/* 角度 */
		case TIMU_ANGLE_ID:
		{
			memcpy(angle_buffer,CAN_Rx_data,8);
			imu_data.pitch = angle_buffer[0];
			imu_data.yaw 	 = angle_buffer[1];
			break;
		}
	}
}

