#include "ML51.h"
/*for WIFI*/
void Init_UART2(void)
{
    MFP_P25_UART2_TXD;                              /* 设置P0.0和P0.1为UART2端口*/
	  P25_QUASI_MODE; 
    MFP_P24_UART2_RXD;
	  P24_INPUT_MODE; 
	
    UART2_Open(24000000,9600);                 /* Open UART2 use timer1 as baudrate generate and baud rate = 115200*/
    ENABLE_SC0_RECEIVE_DATA_REACH_INTERRUPT;   //开启接收中断
    ENABLE_GLOBAL_INTERRUPT;                   //开启全局中断

}

void Uart2senddata(UINT8* Data,UINT8 len)
 {
   UINT8 i;
   for(i=0;i<len;i++)
	 {
		 UART_Send_Data(UART2, *(Data+i));
//	 send_byte(*(date+i));	 
	 }
 }

