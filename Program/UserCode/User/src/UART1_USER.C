#include "ML51.h"

/*  for CO2*/
void Init_UART1(void)
{
    MFP_P23_UART1_TXD;
    P23_QUASI_MODE;
    MFP_P22_UART1_RXD;
    P22_INPUT_MODE;

    UART_Open(24000000,UART1_Timer3,9600);
    ENABLE_UART1_INTERRUPT;                 //For interrupt enable , interrupt subroutine is in uart.c file
    ENABLE_GLOBAL_INTERRUPT;                //Enable global interrupt
}


void Uart1senddata(UINT8* Data,UINT8 len)
 {
   UINT8 i;
   for(i=0;i<len;i++)
	 {
		 UART_Send_Data(UART1, *(Data+i));
//	 send_byte(*(date+i));	 
	 }
 }
 
 
 