/**
  ***********************************************
  * @brief  幻尔舵机驱动板底层配置与驱动接口
  * @author 冀风光
  * @date   2022年9月13日10:45:58
  * @note   
  ***********************************************
  */
  
#include "bsp_ServerDriver.h"
#include <stdarg.h>
#include <string.h>

#define 	Servo_Control_Msg_Tx_Buf_Len 	9
//宏函数 获得A的低八位
#define GET_LOW_BYTE(A) ((uint8_t)(A))
//宏函数 获得A的高八位
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))


uint8_t LobotTxBuf[128];  //发送缓存
uint8_t LobotRxBuf[16];

/* 单个舵机控制接口 */
void singleServerControl( uint8_t servoID, uint16_t Position, uint16_t Time ) {
	
	if (servoID > 31 || !(Time > 0)) {  //舵机ID不能打于31,可根据对应控制板修改
		return;
	}
	/* 打包通信协议 */
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;    //填充帧头
	LobotTxBuf[2] = 8;
	LobotTxBuf[3] = CMD_SERVO_MOVE;           //数据长度=要控制舵机数*3+5，此处=1*3+5//填充舵机移动指令
	LobotTxBuf[4] = 1;                        //要控制的舵机个数
	LobotTxBuf[5] = GET_LOW_BYTE(Time);       //取得时间的低八位
	LobotTxBuf[6] = GET_HIGH_BYTE(Time);      //取得时间的高八位
	LobotTxBuf[7] = servoID;                  //舵机ID
	LobotTxBuf[8] = GET_LOW_BYTE(Position);   //取得目标位置的低八位
	LobotTxBuf[9] = GET_HIGH_BYTE(Position);  //取得目标位置的高八位
	
	/* 调用相应串口发送 */
	HAL_UART_Transmit_DMA(&huart4, LobotTxBuf, Servo_Control_Msg_Tx_Buf_Len);
}

/* 多个舵机控制接口 */
void moveServos(uint8_t Num, uint16_t Time, ...)
{
	uint8_t index = 7;
	uint8_t i = 0;
	uint16_t temp;
	va_list arg_ptr;  //

	va_start(arg_ptr, Time); //取得可变参数首地址
	if (Num < 1 || Num > 32) {
		return;               //舵机数不能为零和大与32，时间不能小于0
	}
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;      //填充帧头
	LobotTxBuf[2] = Num * 3 + 5;                //数据长度 = 要控制舵机数 * 3 + 5
	LobotTxBuf[3] = CMD_SERVO_MOVE;             //舵机移动指令
	LobotTxBuf[4] = Num;                        //要控制舵机数
	LobotTxBuf[5] = GET_LOW_BYTE(Time);         //取得时间的低八位
	LobotTxBuf[6] = GET_HIGH_BYTE(Time);        //取得时间的高八位

	for (i = 0; i < Num; i++) {//从可变参数中取得并循环填充舵机ID和对应目标位置
		temp = va_arg(arg_ptr, int);//可参数中取得舵机ID
		LobotTxBuf[index++] = GET_LOW_BYTE(((uint16_t)temp));
		temp = va_arg(arg_ptr, int);  //可变参数中取得对应目标位置
		LobotTxBuf[index++] = GET_LOW_BYTE(((uint16_t)temp)); //填充目标位置低八位
		LobotTxBuf[index++] = GET_HIGH_BYTE(temp);//填充目标位置高八位
	}

	va_end(arg_ptr);  //置空arg_ptr

	HAL_UART_Transmit_DMA(&huart4, LobotTxBuf, Servo_Control_Msg_Tx_Buf_Len);    //发送
}

/* 当前舵机角度读取接口 */
