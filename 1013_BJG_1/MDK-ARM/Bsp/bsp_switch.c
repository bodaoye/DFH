/**
  *************************************************************************
  * @brief  NPN NO �͹�翪������
  * @author ZZJ
  * @date   2020/9/13
  * @note   ���ϰ���ʱ����ߵ�ƽ��
  *************************************************************************
  */
#include "bsp_switch.h"


#define FL  0
#define BL  1
#define BR  2
#define FR  3


#define FL_switch PBin(1)
#define BL_switch PBin(2)
#define FR_switch PEin(2)
#define BR_switch PEin(3)

/*����״̬����*/
uint8_t Switch[SWITCH_SIZE];


/**
  * @brief  ���¹�翪��״̬,��ȡ��翪��״̬����1��Ϊ���ϰ�����0��Ϊ���ϰ�
  * @note   �����ں㶨��������ִ��
  */
void vGetSwitchStatus(void)
{
    Switch[FL] = FL_switch;
    Switch[BL] = BL_switch;
    Switch[FR] = FR_switch;
    Switch[BR] = BR_switch;
    
}


