#include "tim.h"
#include "mat.h"
#include "bsp_sjg.h"
#include "bsp_bjg.h"
#include "bsp_gyro.h"
#include "chassis_task.h"
#include "Collect_Init.h"
#include "global_def.h"
#include "DataScope_DP.h"
#include "bsp_openmv.h"
#include "vision_task.h"
int BR = 0;
int BL = 1;
int FR = 2;
int FL = 3;
int tof0 = 0;
int tof1 = 0;

uint16_t distance = 0;

 void Data_Collect_Init(void){
    Vision_Task_Init();
    Tof_Init();
    WT101_Init();
    vTFminiPlusInit();

    VisionInit();
    Chassis_Init();

}


void USER_UART_IDLECallback(UART_HandleTypeDef *huart){
    if(huart->Instance== USART1){							//图形调试串口
      
    }

    if(huart->Instance == USART2){  // 陀螺仪数据处理  
        Get_Angle();        
    }

    if(huart->Instance== USART3){							//前大激光串口
        vTfFrontGetData(TFminiPlusBuffArray_Front);
        HAL_UART_Receive_DMA(&huart3, (uint8_t*)TFminiPlusBuffArray_Front, TFMINIPLUS_BUFF_SIZE);

    } 

    if(huart->Instance== UART4){							//后大激光串口
        vTfBackGetData(TFminiPlusBuffArray_Back);
        HAL_UART_Receive_DMA(huart, (uint8_t*)TFminiPlusBuffArray_Back, TFMINIPLUS_BUFF_SIZE);
    }

    if(huart->Instance == UART5)  // openmv 数据接收
    {
        vVisionGetData(VisionBuffArray);
   //    memset(VisionBuffArray,0,VISION_BUFF_SIZE);
        HAL_UART_Receive_DMA(huart, (uint8_t*)VisionBuffArray, VISION_BUFF_SIZE);
    }

    if(huart->Instance== USART6){							//小激光串口
        Get_Data_From_TOF0();
        T0f_Conversion_HEX( tof1_dma_buff, &tof1 );
        JG[4]= tof1;
        HAL_UART_Receive_DMA(&huart6,tof1_dma_buff,TF_BUFFER_SIZE);
    }		
}










