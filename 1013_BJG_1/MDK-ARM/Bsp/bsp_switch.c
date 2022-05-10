/**
  *************************************************************************
  * @brief  NPN NO 型光电开关驱动
  * @author ZZJ
  * @date   2020/9/13
  * @note   有障碍物时输出高电平。
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

/*开关状态数组*/
uint8_t Switch[SWITCH_SIZE];


/**
  * @brief  更新光电开关状态,获取光电开关状态，【1】为有障碍，【0】为无障碍
  * @note   在周期恒定的任务中执行
  */
void vGetSwitchStatus(void)
{
    Switch[FL] = FL_switch;
    Switch[BL] = BL_switch;
    Switch[FR] = FR_switch;
    Switch[BR] = BR_switch;
    
}


