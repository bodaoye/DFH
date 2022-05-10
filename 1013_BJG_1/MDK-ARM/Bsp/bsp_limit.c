#include "bsp_limit.h"

int test1 = 0;
int test2 = 0;
/**
  * @brief ��ⰴ���Ƿ���
  * @param Key_Port�ǰ�����Ӧ��IO�ڶ˿ڣ�Key_Pin�ǰ�����Ӧ��IO�ڱ�ţ�Level�ǰ������¶�Ӧ�ĵ�ƽ״̬
  * @retval ������·���1�����û�а��·���0
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
	if(Key_Detection(GPIOB,GPIO_PIN_1,GPIO_PIN_SET) == 0){test1++;}//�����翪��//��ǰ
	if(Key_Detection(GPIOB,GPIO_PIN_2,GPIO_PIN_SET) == 0){test2++;}//��β��翪��//�Һ�
} 
