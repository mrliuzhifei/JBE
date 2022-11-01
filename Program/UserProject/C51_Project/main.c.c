
#include "ML51.h"
#include "BC3602.h"
#include "BC3602_CMD_REG.h"
/*------------------------------------------------------------------*/

void main (void) 
{   
	  FsysSelect(FSYS_HIRC); 
    Init_UART1();
    //Init_UART2();
	  //Init_UART3();
	  Timer0_Init();
	  CO2_Init();
	  LED_Init();
	  Init_Lcd();
    WDT_Init();
	  Init_I2C();
	  ADC_Init();
	  Init_KeyPort();
	  CO2_Init();
	  EEPROM_ReadDataInit();	//¶ÁÈ¡eepromµÄÖµ
	  //Init_Wifi();
    RF_Init();
	  
  while(1)
  {
		FEED_WDT();     //
	  Time_Process();
	  //BC3602_RF_Process();
		
  }
}





