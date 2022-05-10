#include "path_task.h"
#include "chassis_task.h"
#include "pid.h"
#include "bsp_sjg.h"
#include "bsp_gyro.h"
#include "bsp_switch.h"
#include "vision_task.h"
#define PATH_MODE_TURN 			//�궨��Ϊת��
//#define PATH_MODE_PY				//�궨��Ϊƽ��
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
uint8_t path_flag = 0;		//·����־λ	
uint8_t test_flag = 0;

uint8_t grass_path[6]={0};//���뿪�أ���־����¢���в���λ
void Path_Task(void)					//ѭ��¢�����񣬸ı����ģʽ
{
  test_flag = path_flag + 1;
	vGetSwitchStatus();	//��翪�أ�1Ϊ���ϰ� 0Ϊ���ϰ�
	switch(test_flag)
	{
		case 1:	Path_One();		      break;			//�˺����� ��1   ��ת90�� ת��   90�� ����� 
		case 2:	Path_Two();			    break;			//�˺����� ��2   ��ת90�� ת��  180�� ����� ������ǰ����
		case 3:	Path_Three();		    break;			//�˺����� ��3   ��ת90�� ת��  -90�� �����   
		case 4:	Path_Four();		    break;			//�˺����� ��4   ��ת90�� ת��    0�� ����� �����ٺ��ˣ�
		case 5:	Path_Five();		    break;			//�˺����� ��5   ��ת90�� ת��  -90�� ����� �������� 
		case 6:	Path_Six();		      break;			//�˺����� ��6   ��ת90�� ת�� -180�� ����� ������ǰ����
		case 7:	Path_Seven();		    break; 			//�˺����� ��7   ��ת90�� ת��  -90�� ����� 
		case 8:	Path_Eight();		    break;			//�˺����� ��8   ��ת90�� ת��    0�� ����� �����ٺ��ȣ�
		case 9:	Path_Night();		    break;			//�˺����� ��9   ��ת90�� ת��  -90�� ����� ��������
		case 10:Path_Ten();		      break;			//�˺����� ��10  ��ת90�� ת�� -180�� ����� 
		case 11:Path_Eleven();	    break;			//�˺����� ��11  ��ת90�� ת��  -90�� ����� 
		case 12:Path_Twelve();	    break;			//�˺����� ��12  ��ת90�� ת�� -180�� ����� 
//		case 13:Path_Thirteen();    break;

	}
}

//void Grass_Path_Check()  //���ײ���
//{
//	grass_path[0]=PATH0_CHECK;
//	grass_path[1]=PATH1_CHECK;
//	grass_path[2]=PATH2_CHECK;
//	grass_path[3]=PATH3_CHECK;
//	grass_path[4]=PATH4_CHECK;
//	grass_path[5]=PATHALL_CHECK;
//}
#if 0//��תʩ���ܷ�
void Path_One(void)
{
	static uint8_t flag=0;
  
    if( flag==0 &&Switch[FR]==1 && Switch[BR]==1) 
    {
        flag = 1; //�ж��Ѿ������һ·��  
    }
    if(flag==1 && JG[0] <= Stop_Disdance_big &&  Switch[FR]==0 && Switch[BR]==0 )	//�ж��Ѿ������һ��
    {
        flag =2;
    }
    if(flag==2)
    {
        if(Turn(-90,stop_TIME))		//��ת90�ȣ���Сͣһ�£����濴��0.1s��û�и��ȶ���
        {
            path_flag = 1;
            switch_mode(Y_Mode1);
        }
    }
}
void Path_Two(void)
{	
    static uint8_t flag=0;
    if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	//�ж��Ѿ�����ڶ�·��
    if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//�ж��Ѿ�����ڶ���
    {
        flag=2;
    }
    if(flag==2)
    {
        if(Turn(-90,stop_TIME))			//��ת90�ȣ���Сͣһ��
        {
            path_flag = 2;
            switch_mode(Y_Mode3);  //�л�Ϊ��ǰ����ֱ��ģʽ
        }
    }
}

void Path_Three(void)
{
	static uint8_t flag=1;
//  if(flag==0 && JG[0] > Move_Disdance_big ) flag=1; //�ж��Ѿ��������·��
	if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)
	{
		flag=2;
	}
	if(flag == 2 && Switch[FR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[BR]==0) //�ж��Ѿ����������
	{
		flag=4;
	}
	if(flag ==4)
	{
        Ramp_Stop(0);
		if(Turn(-90,stop_TIME))		//��ת90�ȣ���Сͣһ��
		{
			path_flag = 3;
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode1);//�л�Ϊ�Ӿ���ǰʩ��ģʽ
		}  
	}
}
void Path_Four(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//�ж��Ѿ�ת�����
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //�ж��Ѿ��������·��
	if(flag==2 && JG[0] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//�ж��Ѿ�������ĵ�
	{
		flag=3;
	}
	if(flag==3)
	{
        switch_mode(Protect_Mode);//��������᲻�ᷭ������Ļ���Ҫ��һ��б��ɲ��
        Gimbal_Work(back);
        if(Turn(-90,stop_TIME)) //��ת90�ȣ���Сͣһ��
        {
            switch_mode(Y_Mode4);//�л�Ϊ������ֱ��ģʽ
            path_flag = 4;
        }
	}
}
void Path_Five(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //�жϵ��ĵ�ת�����
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //�ж��Ѿ��������·��
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[FR]==0)                         //�ж��Ѿ���������
	{
		flag=4;
	}
	if(flag == 4)
	{
        Ramp_Stop(1); 
		if(Turn(90,stop_TIME))		        //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode2);          //�л�Ϊ�Ӿ����ʩ��ֱ��ģʽ
			path_flag = 5;
		}
	}
}

void Path_Six(void)
{
	static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //�ж��Ѿ��������·��
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//�ж��Ѿ����������
	{
		flag=2;
	}
	if(flag==2)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(90,stop_TIME))                         //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode3);//�л�Ϊ��ǰ����ֱ��ģʽ
			path_flag = 6;

		}
	}
}

void Path_Seven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //�жϵ�����ת�����
    if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)   //�ж��Ѿ��������·��
    {
        flag=2;
    }
    if(flag == 2 && Switch[FR]==0)
    {
        flag=3;
    }
    if(flag == 3 && Switch[BR]==0) //�ж��Ѿ�������ߵ�
    {
        flag=4;
    }
    if(flag == 4)
    {
        Ramp_Stop(1);    
        if(Turn(-90,stop_TIME))		//��ת90�ȣ���Сͣһ��
        {     
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode1);//�л�Ϊ�Ӿ���ǰʩ��ģʽ
            path_flag = 7;
        }
	}
}

void Path_Eight(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//�ж��Ѿ�ת�����
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //�ж��Ѿ�����ڰ�·��
	if(flag==2 && JG[0] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//�ж��Ѿ�����ڰ˵�
	{
		flag=3;
	}
	if(flag==3)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(-90,stop_TIME))			//��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode4);//�л�Ϊ������ֱ��ģʽ
			path_flag = 8;
		}
	}
}

void Path_Night(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //�жϵڰ˵�ת�����
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //�ж��Ѿ�����ھ�·��
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[FR]==0)                         //�ж��Ѿ�����ھŵ�
	{
		flag=4;
	}
	if(flag == 4)
    {
        Ramp_Stop(1);   
		if(Turn(90,stop_TIME))		//��ת90�ȣ���Сͣһ��
		{
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode2);//�л�Ϊ�Ӿ����ʩ��ģʽ
			path_flag = 9;
		}
    }
}

void Path_Ten(void)
{
  static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //�ж��Ѿ������ʮ·��
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==Switch[BR]==0)	//�ж��Ѿ������ʮ��
	{
		flag=2;
	}
	if(flag==2)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(90,stop_TIME))			//��ת90�ȣ���Сͣһ��
		{
            switch_mode(Exit_Mode); //�л�Ϊ��¢ģʽ
			path_flag = 10;
		}
	}
}

void Path_Eleven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[3] > Move_Disdance_big)			//�ж���ת��
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0) //�ж��Ѿ������ʮһ��
	{
		flag=2;
	}
	if(flag==2)
	{ 
		if(Turn(-90,stop_TIME))    //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Protect_Mode);
            Gimbal_Work(extend);
            switch_mode(Vision_Mode1);//�л�Ϊ�Ӿ���ǰʩ��ģʽ
			path_flag = 11;
		}
	}
}
void Path_Twelve(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] >= Move_Disdance_big)	//�ж���ת��
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)//�ж��Ѿ������ʮ����
	{
		flag=2;
	}
	if(flag==2)
	{
        switch_mode(Protect_Mode);
        Gimbal_Work(back);
		if(Turn(-90,stop_TIME))   //��ת90�ȣ���Сͣһ��
		{
//			switch_mode(Y_Mode1); //�л�Ϊ��ǰֱ��ģʽ
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
		switch_mode(Protect_Mode);              //ת��ǰ���ƶ����Ҷ���stop��������ȶ��ԡ�
		stop_time=0;
		flag=1;
	}
	if(flag == 1 && stop_time >= time)       //�����б���ģʽ��ʱ�� ���ڵ��� ���õ�ֹͣʱ���ǣ���ʼת��
	{
 		angle_set += target_angle;
        if(angle_set>=180)angle_set-=360;
        if(angle_set<=-180)angle_set+=360;
		angle_flag = 0;                        //��־�ſ�ʼת��
		switch_mode(Turn_Mode);
		flag=2;                                //��־��ת�����
	}
	if(flag == 2 && ABS(Angle_Error_WT)<= 3.0f && angle_flag ==1)  //ת����飬���ת����Ϻ�Ƕ������1�����ڣ�����flag������������
	{
		flag=0;
		return 1;
	}	
	return 0;
}
#endif
#if 1//��ת��ʩ���ܷ�
void Path_One(void)
{
	
    static uint8_t flag=0;
  
 if( flag==0 &&Switch[FR]==1 && Switch[BR]==1) 
  {
    flag = 1; //�ж��Ѿ������һ·��  
  }
	if(flag==1 && JG[0] <= Stop_Disdance_big &&  Switch[FR]==0 && Switch[BR]==0 )	//�ж��Ѿ������һ��
	{
		flag =2;
	}
	if(flag==2)
	{
		if(Turn(-90,stop_TIME))		//��ת90�ȣ���Сͣһ�£����濴��0.1s��û�и��ȶ���
		{
			path_flag = 1;
      switch_mode(Y_Mode1);
		}
	}
}
void Path_Two(void)
{	
    static uint8_t flag=0;
    if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	//�ж��Ѿ�����ڶ�·��
    if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//�ж��Ѿ�����ڶ���
    {
        flag=2;
    }
    if(flag==2)
    {
        if(Turn(-90,stop_TIME))			//��ת90�ȣ���Сͣһ��
        {
            path_flag = 2;
            switch_mode(Y_Mode3);  //�л�Ϊ��ǰ����ֱ��ģʽ
        }
    }
}

void Path_Three(void)
{
	static uint8_t flag=1;
//  if(flag==0 && JG[0] > Move_Disdance_big ) flag=1; //�ж��Ѿ��������·��
	if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)
	{
		flag=2;
	}
	if(flag == 2 && Switch[FR]==0)
	{
		flag=3;
	}
  if(flag == 3 && Switch[BR]==0) //�ж��Ѿ����������
	{
		flag=4;
	}
	if(flag ==4)
	{
        Ramp_Stop(0);
		if(Turn(-90,stop_TIME))		//��ת90�ȣ���Сͣһ��
		{
			path_flag = 3;
            switch_mode(Y_Mode1);//�л�Ϊ��¢ģʽ
		}  
	}
}
void Path_Four(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//�ж��Ѿ�ת�����
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //�ж��Ѿ��������·��
	if(flag==2 && JG[0] <= Stop_Disdance_big )	//�ж��Ѿ�������ĵ�
	{
        Ramp_Stop(0);
		flag=3;
	}
	if(flag==3&& Switch[FR]==0 && Switch[BR]==0)
	{
		if(Turn(-90,stop_TIME)) //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode4);//�л�Ϊ������ֱ��ģʽ
			path_flag = 4;
		}
	}
}
void Path_Five(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //�жϵ��ĵ�ת�����
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //�ж��Ѿ��������·��
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
  if(flag == 3 && Switch[FR]==0)                         //�ж��Ѿ���������
	{
		flag=4;
	}
	if(flag == 4)
	{
        Ramp_Stop(1); 
		if(Turn(90,stop_TIME))		                          //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode2);                            //�л�Ϊ���ֱ��ģʽ
			path_flag = 5;
		}
	}
}

void Path_Six(void)
{
	static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //�ж��Ѿ��������·��
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//�ж��Ѿ����������
	{
		flag=2;
	}
	if(flag==2)
	{
		if(Turn(90,stop_TIME))                         //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode3);//�л�Ϊ��ǰ����ֱ��ģʽ
//            switch_mode(Protect_Mode);  
			path_flag = 6;

		}
	}
}


void Path_Seven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //�жϵ�����ת�����
    if(flag == 1 && Switch[FR]==1 && Switch[BR]==0)   //�ж��Ѿ��������·��
    {
        flag=2;
    }
    if(flag == 2 && Switch[FR]==0)
    {
        flag=3;
    }
    if(flag == 3 && Switch[BR]==0) //�ж��Ѿ�������ߵ�
    {
        flag=4;
    }
    if(flag == 4)
    {
        Ramp_Stop(1);    
        if(Turn(-90,stop_TIME))		//��ת90�ȣ���Сͣһ��
        {
            switch_mode(Y_Mode1); //�л�Ϊ��¢ģʽ
            path_flag = 7;
        }
	}
}

void Path_Eight(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[4] > Stop_Disdance_small)	flag=1;//�ж��Ѿ�ת�����
    if(flag==1 && Switch[FR]==1 && Switch[BR]==1) flag=2;  //�ж��Ѿ�����ڰ�·��
	if(flag==2 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 &&Switch[BR]==0)	//�ж��Ѿ�����ڰ˵�
	{
		flag=3;
	}
	if(flag==3)
	{
		if(Turn(-90,stop_TIME))			//��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode4);//�л�Ϊ������ֱ��ģʽ
			path_flag = 8;
		}
	}
}

void Path_Night(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] > Move_Disdance_big )	flag=1;	    //�жϵڰ˵�ת�����
	if(flag == 1 && Switch[FR]==0 && Switch[BR]==1)         //�ж��Ѿ�����ھ�·��
	{
		flag=2;
	}
	if(flag == 2 && Switch[BR]==0)
	{
		flag=3;
	}
    if(flag == 3 && Switch[FR]==0)                         //�ж��Ѿ�����ھŵ�
	{
		flag=4;
	}
	if(flag == 4)
    {
        Ramp_Stop(1);   
		if(Turn(90,stop_TIME))		//��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode2);//�л�Ϊ���ֱ��ģʽ
			path_flag = 9;
		}
  }
}

void Path_Ten(void)
{
  static uint8_t flag=0;
	if(flag==0 && Switch[FR]==1 && Switch[BR]==1 )	flag=1; //�ж��Ѿ������ʮ·��
	if(flag==1 && JG[3] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)	//�ж��Ѿ������ʮ��
	{
		flag=2;
	}
	if(flag==2)
	{
		if(Turn(90,stop_TIME))			//��ת90�ȣ���Сͣһ��
		{
            switch_mode(Exit_Mode); //�л�Ϊ��¢ģʽ
			path_flag = 10;
		}
	}
}

void Path_Eleven(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[3]  && JG[0] >= Stop_Disdance_big)			//�ж���ת��
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0) //�ж��Ѿ������ʮһ��
	{
		flag=2;
	}
	if(flag==2)
	{ 
		if(Turn(-90,stop_TIME))    //��ת90�ȣ���Сͣһ��
		{
            switch_mode(Y_Mode1);   //�л�Ϊ��ǰֱ��ģʽ
			path_flag = 11;
		}
	}
}
void Path_Twelve(void)
{
	static uint8_t flag=0;
	if(flag==0 && JG[0] >= Move_Disdance_big)	//�ж���ת��
	{
		flag=1;
	}
	if(flag==1 && JG[0] <= Stop_Disdance_big && Switch[FR]==0 && Switch[BR]==0)//�ж��Ѿ������ʮ����
	{
		flag=2;
	}
	if(flag==2)
	{
		if(Turn(-90,stop_TIME))   //��ת90�ȣ���Сͣһ��
		{
//			switch_mode(Y_Mode1); //�л�Ϊ��ǰֱ��ģʽ
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
		switch_mode(Protect_Mode);              //ת��ǰ���ƶ����Ҷ���stop��������ȶ��ԡ�
		stop_time=0;
		flag=1;//������
	}
	if(flag == 1 && stop_time >= time)       //�����б���ģʽ��ʱ�� ���ڵ��� ���õ�ֹͣʱ���ǣ���ʼת��
	{
 		angle_set += target_angle;
        if(angle_set>=180)angle_set-=360;
        if(angle_set<=-180)angle_set+=360;
		angle_flag = 0;                        //��־�ſ�ʼת��
		switch_mode(Turn_Mode);
		flag=2;                                //��־��ת�����
	}
	if(flag == 2 && ABS(Angle_Error_WT)<= 3.0f && angle_flag ==1)  //ת����飬���ת����Ϻ�Ƕ������1�����ڣ�����flag������������
	{
		flag=0;
		return 1;
	}	
	return 0;
}
#endif

