
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
    
    float set[3];				//Ŀ��ֵ,����NOW�� LAST�� LLAST���ϴ�
    float get[3];				//����ֵ
    float err[3];				//���
	
    
    float pout;							//p���
    float iout;							//i���
    float dout;							//d���
    
    float pos_out;						//����λ��ʽ���
    float last_pos_out;				//�ϴ����
    float delta_u;						//��������ֵ
    float delta_out;					//��������ʽ��� = last_delta_out + delta_u
    float last_delta_out;
    
	  float max_err;
	  float deadband;				//err < deadband return
    uint32_t pid_mode;
    uint32_t MaxOutput;				//����޷�
    uint32_t IntegralLimit;		//�����޷�
    
    void (*f_param_init)(struct __pid_t *pid,  //PID������ʼ��
                    uint32_t pid_mode,
                    uint32_t maxOutput,
                    uint32_t integralLimit,
                    float p,
                    float i,
                    float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid���������޸�

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
    float input;        //��������
    float out;          //�������
    float min_value;    //�޷���Сֵ
    float max_value;    //�޷����ֵ
    float frame_period; //ʱ����
} ramp_function_source_t;

float pid_calc(pid_t* pid, float fdb, float ref);
void  abs_limit(float *a, float ABS_MAX,float offset);  
float circle_error(float set ,float get ,float circle_para);//�������ݴ���
float ramp_calc(ramp_function_source_t *ramp_source_type, float frame_period, float input, float max, float min);//б�º���

extern pid_t motorBR_speed_pid;extern pid_t motorBL_speed_pid;extern pid_t motorFR_speed_pid;extern pid_t motorFL_speed_pid; 
extern pid_t wall_V_pid;					//openmv���ݵ�x����,��Ҫ����ĳ������֮��
extern pid_t wall_L_pid;					//��С������ǽPID
extern pid_t gimbal_position_pid;
extern pid_t gimbal_speed_pid;
extern pid_t straight_pid;		        	//ֱ��PID  �����Ǳ��ֳ�ͷֱ
extern pid_t stop_pid; 						//ֹͣPID
extern pid_t turn_pid;						//ת��PID
extern pid_t gimbal_motor_pid;
extern pid_t vision_aim_x;
extern pid_t vision_aim_y;
#endif


