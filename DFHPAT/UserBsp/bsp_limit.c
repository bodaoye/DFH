/**
  *************************************************************************
  * @brief  光电开关驱动
  * @author JFG
  * @date   2020/10/08
  * @note   四个光电开关，未标号
  *************************************************************************
  */
#include "bsp_limit.h"

limitKey_t limitFindKey;
uint8_t limitStatus[LimitNum] = {0};

/* 获取四个光电开关的电平信号 */
void getLimitStatus(uint8_t *status) {
	 status[L_Left_Head]   = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1);
	 status[L_Right_Head]  = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7);
	 status[L_Left_Tail]   = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6);
	 status[L_Right_Tail] = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);
}
