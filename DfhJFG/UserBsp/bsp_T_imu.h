/**
	* @file bsp_T_imu.h
	* @version 1.0
	* @date 2020.1.4
  *
  * @brief
  *
  *	@author YY
  *
  */
#ifndef __BSP_T_IMU_H
#define __BSP_T_IMU_H

#include "bsp_can.h"
#include "string.h"
#include "cmsis_os.h"
#include "task.h"
#include "main.h"

typedef enum
{
	DATA_NOT_READY         = 0x00,
	DATA_READY             = 0x01,
	DATA_ERROR             = 0x02,
} imu_data_status_e;

typedef struct
{
	float pitch;
	float yaw;
	float wy;
	float wz;
	imu_data_status_e data_ready_flag;
} Taurus_imu_data_t;

extern Taurus_imu_data_t imu_data;

void T_imu_calcu(uint32_t can_id,uint8_t * CAN_Rx_data);

#endif

