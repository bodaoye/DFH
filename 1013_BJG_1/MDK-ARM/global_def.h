#ifndef __GLOBAL_DEF_H
#define __GLOBAL_DEF_H

/*全局控制ID*/
typedef enum
{
    // 左前 右前 �'/液� 左后
    BR  = 0,
    BL  = 1,
    FR  = 2,
    FL  = 3,
    HH  = 4,
    // 左前 左后
    JG_F = 0,
    JG_B = 1,
    // 连续,水平
    CON = 1,
    HOR = 2,

} eId_t;

/*当前任务状态枚举类型*/
typedef enum
{
    READY = 0,
    RUN,
    FINISH
} eStatus_t;

#endif

