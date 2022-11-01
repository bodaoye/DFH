#include "debug_task.h"
#include "bsp_debug.h"
#include "cmsis_os.h"
#include "bsp_debug.h"
#include "chassis_task.h"
uint8_t debug_wave = 6; /* 用以选择打印的数据类型 */
//弱函数的强定义
void DataWavePkg(void) {
    /* 视觉调试专用部分  */
    switch (debug_wave) {              //状态切换 
      case 0:{
        DataScope_Get_Channel_Data(chassisCurrentState);//当前状态
         DataScope_Get_Channel_Data(chassisNextAction);//下一实可指向的状态
        DataScope_Get_Channel_Data(chassisSenorData.headBigLazer_kal);//大激光读数
        DataScope_Get_Channel_Data(chassisState.currentAngle);
       break;
      }
      case 1: {//内环
        DataScope_Get_Channel_Data(chassis.wheel_spd_ref[0]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_fdb[0]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_ref[1]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_fdb[1]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_ref[2]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_fdb[2]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_ref[3]);
        DataScope_Get_Channel_Data(chassis.wheel_spd_fdb[3]);
        break;
      }
      case 2: {
        DataScope_Get_Channel_Data(chassis.chassis_angle_ref);
        DataScope_Get_Channel_Data(chassis.chassis_angle_fdb);
      }
      case 3: {
        DataScope_Get_Channel_Data(chassis.chassis_stickdis_ref);
        DataScope_Get_Channel_Data(chassis.chassis_stickdis_fdb);
      }
      case 4: {
        DataScope_Get_Channel_Data(chassisSenorData.leftSmallLazer);
        DataScope_Get_Channel_Data(chassisSenorData.headBigLazer_kal);
        DataScope_Get_Channel_Data(chassisSenorData.imuYaw);
       }
      case 5: {
         DataScope_Get_Channel_Data(chassis.wheel_spd_fdb[0]);
         DataScope_Get_Channel_Data(chassisSenorData.headBigLazer_kal);
       }
      case 6: {
        DataScope_Get_Channel_Data(chassisSenorData.leftHeadKey);
        DataScope_Get_Channel_Data(chassisSenorData.leftTailKey );
        DataScope_Get_Channel_Data(findFlagTest);
       }
      
  }
  
}

void debug_task(void const *argu) {
    uint32_t thread_wake_time = osKernelSysTick();
    for (;;) {
        DataWave(&huart2);	
        osDelayUntil(&thread_wake_time, 1);
//   singleServerControl(0x03, 180, 3000);
//    DataWave(&huart4);
     osDelayUntil(&thread_wake_time, 1);
    }
}

