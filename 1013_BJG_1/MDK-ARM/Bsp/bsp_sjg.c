/**
  *************************************************************************
  * @brief  TOF10120 С������ģ������
  * @author WZB
  * @date   2021/10/10
  * @note   �������� ID ����Ϊ0x00 - 0x01
  *         usart6 ���� 2��С����
  *************************************************************************
  */  
#include "bsp_sjg.h"
#include "mat.h"

/*С���⻺����*/
uint8_t tof0_dma_buff[30], tof1_dma_buff[30],tof2_dma_buff[30];
uint16_t JG[5];//���վ���ֵ 0Ϊ��ͷ�󼤹�,3Ϊ��β�󼤹�,4Ϊ����ǽ����
/*С����ṹ������*/
TFMensure_t TF[TF_NUM];

/* Private variables ---------------------------------------------------------*/
/**
    *@bref      Send the Initial data to TOF10120
    *@param[in] None
    *@param[in] None
    */
void Send_Data_To_UART(uint8_t dat)
{      
//    USART1->DR = dat; 
//		while(!(USART3->SR & (1 << 6))); //Send cyclically untill it was finished 
		USART6->DR = dat;
		while(!(USART6->SR & (1 << 6)));//Send cyclically untill it was finished  
}

/**
    *@bref      TOF_Init
    *@param[in] None
    *@param[in] None
		*@Purpose   Wrapper function
    */


void Tof_Init(void){
	Send_Data_To_UART('s');
	Send_Data_To_UART('5');
	Send_Data_To_UART('-');
	Send_Data_To_UART('1');
	Send_Data_To_UART('#'); //Set to passive reading mode
	HAL_Delay(10);
  
    Get_Data_From_TOF0();
	Get_ID_From_TOF0();
  
	__HAL_UART_ENABLE_IT(&huart6,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart6,tof1_dma_buff,TF_BUFFER_SIZE);	
}

/**
    *@bref      Send and recieve cyclically
    *@param[in] None
    *@param[in] None
		*@Purpose   Wrapper function
    */



unsigned char rxcnt0 = 0;
void Get_Data_From_TOF0(void){

		Send_Data_To_UART('r');
		Send_Data_To_UART('6');
		Send_Data_To_UART('#');
	
	}
void Get_ID_From_TOF0(void){

		Send_Data_To_UART('r');
		Send_Data_To_UART('7');
		Send_Data_To_UART('#');  
		}





