#ifndef __ID_H
#define __ID_H

/*ȫ�ֿ���ID*/
typedef enum
{
    // ��ǰ ��ǰ �Һ� ���
    LF  = 0,
    RF  = 1,
    RB  = 2,
    LB  = 3,
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

