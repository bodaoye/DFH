#ifndef __BSP_PRINTF_H
#define __BSP_PRINTF_H

#include "usart.h"
#include "stdio.h"



  /* Private function prototypes -----------------------------------------------*/

  #ifdef __GNUC__
      #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #else
      #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #endif
   
  /* Private functions ---------------------------------------------------------*/