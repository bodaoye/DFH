#include "path_task.h"
#include "chassis_task.h"
#include "pid.h"
#include "bsp_sjg.h"
#include "bsp_gyro.h"
#include "bsp_switch.h"
#include "vision_task.h"
#define PATH_MODE_TURN 			//宏定义为转弯
//#define PATH_MODE_PY				//宏定义为平移
#define GET_RIGHT_ITR  (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
#define PATH0_CHECK		 (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
#define PATH1_CHECK		 (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14))
#define PATH2_CHECK		 (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12))
#define PATH3_CHECK		 (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_10))
#define PATH4_CHECK		 (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_8))
#define PATHALL_CHECK	 (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2))
#define FL 0
#define BL 1
#define BR  2
#define FR  3

#define extend 0
#define back 1
uint8_t path_flag = 0;		//路径标志位	
uint8_t test_flag = 0;

uint8_t grass_path[6]={0};//拨码开关，标志哪条垄道有补苗位
void Path_Task(void)					//循历垄道任务，改变底盘模式
{
  test_flag = path_flag + 1;
	vGetSwitchStatus();	//光电开关，1为有障碍 0为无障碍
	switch(test_flag)
	{
		case 1:	Path_One();		      break;			//此函数在 点1   正转90度 转成   90度 后结束 
		case 2:	Path_Two();			    break;			//此函数在 点2   正转90度 转成  180度 后结束 （低速前进）
		case 3:	Path_Three();		    break;			//此函数在 点3   正转90度 转成  -90度 后结束   
		case 4:	Path_Four();		    break;			//此函数在 点4   正转90度 转成    0度 后结束 （低速后退）
		case 5:	Path_Five();		    break;			//此函数在 点5   反转90度 转成  -90度 后结束 （倒车） 
		case 6:	Path_Six();		      break;			//此函数在 点6   反转90度 转成 -180度 后结束 （低速前进）
		case 7:	Path_Seven();		    break; 			//此函数在 点7   正转90度 转成  -90度 后结束 
		case 8:	Path_Eight();		    break;			//此函数在 点8   正转90度 转成    0度 后结束 （低速后腿）
		case 9:	Path_Night();		    break;			//此函数在 点9   反转90度 转成  -90度 后结束 （倒车）
		case 10:Path_Ten();		      break;			//此函数在 点10  反转90度 转成 -180度 后结束 
		case 11:Path_Eleven();	    break;			//此函数在 点11  正转90度 转成  -90度 后结束 
		case 12:Path_Twelve();	    break;			//此函数在 点12  反转90度 转成 -180度 后结束 
//		case 13:Path_Thirteen();    break;

	}
}

//void Grass_Path_Check()  //作弊拨杆
//{
//	grass_path[0]=PATH0_CHECK;
//	grass_path[1]=PATH1_CHECK;
//	grass_path[2]=PATH2_CHECK;
//	grass_path[3]=PATH3_CHECK;
//	grass_path[4]=PATH4_CHECK;
//	grass_path[5]=PATHALL_CHECK;
//}
#if 0//旋转施肥跑法
void Path_One(void)
{
	static uint8_t flag=0;
  
    if( flag==0 &&Switch[FR]==1 && Switch[BR]==1) 
    {
        flag = 1; //判断已经进入第一路径  
    }
    if(flag==1 && JG[0] <= Stop_Disdance_big &&  Switch[FR]==0 && Switch[BR]==0 )	//判断已经到达第一点
    {
        flag =2;
    }
    if(flag==2)
    {
        if(Turn(-90,stop_TIME))		//正转90度，车小停一下（后面看看0.1s有没有更稳定）
        {
            path_flag = 1;
            switch_mode(Y_Mode1);
        }
    }
}
void Path_Two(void)
{	
    static uint8_t flag=0;
    if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	//判断已经进入第二路径
    if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//判断已经到达第二点
    {
        flag=2;
    }
    if(flag==2)
    {
        if(Turn(-90,stop_TIME))			//正转90度，车小停一下
        {
            path_flag = 2;
            switch_mode(Y_Mode3);  //切换为向前低速直行模式
        }
    }
}

void Path_Three(void)
{
	static uint8_t flag=1;
//  if(flag==0 && JG[0] > Move_Disdance_big ) flag=1; //判断已经进入第三路径
	if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)
	{
		flag=2;
	}
	if(flag == 2 && Switch[FR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[BR]==0) //判断已经到达第三点
	{
		flag=4;
	}
	if(flag ==4)
	{
        Ramp_Stop(0);
		if(Turn(-90,stop_TIME))		//正转90度，车小停一下
		{
			path_flag = 3;
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode1);//切换为视觉向前施肥模式
		}  
	}
}
void Path_Four(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//判断已经转弯完毕
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //判断已经进入第四路径
	if(flag==2 && JG[0] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//判断已经到达第四点
	{
		flag=3;
	}
	if(flag==3)
	{
        switch_mode(Protect_Mode);//看看这里会不会翻车，会的话需要加一句斜坡刹车
        Gimbal_Work(back);
        if(Turn(-90,stop_TIME)) //正转90度，车小停一下
        {
            switch_mode(Y_Mode4);//切换为向后低速直行模式
            path_flag = 4;
        }
	}
}
void Path_Five(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //判断第四点转弯完毕
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //判断已经进入第五路径
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[FR]==0)                         //判断已经到达第五点
	{
		flag=4;
	}
	if(flag == 4)
	{
        Ramp_Stop(1); 
		if(Turn(90,stop_TIME))		        //反转90度，车小停一下
		{
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode2);          //切换为视觉向后施肥直行模式
			path_flag = 5;
		}
	}
}

void Path_Six(void)
{
	static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //判断已经进入第六路径
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//判断已经到达第六点
	{
		flag=2;
	}
	if(flag==2)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(90,stop_TIME))                         //反转90度，车小停一下
		{
            switch_mode(Y_Mode3);//切换为向前低速直行模式
			path_flag = 6;

		}
	}
}

void Path_Seven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //判断第六点转弯完毕
    if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)   //判断已经进入第七路径
    {
        flag=2;
    }
    if(flag == 2 && Switch[FR]==0)
    {
        flag=3;
    }
    if(flag == 3 && Switch[BR]==0) //判断已经到达第七点
    {
        flag=4;
    }
    if(flag == 4)
    {
        Ramp_Stop(1);    
        if(Turn(-90,stop_TIME))		//正转90度，车小停一下
        {     
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode1);//切换为视觉向前施肥模式
            path_flag = 7;
        }
	}
}

void Path_Eight(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//判断已经转弯完毕
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //判断已经进入第八路径
	if(flag==2 && JG[0] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//判断已经到达第八点
	{
		flag=3;
	}
	if(flag==3)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(-90,stop_TIME))			//正转90度，车小停一下
		{
            switch_mode(Y_Mode4);//切换为向后低速直行模式
			path_flag = 8;
		}
	}
}

void Path_Night(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //判断第八点转弯完毕
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //判断已经进入第九路径
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[FR]==0)                         //判断已经到达第九点
	{
		flag=4;
	}
	if(flag == 4)
    {
        Ramp_Stop(1);   
		if(Turn(90,stop_TIME))		//反转90度，车小停一下
		{
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode2);//切换为视觉向后施肥模式
			path_flag = 9;
		}
    }
}

void Path_Ten(void)
{
  static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //判断已经进入第十路径
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//判断已经到达第十点
	{
		flag=2;
	}
	if(flag==2)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(90,stop_TIME))			//反转90度，车小停一下
		{
            switch_mode(Exit_Mode); //切换为出垄模式
			path_flag = 10;
		}
	}
}

void Path_Eleven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[3] > Move_Disdance_big)			//判断已转弯
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0) //判断已经到达第十一点
	{
		flag=2;
	}
	if(flag==2)
	{ 
		if(Turn(-90,stop_TIME))    //正转90度，车小停一下
		{
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode1);//切换为视觉向前施肥模式
			path_flag = 11;
		}
	}
}
void Path_Twelve(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] >= Move_Disdance_big)	//判断已转弯
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)//判断已经到达第十二点
	{
		flag=2;
	}
	if(flag==2)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(-90,stop_TIME))   //正转90度，车小停一下
		{
//			switch_mode(Y_Mode1); //切换为向前直行模式
            switch_mode(Protect_Mode);
			path_flag = 12;
		}
	}
}

//void Path_Thirteen(void)
//{
//	static uint16_t i;
//	if(JG[0] >= Move_Disdance_big)
//	{
//		i++;
//		if(i >= 150)	
//		{
//			switch_mode(Protect_Mode);
//			path_flag = 13;
//		}
//	}
//}


uint8_t Turn(int16_t target_angle , uint16_t time)
{
	static uint8_t flag=0;
	if(flag==0)
	{
		switch_mode(Protect_Mode);              //转弯前的制动，且短暂stop，以提高稳定性。
		stop_time=0;
		flag=1;
	}
	if(flag == 1 && stop_time >= time)       //当运行保护模式的时间 大于等于 设置的停止时间是，开始转弯
	{
 		angle_set += target_angle;
        if(angle_set>=180)angle_set-=360;
        if(angle_set<=-180)angle_set+=360;
		angle_flag = 0;                        //标志着开始转弯
		switch_mode(Turn_Mode);
		flag=2;                                //标志着转弯完毕
	}
	if(flag == 2 && ABS(Angle_Error_WT)<= 3.0f && angle_flag ==1)  //转弯检验，如果转弯完毕后角度误差在1°以内，清零flag且跳出函数。
	{
		flag=0;
		return 1;
	}	
	return 0;
}
#endif
#if 1//旋转不施肥跑法
void Path_One(void)
{
	
    static uint8_t flag=0;
  
 if( flag==0 &&Switch[FR]==1 && Switch[BR]==1) 
  {
    flag = 1; //判断已经进入第一路径  
  }
	if(flag==1 && JG[0] <= Stop_Disdance_big &&  Switch[FR]==0 && Switch[BR]==0 )	//判断已经到达第一点
	{
		flag =2;
	}
	if(flag==2)
	{
		if(Turn(-90,stop_TIME))		//正转90度，车小停一下（后面看看0.1s有没有更稳定）
		{
			path_flag = 1;
      switch_mode(Y_Mode1);
		}
	}
}
void Path_Two(void)
{	
    static uint8_t flag=0;
    if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	//判断已经进入第二路径
    if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//判断已经到达第二点
    {
        flag=2;
    }
    if(flag==2)
    {
        if(Turn(-90,stop_TIME))			//正转90度，车小停一下
        {
            path_flag = 2;
            switch_mode(Y_Mode3);  //切换为向前低速直行模式
        }
    }
}

void Path_Three(void)
{
	static uint8_t flag=1;
//  if(flag==0 && JG[0] > Move_Disdance_big ) flag=1; //判断已经进入第三路径
	if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)
	{
		flag=2;
	}
	if(flag == 2 && Switch[FR]==0)
	{
		flag=3;
	}
  if(flag == 3 && Switch[BR]==0) //判断已经到达第三点
	{
		flag=4;
	}
	if(flag ==4)
	{
        Ramp_Stop(0);
		if(Turn(-90,stop_TIME))		//正转90度，车小停一下
		{
			path_flag = 3;
            switch_mode(Y_Mode1);//切换为进垄模式
		}  
	}
}
void Path_Four(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//判断已经转弯完毕
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //判断已经进入第四路径
	if(flag==2 && JG[0] <= Stop_Disdance_big )	//判断已经到达第四点
	{
        Ramp_Stop(0);
		flag=3;
	}
	if(flag==3&& Switch[FR]==0 && Switch[BR]==0)
	{
		if(Turn(-90,stop_TIME)) //正转90度，车小停一下
		{
            switch_mode(Y_Mode4);//切换为向后低速直行模式
			path_flag = 4;
		}
	}
}
void Path_Five(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //判断第四点转弯完毕
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //判断已经进入第五路径
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
  if(flag == 3 && Switch[FR]==0)                         //判断已经到达第五点
	{
		flag=4;
	}
	if(flag == 4)
	{
        Ramp_Stop(1); 
		if(Turn(90,stop_TIME))		                          //反转90度，车小停一下
		{
            switch_mode(Y_Mode2);                            //切换为向后直行模式
			path_flag = 5;
		}
	}
}

void Path_Six(void)
{
	static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //判断已经进入第六路径
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//判断已经到达第六点
	{
		flag=2;
	}
	if(flag==2)
	{
		if(Turn(90,stop_TIME))                         //反转90度，车小停一下
		{
            switch_mode(Y_Mode3);//切换为向前低速直行模式
//            switch_mode(Protect_Mode);  
			path_flag = 6;

		}
	}
}


void Path_Seven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //判断第六点转弯完毕
    if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)   //判断已经进入第七路径
    {
        flag=2;
    }
    if(flag == 2 && Switch[FR]==0)
    {
        flag=3;
    }
    if(flag == 3 && Switch[BR]==0) //判断已经到达第七点
    {
        flag=4;
    }
    if(flag == 4)
    {
        Ramp_Stop(1);    
        if(Turn(-90,stop_TIME))		//正转90度，车小停一下
        {
            switch_mode(Y_Mode1); //切换为进垄模式
            path_flag = 7;
        }
	}
}

void Path_Eight(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//判断已经转弯完毕
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //判断已经进入第八路径
	if(flag==2 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 &&Switch[BR]==0)	//判断已经到达第八点
	{
		flag=3;
	}
	if(flag==3)
	{
		if(Turn(-90,stop_TIME))			//正转90度，车小停一下
		{
            switch_mode(Y_Mode4);//切换为向后低速直行模式
			path_flag = 8;
		}
	}
}

void Path_Night(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //判断第八点转弯完毕
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //判断已经进入第九路径
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[FR]==0)                         //判断已经到达第九点
	{
		flag=4;
	}
	if(flag == 4)
    {
        Ramp_Stop(1);   
		if(Turn(90,stop_TIME))		//反转90度，车小停一下
		{
            switch_mode(Y_Mode2);//切换为向后直行模式
			path_flag = 9;
		}
  }
}

void Path_Ten(void)
{
  static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //判断已经进入第十路径
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//判断已经到达第十点
	{
		flag=2;
	}
	if(flag==2)
	{
		if(Turn(90,stop_TIME))			//反转90度，车小停一下
		{
            switch_mode(Exit_Mode); //切换为出垄模式
			path_flag = 10;
		}
	}
}

void Path_Eleven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[3]  && JG[0] >= Stop_Disdance_big)			//判断已转弯
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0) //判断已经到达第十一点
	{
		flag=2;
	}
	if(flag==2)
	{ 
		if(Turn(-90,stop_TIME))    //正转90度，车小停一下
		{
            switch_mode(Y_Mode1);   //切换为向前直行模式
			path_flag = 11;
		}
	}
}
void Path_Twelve(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] >= Move_Disdance_big)	//判断已转弯
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)//判断已经到达第十二点
	{
		flag=2;
	}
	if(flag==2)
	{
		if(Turn(-90,stop_TIME))   //正转90度，车小停一下
		{
//			switch_mode(Y_Mode1); //切换为向前直行模式
            switch_mode(Protect_Mode);  
			path_flag = 12;
		}
	}
}

//void Path_Thirteen(void)
//{
//	static uint16_t i;
//	if(JG[0] >= Move_Disdance_big)
//	{
//		i++;
//		if(i >= 150)	
//		{
//			switch_mode(Protect_Mode);
//			path_flag = 13;
//		}
//	}
//}


uint8_t Turn(int16_t target_angle , uint16_t time)
{
	static uint8_t flag=0;
	if(flag==0)
	{
		switch_mode(Protect_Mode);              //转弯前的制动，且短暂stop，以提高稳定性。
		stop_time=0;
		flag=1;//进不来
	}
	if(flag == 1 && stop_time >= time)       //当运行保护模式的时间 大于等于 设置的停止时间是，开始转弯
	{
 		angle_set += target_angle;
        if(angle_set>=180)angle_set-=360;
        if(angle_set<=-180)angle_set+=360;
		angle_flag = 0;                        //标志着开始转弯
		switch_mode(Turn_Mode);
		flag=2;                                //标志着转弯完毕
	}
	if(flag == 2 && ABS(Angle_Error_WT)<= 3.0f && angle_flag ==1)  //转弯检验，如果转弯完毕后角度误差在1°以内，清零flag且跳出函数。
	{
		flag=0;
		return 1;
	}	
	return 0;
}
#endif

