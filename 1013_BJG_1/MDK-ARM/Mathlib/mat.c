#include "mat.h"

unsigned char HexToChar(unsigned char bChar)
{
	if((bChar>=0x30)&&(bChar<=0x39))
	{
		bChar -= 0x30;
	}
	else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
	{
		bChar -= 0x37;
	}
	else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
	{
		bChar -= 0x57;
	}
	else 
	{
		bChar = 0xff;
	}
	return bChar;

}

void T0f_Conversion_HEX( uint8_t tof0_dma_buff[], int *ptof0 ){
	
	int i = 0;
	for( ; i < 10; i++ ){
		if( i == 4 && tof0_dma_buff[i] == 'm' ){
					
			tof0_dma_buff[i - 2] = HexToChar(tof0_dma_buff[i - 2]);
			tof0_dma_buff[i - 1] = HexToChar(tof0_dma_buff[i - 1]);				//转换为16进制 
			 *ptof0 =  	tof0_dma_buff[i - 2] *10 + 
			 			 	tof0_dma_buff[i - 1] *1; 	
			 break;
		}else if( i == 5 && tof0_dma_buff[i] == 'm' ){
			
			tof0_dma_buff[i - 3] = HexToChar(tof0_dma_buff[i - 3]);
			tof0_dma_buff[i - 2] = HexToChar(tof0_dma_buff[i - 2]);
			tof0_dma_buff[i - 1] = HexToChar(tof0_dma_buff[i - 1]);				//转换为16进制 
			 *ptof0 = tof0_dma_buff[i - 3] *100 + 
			 			tof0_dma_buff[i - 2] *10 + 
			 			 	tof0_dma_buff[i - 1] *1; 
			 break;
		}else if( i == 6 && tof0_dma_buff[i] == 'm' ){
			tof0_dma_buff[i - 4] = HexToChar(tof0_dma_buff[i - 4]);
			tof0_dma_buff[i - 3] = HexToChar(tof0_dma_buff[i - 3]);
			tof0_dma_buff[i - 2] = HexToChar(tof0_dma_buff[i - 2]);
			tof0_dma_buff[i - 1] = HexToChar(tof0_dma_buff[i - 1]);				//转换为16进制 
			 *ptof0 = tof0_dma_buff[i - 4] * 1000 +
									tof0_dma_buff[i - 3] * 100 + 
										tof0_dma_buff[i - 2] * 10 + 
											tof0_dma_buff[i - 1] * 1; 
			 break;
		}
	}
}


