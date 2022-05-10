/**
  ****************************************************************************
  * @brief  32与openmv4 H7 plus的通讯驱动模块
  * @note   openmv发送数据帧格式：0x66 + data_H + data_L + sum
  *         
  *         数据有效性：如果data_H与data_L都是0xA0，则数据无效
  ****************************************************************************
  */
#include "bsp_openmv.h"


/*视觉误差变量*/
volatile float x_VisionError = 0;
volatile float y_VisionError = 0;

/*数据有效性标志*/
volatile uint8_t vision_data_flag = 0;

/*视觉回传数据缓存*/
uint8_t VisionBuffArray[VISION_BUFF_SIZE];


/**
  * @brief  解算偏差计算，与有效性检测
  * @note   在自定义串口空闲中断函数中调用
  */
    int16_t y_temp = 0;
    int16_t x_temp = 0;
    int vision_header = 0;
void vVisionGetData(uint8_t* buff)
{
    if( buff[0] == 0x2c && buff[3] == 0x5b){
        vision_header = buff[0];
        x_temp = (int16_t)buff[1];
        y_temp = (int16_t)buff[2];
    }
    else if( buff[0] == 0xFE ){
        vision_header = buff[0];
        x_temp = 0;
        y_temp = 0;
    }
    else if( buff[0] == 0xFF ){
        vision_header = buff[0];
        x_temp = 0;
        y_temp = 0;
    }
		
}

/**
  * @brief  视觉模块通信初始化
  * @note   可以独立使用，也可以由串口外设初始化函数中统一调用
  */
void VisionInit(void)
{
    __HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart5,VisionBuffArray,VISION_BUFF_SIZE);
}




