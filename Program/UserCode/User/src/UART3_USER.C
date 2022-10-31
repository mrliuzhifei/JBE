#include "ML51.h"
/*for HCHO and printf*/
void Init_UART3(void)
{
    MFP_P12_UART3_TXD;
    P12_QUASI_MODE;
    MFP_P11_UART3_RXD;
    P11_INPUT_MODE;
    UART3_Open(24000000,9600);                 /* Open UART3 use timer1 as baudrate generate and baud rate = 115200*/

    //ENABLE_SC1_RECEIVE_DATA_REACH_INTERRUPT;
    //ENABLE_GLOBAL_INTERRUPT;


}


char putchar (char c)
{
	  //while(!(SC0TSR|SET_BIT3));
	  UART3_Send_Data(c);  //·¢ËÍÊý¾Ý		
		//SBUF1 = c;      /* output character */
		return (c);
}


