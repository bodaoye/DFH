/**
  ****************************************************************************
  * @brief  32��openmv4 H7 plus��ͨѶ����ģ��
  * @note   openmv��������֡��ʽ��0x66 + data_H + data_L + sum
  *         
  *         ������Ч�ԣ����data_H��data_L����0xA0����������Ч
  ****************************************************************************
  */
#include "bsp_openmv.h"


/*�Ӿ�������*/
volatile float x_VisionError = 0;
volatile float y_VisionError = 0;

/*������Ч�Ա�־*/
volatile uint8_t vision_data_flag = 0;

/*�Ӿ��ش����ݻ���*/
uint8_t VisionBuffArray[VISION_BUFF_SIZE];


/**
  * @brief  ����ƫ����㣬����Ч�Լ��
  * @note   ���Զ��崮�ڿ����жϺ����е���
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
  * @brief  �Ӿ�ģ��ͨ�ų�ʼ��
  * @note   ���Զ���ʹ�ã�Ҳ�����ɴ��������ʼ��������ͳһ����
  */
void VisionInit(void)
{
    __HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart5,VisionBuffArray,VISION_BUFF_SIZE);
}




