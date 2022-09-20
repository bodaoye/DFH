#ifndef _MATH_CALCU_H_
#define _MATH_CALCU_H_

#include "math.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#define N2 100
#define e  2.718282f
#define SIGMOID_PERIOD 0.133333f
#define SIGMOID_MAX    10

/* 圆周率 */
#ifndef PI
    #define PI 3.14159265358979323846f
#endif

/* 符号 */
#define SIGN(x) ( (x) > 0? (1): ( (x) < 0? (-1): 0 ) )

#define ABS(x)		((x>0)? (x): (-x))

typedef struct
{
//	float value;
//	float inital;
	float change_scale;
	float real_target;
	float limit_target;
	TickType_t ticks;
	TickType_t last_ticks;
//	TicksTypedef *SlopeTick;
}Slope_Struct;

typedef __packed struct
{
    float input;        //输入数据
    float out;          //滤波输出的数据
    float num[1];       //滤波参数
    float frame_period; //滤波的时间间隔 单位 s
} first_order_filter_type_t;

typedef struct
{
	float input;        //输入数据
	float out;          //输出数据
	float min_value;    //限幅最小值
	float max_value;    //限幅最大值
	float frame_period; //时间间隔
} ramp_function_source_t;

/* 数据 NAN 处理 */
#define NAN_PROCESS(now, last)      \
    do {                            \
        if (isnan(now)) {           \
            (now) = (last);         \
        } else {                    \
            (last) = (now);         \
        }                           \
    } while (0)

/* 符号 */
#define SIGN(x) ( (x) > 0? (1): ( (x) < 0? (-1): 0 ) )

/* 输出限幅 */
#define Output_Limit(output,max,min) \
    ( (output) <= (max) && (output)>=(min)? output: \
    ( (output) > (max)? (output = max):(output = min)) )

void 	ramp_calc(ramp_function_source_t *ramp_source_type, float frame_period, float input, float max, float min);
void 	Slope_On(Slope_Struct *V);
void 	Bubble_Sort(int *a,uint8_t n);
float circle_error(float *set ,float *get ,float circle_para);
void 	abs_limit(float *a, float ABS_MAX,float offset);
float data_limit(float data, float max, float min);
float GildeAverageValueFilter(float NewValue, float *Data);
void least_square_linear_fit(float x[], float y[], const int num, float* a, float* b);
#endif

