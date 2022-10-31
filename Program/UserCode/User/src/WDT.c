#include "ML51.h"
void WDT_Init(void)
{
    TA=0xAA;TA=0x55;WDCON=0x07;          //Setting WDT prescale TA 为时效访问寄存器
	#ifdef POWER_DOWN_MODE
    set_WDCON_WIDPD;                     //WDT run in POWER DOWM mode setting if needed
	#endif
    //ENABLE_WDT_INTERRUPT;
   // ENABLE_GLOBAL_INTERRUPT;
    set_WDCON_WDTR;                      //WDT run
    set_WDCON_WDCLR;                     //Clear WDT timer
    while((WDCON|~SET_BIT6)==0xFF);      //等待完成喂狗
}

void FEED_WDT(void)
{
    clr_WDCON_WDTF;
    set_WDCON_WDCLR;
    while((WDCON|~SET_BIT6)==0xFF);
}


