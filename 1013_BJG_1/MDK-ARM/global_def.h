#ifndef __GLOBAL_DEF_H
#define __GLOBAL_DEF_H

/*ȫ�ֿ���ID*/
typedef enum
{
    // ��ǰ ��ǰ �'/Һ� ���
    BR  = 0,
    BL  = 1,
    FR  = 2,
    FL  = 3,
    HH  = 4,
    // ��ǰ ���
    JG_F = 0,
    JG_B = 1,
    // ����,ˮƽ
    CON = 1,
    HOR = 2,

} eId_t;

/*��ǰ����״̬ö������*/
typedef enum
{
    READY = 0,
    RUN,
    FINISH
} eStatus_t;

#endif

