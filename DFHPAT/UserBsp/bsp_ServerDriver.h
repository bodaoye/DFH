#ifndef __BSP__SERVERDRIVER__H__
#define __BSP__SERVERDRIVER__H__

#include "stm32f4xx.h"
#include "usart.h"

#define FRAME_HEADER 0x55             //帧头
#define CMD_SERVO_MOVE 0x03           //舵机移动指令
#define CMD_ACTION_GROUP_RUN 0x06     //运动动作组指令
#define CMD_ACTION_GROUP_STOP 0x07    //停止运动组指令
#define CMD_ACTION_GROUP_SPEED 0x0B   //设置动作组运行速度
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //获取电池电压指令
void singleServerControl( uint8_t servoID, uint16_t Position, uint16_t Time );
void moveServos(uint8_t Num, uint16_t Time, ...);
#endif
