#include "bsp_motor.h"


/**
  * @brief  ��������ת��pwm
  * @param  ������,pwm
  * @retval ��
  */
void set_pwm(uint8_t moto_number,int pwm)
{
	switch(moto_number)
	{
    case 0:		
			if(pwm>=0)
			{
				HAL_GPIO_WritePin(IN31_GPIO_Port,IN31_Pin,GPIO_PIN_RESET);
                HAL_GPIO_WritePin(IN32_GPIO_Port,IN32_Pin,GPIO_PIN_SET); 			
				TIM12->CCR2=pwm;					//��ת
				break;
			}
			else if(pwm<0)
			{
				HAL_GPIO_WritePin(IN31_GPIO_Port,IN31_Pin,GPIO_PIN_SET);
                HAL_GPIO_WritePin(IN32_GPIO_Port,IN32_Pin,GPIO_PIN_RESET); 			
				TIM12->CCR2=-pwm;					//��ת
				break;
			}	
    case 1:		
			if(pwm>=0)
			{
			 	HAL_GPIO_WritePin(IN21_GPIO_Port,IN21_Pin,GPIO_PIN_RESET);
                HAL_GPIO_WritePin(IN22_GPIO_Port,IN22_Pin,GPIO_PIN_SET); 			
				TIM13->CCR1=pwm;				  //��ת
				break;
			}
			else if(pwm<0)
			{
				HAL_GPIO_WritePin(IN21_GPIO_Port,IN21_Pin,GPIO_PIN_SET);
                HAL_GPIO_WritePin(IN22_GPIO_Port,IN22_Pin,GPIO_PIN_RESET); 			
				TIM13->CCR1=-pwm;					//��ת
				break;
			}
					
    case 2:		//FR 2
			if(pwm>=0)
			{
                HAL_GPIO_WritePin(IN11_GPIO_Port,IN11_Pin,GPIO_PIN_SET);
                HAL_GPIO_WritePin(IN12_GPIO_Port,IN12_Pin,GPIO_PIN_RESET);
				TIM10->CCR1=pwm;					//��ת
				break;
			}
			else if(pwm<0)
			{
				HAL_GPIO_WritePin(IN11_GPIO_Port,IN11_Pin,GPIO_PIN_RESET);
                HAL_GPIO_WritePin(IN12_GPIO_Port,IN12_Pin,GPIO_PIN_SET); 			
				TIM10->CCR1=-pwm;					//��ת
				break;
			}
								
	case 3:			
			if(pwm>=0)
			{
				HAL_GPIO_WritePin(IN01_GPIO_Port,IN01_Pin,GPIO_PIN_SET);
                HAL_GPIO_WritePin(IN02_GPIO_Port,IN02_Pin,GPIO_PIN_RESET);	
				TIM11->CCR1=pwm;					//��ת
				break;
			}
			else if(pwm<0)
			{
				HAL_GPIO_WritePin(IN01_GPIO_Port,IN01_Pin,GPIO_PIN_RESET);
                HAL_GPIO_WritePin(IN02_GPIO_Port,IN02_Pin,GPIO_PIN_SET);		
				TIM11->CCR1=-pwm;					//��ת
				break;
			}
	
    }

}






void set_gimbal_pwm(uint8_t moto_number,int pwm)
{
    switch(moto_number)
    {
    case 4:		
			if(pwm>=0)			//���4Ϊ��̨���
			{
				HAL_GPIO_WritePin(IN41_GPIO_Port,IN41_Pin,GPIO_PIN_SET);
                HAL_GPIO_WritePin(IN42_GPIO_Port,IN42_Pin,GPIO_PIN_RESET);	
				TIM14->CCR1=pwm;						//��ת
				break;
			}
			 if(pwm<0)
			{
				HAL_GPIO_WritePin(IN41_GPIO_Port,IN41_Pin,GPIO_PIN_RESET);
                HAL_GPIO_WritePin(IN42_GPIO_Port,IN42_Pin,GPIO_PIN_SET);			
                TIM14->CCR1=-pwm;				          //��ת
				break;
			}
            								
	case 3:			
			if(pwm>=0)
			{
				HAL_GPIO_WritePin(IN01_GPIO_Port,IN01_Pin,GPIO_PIN_SET);
                HAL_GPIO_WritePin(IN02_GPIO_Port,IN02_Pin,GPIO_PIN_RESET);	
				TIM11->CCR1=pwm;					//��ת
				break;
			}
			else if(pwm<0)
			{
				HAL_GPIO_WritePin(IN01_GPIO_Port,IN01_Pin,GPIO_PIN_RESET);
                HAL_GPIO_WritePin(IN02_GPIO_Port,IN02_Pin,GPIO_PIN_SET);		
				TIM11->CCR1=-pwm;					//��ת
				break;
			}
	
    }
}

void bsp_Stop(void)
{
    HAL_GPIO_WritePin(IN01_GPIO_Port,IN01_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN02_GPIO_Port,IN02_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN11_GPIO_Port,IN11_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN12_GPIO_Port,IN12_Pin,GPIO_PIN_RESET); 	
    HAL_GPIO_WritePin(IN21_GPIO_Port,IN21_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN22_GPIO_Port,IN22_Pin,GPIO_PIN_RESET); 
    HAL_GPIO_WritePin(IN31_GPIO_Port,IN31_Pin,GPIO_PIN_RESET);	
    HAL_GPIO_WritePin(IN32_GPIO_Port,IN32_Pin,GPIO_PIN_RESET); 	
    TIM10->CCR1=0;	
    TIM11->CCR1=0;
    TIM12->CCR2=0;
    TIM13->CCR1=0;
}

















