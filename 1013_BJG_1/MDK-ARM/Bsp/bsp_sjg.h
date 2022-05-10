#ifndef		_TOF_H_
#define		_TOF_H_


#include "main.h"
#include "usart.h"
#include "bsp_uart.h"

#define TF_BUFFER_SIZE  30
#define TF_NUM       2


/*小激光结构体类型*/
typedef struct _TFMensure_t
{
    uint8_t  ucId;
    uint8_t  ucRxFlag;
    uint16_t usDistance;
}TFMensure_t;

/*小激光结构体数组*/
extern TFMensure_t TF[TF_NUM];

/*Function declaration*/
//void Send_Data_To_UART1(char dat)；
extern uint8_t tof0_dma_buff[30];
extern uint8_t tof1_dma_buff[30];
extern uint16_t JG[5];
void Get_Data_From_TOF0(void);
void Get_ID_From_TOF0(void);
void Tof_Init(void);
#endif





