/**
  *************************************************************************
  * @brief  WT53TR 维特智能小激光
  * @author JFG
  * @date   2022年9月13日15:29:15
  * @note   
  *************************************************************************
  */
#include "bsp_WT53R.h"
#include "math.h"
uint16_t wt53r_distance[2] = {0};

/**
  * @brief  tof 回传数据解算
  * @param  uint8_t *buff 串口缓存区指针
  * @retval 	1. 成功返回1，失败返回0；
				2. 距离信息cm
  */
void getWT53Rdis(uint8_t *buff,uint8_t num) {
  uint8_t chBuff[4] = {1},numbuff[4] = {0},cnt = 0;/* 字符缓存和数字缓存 */
  uint16_t distance = 0;
	/* 取出该四字节 */
  for(int i = 0; i < 4; i++) {
		chBuff[i] = buff[num + i];
  }
  /* 开始转化 */
  for(int j = 0; j < 4; j++) {
	if( (chBuff[j]>='0') && (chBuff[j]<='9') ) {
	  cnt += 1;/* 计数位自增，表示数字的位数 */
	  numbuff[j] = chBuff[j] - 48;/*转化为0~9的十进制数存入数组中*/
	 } else {
	  numbuff[j] = 0;
	 }
  }
  /*根据数字的位数将数组内数字转化为整数*/
  while(cnt!=0) {
  distance += (numbuff[4 - cnt] * (pow(10,cnt-1)));
  cnt--;
  }
  wt53r_distance[0] = distance;
}
