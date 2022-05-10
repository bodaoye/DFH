#ifndef __GLOBAL_DEF_H
#define __GLOBAL_DEF_H

/*È«¾Ö¿ØÖÆID*/
typedef enum
{
    // ×óÇ° ÓÒÇ° Ó'/Òºó ×óºó
    BR  = 0,
    BL  = 1,
    FR  = 2,
    FL  = 3,
    HH  = 4,
    // ×óÇ° ×óºó
    JG_F = 0,
    JG_B = 1,
    // Á¬Ðø,Ë®Æ½
    CON = 1,
    HOR = 2,

} eId_t;

/*µ±Ç°ÈÎÎñ×´Ì¬Ã¶¾ÙÀàÐÍ*/
typedef enum
{
    READY = 0,
    RUN,
    FINISH
} eStatus_t;

#endif

