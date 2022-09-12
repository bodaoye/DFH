//Channel��ѡ��ͨ����1-10��
//���뵥���ȸ�����
//�ں��������·���DataWave�����޸ķ��͵�����
//�������������޸�

#include "DataScope_DP.h"
#include "chassis_task.h"

#include "main.h"
#include "usart.h"

#include "bsp_gyro.h"
#include "pid.h"
#include "vision_task.h"
#define ABS(x)		((x>0)? (x): (-x))

typedef struct
{
    unsigned char DataScope_OutPut_Buffer[44];	     //���ڷ��ͻ�����
    unsigned char Send_Count;                        //������Ҫ���͵����ݸ���
    unsigned char DataCnt;                           //��������
} DataTypedfef;

DataTypedfef CK;	//���������õ��Ľṹ��

//����˵�����������ȸ�������ת��4�ֽ����ݲ�����ָ����ַ
//target:Ŀ�굥��������
//buf:��д������
//beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	 //�õ�float�ĵ�ַ
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3]; 
}



//����˵������������ͨ���ĵ����ȸ�������д�뷢�ͻ�����
//Data��ͨ������
//Channel��ѡ��ͨ����1-10��  
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
    if ( (Channel > 10) || (Channel == 0) ) 
        return; //ͨ����������10�����0��ֱ����������ִ�к���
    else
    {
    #ifdef MINIBALANCE
        Float2Byte(&Data,CK.DataScope_OutPut_Buffer,( (Channel-1) * 4 +1 ) );   //����֡ͷ
    #else  //VOFA
        Float2Byte(&Data,CK.DataScope_OutPut_Buffer,( (Channel-1) * 4) );
    #endif
    }
}


//����˵������������ȷʶ���֡��ʽ
//Channel_Number����Ҫ���͵�ͨ������
//���ط��ͻ��������ݸ���
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
    if ( (Channel_Number > 10) || (Channel_Number == 0) )
    {
        return 0;    //ͨ����������10�����0��ֱ����������ִ�к���
    }
    else
    {
    #ifdef MINIBALANCE
        CK.DataScope_OutPut_Buffer[0] = '$';  //֡ͷ
        uint8_t temp_cnt = Channel_Number*4+1;
        CK.DataScope_OutPut_Buffer[temp_cnt]  =  temp_cnt;  //֡β
        return (temp_cnt+1);  //����һ�����ݰ����ֽ���
    #else  //VOFA+
        uint8_t temp_cnt = Channel_Number*4+4;
        CK.DataScope_OutPut_Buffer[4*Channel_Number + 0] = 0x00;
        CK.DataScope_OutPut_Buffer[4*Channel_Number + 1] = 0x00;
        CK.DataScope_OutPut_Buffer[4*Channel_Number + 2] = 0x80;
        CK.DataScope_OutPut_Buffer[4*Channel_Number + 3] = 0x7f;
        return temp_cnt;  //����һ�����ݰ����ֽ���
    #endif
    }
}


extern float vision_yaw_perdict;
extern float test_abs_speed_kp;
extern float test_angle_error;

//����˵������λ��ͨ�����ڴ�ӡ���ݲ���
//����˵����ֱ�����������е��ô˺���
void DataWave(UART_HandleTypeDef* huart)
{
    {
      DataScope_Get_Channel_Data((float) (chassis.target_v[0]), 1 );
      DataScope_Get_Channel_Data((float) (chassis.v[0]), 2 );
      DataScope_Get_Channel_Data((float) (chassis.v[1]), 3 );
      DataScope_Get_Channel_Data((float) (chassis.v[2]), 4 );
      DataScope_Get_Channel_Data((float) (chassis.v[3]), 5 );
      
     
//       DataScope_Get_Channel_Data((float) turn_pid.set[0], 1 );
//       DataScope_Get_Channel_Data((float) Angle_Error_WT, 2 );
//    DataScope_Get_Channel_Data((float) (gimbal.error_ecd), 1 );
//    DataScope_Get_Channel_Data((float) (target_ecd), 2 );
//    DataScope_Get_Channel_Data((float) (gimbal.target_ecd), 3 );
//    DataScope_Get_Channel_Data((float) (gimbal.ecd), 4 );      
//      DataScope_Get_Channel_Data((float) (chassis.target_v[2]), 1 );
//      DataScope_Get_Channel_Data((float) (chassis.v[2]), 2 );
//      DataScope_Get_Channel_Data((float) (chassis.target_v[3]), 1 );
//      DataScope_Get_Channel_Data((float) (chassis.v[3]), 2 );
//      
      


//    DataScope_Get_Channel_Data((float) (pid_trigger_ecd.pos_out), 1 );
//    DataScope_Get_Channel_Data((float) (pid_trigger_spd.pos_out), 2 );
//    DataScope_Get_Channel_Data((float) (pid_trigger_spd.iout), 3 );

//    DataScope_Get_Channel_Data((float) (pid_trigger_ecd.err[0]), 1 );
//    DataScope_Get_Channel_Data((float) (pid_trigger_spd.err[0]), 2 );


    }
    CK.Send_Count = DataScope_Data_Generate(5);

    for( CK.DataCnt = 0 ; CK.DataCnt < CK.Send_Count; CK.DataCnt++)
    {
        while((huart->Instance->SR&0X40)==0);
        huart->Instance->DR = CK.DataScope_OutPut_Buffer[CK.DataCnt];
    }
} 

void minibalance(void)
{
  static uint8_t debug_i;                                             
  debug_i++;  
  if(debug_i == 6)
  {
      DataWave(&huart1);
      debug_i = 0;
  }
}



 