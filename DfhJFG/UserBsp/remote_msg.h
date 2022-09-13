/** 
  * @file remote_msg.h
  * @version 2.0
  * @date Nov,21th 2020
	*
  * @brief  
	*
  *	@author YY
  *
  */
#ifndef __REMOTE_MSG_H__
#define __REMOTE_MSG_H__

#include "stm32f4xx_hal.h"

#define DBUS_MAX_LEN  50
#define DBUS_BUFLEN   18

/* remote control information structure */
typedef __packed struct
{
  /* rocker channel information */
  int16_t ch1;
  int16_t ch2;
  int16_t ch3;
  int16_t ch4;
	int16_t ch5;
  /* left and right lever information */
  uint8_t sw1;
  uint8_t sw2;
  /* mouse movement and button information */
  __packed struct
  {
    int16_t x;
    int16_t y;
    int16_t z;
  
    uint8_t l;
    uint8_t r;
  } mouse;
  /* keyboard key information */
  __packed union
  {
    uint16_t key_code;
    __packed struct 
    {
      uint16_t W:1;
      uint16_t S:1;
      uint16_t A:1;
      uint16_t D:1;
      uint16_t SHIFT:1;
      uint16_t CTRL:1;
      uint16_t Q:1;
      uint16_t E:1;
      uint16_t R:1;
      uint16_t F:1;
      uint16_t G:1;
      uint16_t Z:1;
      uint16_t X:1;
      uint16_t C:1;
      uint16_t V:1;
      uint16_t B:1;
    } bit;
  } kb;
} rc_info_t;

/* PC_Key_Definition */ 
typedef enum
{
	KB_Q = 0,
	KB_E = 1,
	KB_R = 2,
	KB_F = 3,
	KB_G = 4,
	KB_Z = 5,
	KB_X = 6,
	KB_C = 7,
	KB_V = 8,
	KB_B = 9,
	KB_CTRL = 10,
} key_index_e;

typedef enum
{
  RC_UP = 1,
  RC_MI = 3,
  RC_DN = 2,
} rc_sw_mode_e;

extern rc_info_t  rc;
extern int kb_status[11];

void rc_callback_handler(rc_info_t *rc, uint8_t *buff);
void keyboard_scanf(uint16_t key, key_index_e key_index);

#endif

