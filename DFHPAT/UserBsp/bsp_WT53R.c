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
  * @retval 距离信息mm
  */
void getWT53Rdis(uint8_t *buff,uint8_t num) {
  uint8_t chBuff[4] = {1},numbuff[4] = {0},cnt = 0;/* 字符缓存和数字缓存 */
  uint16_t distance = 0;
	/* 取出该四字节 */
  for(int i = 0; i < 4; i++) {
		chBuff[i] = buff[num + i];
  } //数组下表有时包含着美妙的巧合，有时候又有着很棘手的问题。
  /* 开始转化 */
  for(int j = 0; j < 4; j++) {
	if( (chBuff[j]>='0') && (chBuff[j]<='9') ) {
	  cnt += 1;/* 计数位自增，表示数字的位数 */
	  numbuff[j] = chBuff[j] - 48;/*转化为0~9的十进制数存入数组中*/	//48是一个神奇的数字
	 } else {
	  numbuff[j] = 0;
	 }
  }
  /*根据数字的位数将数组内数字转化为整数*/
  while(cnt!=0) {
    distance += (numbuff[4 - cnt] * (pow(10,cnt-1)));//数组下表有时包含着美妙的巧合，有时候又有着很棘手的问题。
    cnt--;//时刻注意偏移量和实际位置的差异
  }
  wt53r_distance[0] = distance;
}

void Tof_Read_Data(uint8_t *Tof_Data)
{
 uint16_t tof_dist=0;
 for(int i=28;i<40;i++)
 {
  if(Tof_Data[i]>=48&&Tof_Data[i]<=57)
  {
   tof_dist=tof_dist*10+(Tof_Data[i]-48);
  }   
 }
 wt53r_distance[0]=tof_dist;
}