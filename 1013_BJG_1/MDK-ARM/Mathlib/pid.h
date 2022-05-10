
#ifndef __pid_H
#define __pid_H

#ifdef  __PID_GLOBALS
#define __PID_EXT
#else
#define __PID_EXT extern
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

#define ABS(x)		((x>0)? (x): (-x)) 

enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    
    POSITION_PID,
    DELTA_PID,
};
typedef struct __pid_t
{
    float p;
    float i;
    float d;
    
    float set[3];				//目标值,包含NOW， LAST， LLAST上上次
    float get[3];				//测量值
    float err[3];				//误差
	
    
    float pout;							//p输出
    float iout;							//i输出
    float dout;							//d输出
    
    float pos_out;						//本次位置式输出
    float last_pos_out;				//上次输出
    float delta_u;						//本次增量值
    float delta_out;					//本次增量式输出 = last_delta_out + delta_u
    float last_delta_out;
    
	  float max_err;
	  float deadband;				//err < deadband return
    uint32_t pid_mode;
    uint32_t MaxOutput;				//输出限幅
    uint32_t IntegralLimit;		//积分限幅
    
    void (*f_param_init)(struct __pid_t *pid,  //PID参数初始化
                    uint32_t pid_mode,
                    uint32_t maxOutput,
                    uint32_t integralLimit,
                    float p,
                    float i,
                    float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid三个参数修改

}pid_t;

void PID_struct_init(
    pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    
    float 	kp, 
    float 	ki, 
    float 	kd);

		typedef struct
{
    float input;        //输入数据
    float out;          //输出数据
    float min_value;    //限幅最小值
    float max_value;    //限幅最大值
    float frame_period; //时间间隔
} ramp_function_source_t;

float pid_calc(pid_t* pid, float fdb, float ref);
void  abs_limit(float *a, float ABS_MAX,float offset);  
float circle_error(float set ,float get ,float circle_para);//环形数据处理
float ramp_calc(ramp_function_source_t *ramp_source_type, float frame_period, float input, float max, float min);//斜坡函数

extern pid_t motorBR_speed_pid;extern pid_t motorBL_speed_pid;extern pid_t motorFR_speed_pid;extern pid_t motorFL_speed_pid; 
extern pid_t wall_V_pid;					//openmv传递的x坐标,需要定在某个区间之间
extern pid_t wall_L_pid;					//左小激光贴墙PID
extern pid_t gimbal_position_pid;
extern pid_t gimbal_speed_pid;
extern pid_t straight_pid;		        	//直线PID  陀螺仪保持车头直
extern pid_t stop_pid; 						//停止PID
extern pid_t turn_pid;						//转弯PID
extern pid_t gimbal_motor_pid;
extern pid_t vision_aim_x;
extern pid_t vision_aim_y;
#endif


