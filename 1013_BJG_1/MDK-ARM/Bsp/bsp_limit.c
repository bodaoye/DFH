#include "bsp_limit.h"

int test1 = 0;
int test2 = 0;
/**
  * @brief 检测按键是否按下
  * @param Key_Port是按键对应的IO口端口；Key_Pin是按键对应的IO口编号；Level是按键按下对应的电平状态
  * @retval 如果按下返回1，如果没有按下返回0
  */
uint16_t Key_Detection(GPIO_TypeDef * Key_Port, uint16_t Key_Pin, GPIO_PinState Level)
{
	uint16_t Key_State = 0;
	if(Level == HAL_GPIO_ReadPin(Key_Port,Key_Pin))
	{
		if(Level == HAL_GPIO_ReadPin(Key_Port,Key_Pin))
		{
			Key_State = 1;
		}
	}
	return Key_State;
}

void Switch_Ctrl(void){
	if(Key_Detection(GPIOB,GPIO_PIN_1,GPIO_PIN_SET) == 0){test1++;}//车左光电开关//右前
	if(Key_Detection(GPIOB,GPIO_PIN_2,GPIO_PIN_SET) == 0){test2++;}//车尾光电开关//右后
} 
