#ifndef __PATH_TASK_H
#define __PATH_TASK_H

#include "stdint.h"
#include "bsp_motor.h"



#define stop_TIME  10
#define Stop_Disdance_small  80
#define Move_Disdance_small  200
#define Stop_Disdance_big    42  //ÐÞ¸Ä!!!!
#define Stop_Disdance_big0    42 
#define Move_Disdance_big    80
void Path_Task(void);
void Path_Zero(void);
void Path_One(void);
void Path_Two(void);
void Path_Three(void);
void Path_Four(void);
void Path_Five(void);
void Path_Six(void);
void Path_Seven(void);
void Path_Eight(void);
void Path_Night(void);
void Path_Ten(void);
void Path_Eleven(void);
void Path_Twelve(void);
void Grass_Path_Check(void);
void Path_Thirteen(void);
uint8_t Turn(int16_t target_angle , uint16_t time);

extern uint8_t path_flag;
#endif
