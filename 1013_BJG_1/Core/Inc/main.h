/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define switch_Pin GPIO_PIN_2
#define switch_GPIO_Port GPIOE
#define switchE3_Pin GPIO_PIN_3
#define switchE3_GPIO_Port GPIOE
#define PWM1_Pin GPIO_PIN_6
#define PWM1_GPIO_Port GPIOF
#define PWM0_Pin GPIO_PIN_7
#define PWM0_GPIO_Port GPIOF
#define PWM2_Pin GPIO_PIN_8
#define PWM2_GPIO_Port GPIOF
#define servo0_Pin GPIO_PIN_1
#define servo0_GPIO_Port GPIOA
#define servo1_Pin GPIO_PIN_2
#define servo1_GPIO_Port GPIOA
#define servo2_Pin GPIO_PIN_3
#define servo2_GPIO_Port GPIOA
#define ECD2A_Pin GPIO_PIN_5
#define ECD2A_GPIO_Port GPIOA
#define IN02_Pin GPIO_PIN_6
#define IN02_GPIO_Port GPIOA
#define PWM4_Pin GPIO_PIN_7
#define PWM4_GPIO_Port GPIOA
#define switchB1_Pin GPIO_PIN_1
#define switchB1_GPIO_Port GPIOB
#define switchB2_Pin GPIO_PIN_2
#define switchB2_GPIO_Port GPIOB
#define IN11_Pin GPIO_PIN_7
#define IN11_GPIO_Port GPIOE
#define IN12_Pin GPIO_PIN_8
#define IN12_GPIO_Port GPIOE
#define ECD1A_Pin GPIO_PIN_9
#define ECD1A_GPIO_Port GPIOE
#define IN21_Pin GPIO_PIN_10
#define IN21_GPIO_Port GPIOE
#define ECD1B_Pin GPIO_PIN_11
#define ECD1B_GPIO_Port GPIOE
#define IN22_Pin GPIO_PIN_12
#define IN22_GPIO_Port GPIOE
#define B_BJG_TX_Pin GPIO_PIN_10
#define B_BJG_TX_GPIO_Port GPIOB
#define RX_TF2_uart3_Pin GPIO_PIN_11
#define RX_TF2_uart3_GPIO_Port GPIOB
#define PWM3_Pin GPIO_PIN_15
#define PWM3_GPIO_Port GPIOB
#define ECD4A_Pin GPIO_PIN_12
#define ECD4A_GPIO_Port GPIOD
#define ECD4B_Pin GPIO_PIN_13
#define ECD4B_GPIO_Port GPIOD
#define ECD4AC6_Pin GPIO_PIN_6
#define ECD4AC6_GPIO_Port GPIOC
#define ECD4BC7_Pin GPIO_PIN_7
#define ECD4BC7_GPIO_Port GPIOC
#define IN41_Pin GPIO_PIN_8
#define IN41_GPIO_Port GPIOA
#define IN01_Pin GPIO_PIN_12
#define IN01_GPIO_Port GPIOA
#define BJG_TX_Pin GPIO_PIN_10
#define BJG_TX_GPIO_Port GPIOC
#define BJG_RX_Pin GPIO_PIN_11
#define BJG_RX_GPIO_Port GPIOC
#define minibalance_Pin GPIO_PIN_12
#define minibalance_GPIO_Port GPIOC
#define minibalanceD2_Pin GPIO_PIN_2
#define minibalanceD2_GPIO_Port GPIOD
#define SJG_RX_Pin GPIO_PIN_9
#define SJG_RX_GPIO_Port GPIOG
#define SJG_TX_Pin GPIO_PIN_14
#define SJG_TX_GPIO_Port GPIOG
#define IN42_Pin GPIO_PIN_15
#define IN42_GPIO_Port GPIOG
#define ECD2B_Pin GPIO_PIN_3
#define ECD2B_GPIO_Port GPIOB
#define ECD3A_Pin GPIO_PIN_4
#define ECD3A_GPIO_Port GPIOB
#define ECD3B_Pin GPIO_PIN_5
#define ECD3B_GPIO_Port GPIOB
#define IN32_Pin GPIO_PIN_6
#define IN32_GPIO_Port GPIOB
#define IN31_Pin GPIO_PIN_7
#define IN31_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
