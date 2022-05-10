#include "bsp_printf.h"


//PUTCHAR_PROTOTYPE
//{
//    while((USART1->SR&0X40)==0);
//	USART1->DR = (uint8_t) ch;
//	return ch;
//}

void RCC_Configuration(void)
{    
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
#ifndef USE_STM3210C_EVAL
  /* Enable USARTy Clock */
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 
#else
  /* Enable USARTy Clock */
  RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE); 
#endif
  /* Enable USARTz Clock */
  RCC_APB1PeriphClockCmd(USARTz_CLK, ENABLE);  
}

/* Configure the GPIO ports */

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* Enable the USART3 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
  
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  
#elif defined(USE_STM3210B_EVAL) || defined(USE_STM32100B_EVAL)
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif

  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  
  /* Configure USARTz Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);  
  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

  /* Configure USARTz Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure); 
}

/* Configure the GPIO ports */

 

/* USARTy configuration ------------------------------------------------------*/

void USART_Configuration()
{

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure USARTy */
  USART_Init(USARTy, &USART_InitStructure);
  /* Configure USARTz */
  USART_Init(USARTz, &USART_InitStructure);

}

/* USARTy configuration ------------------------------------------------------*/

 

/*  Retargets the C library printf function to the USART.  */
PUTCHAR_PROTOTYPE
{
USART_SendData(USART1, (uint8_t) ch);


while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
{}

return ch;
}

/*  Retargets the C library printf function to the USART.  */
