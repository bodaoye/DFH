#include "modeSwitch_task.h"
#include "remote_msg.h"
#include "cmsis_os.h"

/**********************************/
/* !!! �ϳ�ǰ�ǵó�ʼֵ�ĳ�0 !!! */
uint8_t lock_flag = 1;
ctrl_mode_e 	ctrl_mode;

void modeSwitch_task(void const *argu) {
	
    uint32_t thread_wake_time = osKernelSysTick();
	
    for (;;) {
    	if(lock_flag)
        remote_unlock();

    get_sw_mode();
    
		osDelay(5);
    osDelayUntil(&thread_wake_time, 1);
    }
}


/**
  * @brief ��ѡ�񿪹�
  * @param     
  * @attention  
	* @note  
  */
static void sw1_mode_handler(void)
{
	switch (rc.sw1)
	{
		case RC_UP:
		{
      ctrl_mode = PROTECT_MODE;
			break;
		}
		case RC_MI:
		{
      ctrl_mode = STANDARD_MODE;
			break;					
		}
		case RC_DN:
		{
			break;
		}
		default:
		{
			break;
		}	
	}
}

/**
  * @brief ��ѡ�񿪹�
  * @param     
  * @attention  
	* @note  
  */
static void sw2_mode_handler(void)
{
	switch (rc.sw2)
	{
		case RC_UP:
		{
			break;
		}
		case RC_MI:
		{
			break;			
		}
		case RC_DN:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

/**
  * @brief ģʽѡ��ص�����
  * @param     
  * @attention  
	* @note  
  */
void get_sw_mode(void)
{
   sw1_mode_handler();
	 sw2_mode_handler();
}

/**
  * @brief ң��������
  */
void remote_unlock(void)
{
	if(rc.sw1 == RC_MI && rc.sw2 == RC_UP)
	{
		if(rc.ch2==-660)
		{
			if(rc.ch1==660)
			{
				lock_flag = 0;
			}
		}					
	}
}	
