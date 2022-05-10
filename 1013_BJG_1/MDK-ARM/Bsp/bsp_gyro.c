#include "bsp_gyro.h"


uint8_t WT101_buf[11];
uint8_t angle_flag;					//该标志位作用：目标角度发生变化时，进入转弯模式后，陀螺仪仍未解算数据，导致误以为已经转完弯。

float yaw=0;
float angle_get=0;
float Angle_Error_WT=0;						//陀螺仪的角度差值，调节车头直PID
float angle_set=0;							//目标角度，转90时+90即可			




void Get_Angle()
{
	uint8_t tmp_flag=0;
	tmp_flag =  __HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE); 
  if((tmp_flag != RESET))
   { 
     __HAL_UART_CLEAR_IDLEFLAG(&huart2);
		 HAL_UART_DMAStop(&huart2);
		if(WT101_buf[0]==0x55 && WT101_buf[1]== 0x53)
		{
			angle_get = (( (WT101_buf[7]<<8) | WT101_buf[6]) / 32768.0f * 180.0f);
			Angle_Error_WT = angle_set -  angle_get ;										//陀螺仪角度差
			if(Angle_Error_WT <= -180)			Angle_Error_WT +=360;
      if(Angle_Error_WT >   180)			Angle_Error_WT -=360;
      if(angle_get <= -180)			      angle_get +=360;
      if(angle_get >   180)			      angle_get -=360;
			angle_flag=1;
		}
		HAL_UART_Receive_DMA(&huart2,WT101_buf,11);
	}
}
void WT101_Init()
{
	HAL_UART_Receive_DMA(&huart2,WT101_buf,11);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
}



