#include "ML51.h"
uint16_t disvlue;
//#define TEST_LCD
void Init_Lcd(void)
{
 /*TSLiu */
/* Enable COM pin MFP */
    
    MFP_P41_LCD_COM2;  //硬件接的是COM2
    MFP_P40_LCD_COM3;  //硬件接的是COM3
	  MFP_P47_LCD_COM0;
	  MFP_P33_LCD_COM1;
   // MFP_P45_LCD_COM4;  //硬件接的是COM0
   // MFP_P44_LCD_COM5;  //硬件接的是COM1
/* Enable SEG pin MFP */
    MFP_P06_LCD_SEG0;
    MFP_P07_LCD_SEG1;
    MFP_P34_LCD_SEG2;
    MFP_P35_LCD_SEG3;
    MFP_P20_LCD_SEG4;
   // MFP_P21_LCD_SEG5;
    MFP_P26_LCD_SEG6;
    MFP_P27_LCD_SEG7;
    MFP_P64_LCD_SEG8;
    MFP_P65_LCD_SEG9;
    MFP_P66_LCD_SEG10;
    MFP_P67_LCD_SEG11;
    MFP_P30_LCD_SEG12;
    MFP_P31_LCD_SEG13;
    MFP_P32_LCD_SEG14;
    //MFP_P33_LCD_SEG15;
    //MFP_P47_LCD_SEG16;
		MFP_P44_LCD_SEG30;
		MFP_P45_LCD_SEG31;
    MFP_P46_LCD_SEG17;
    MFP_P17_LCD_SEG18;
    MFP_P16_LCD_SEG19;
    MFP_P15_LCD_SEG20;
    MFP_P14_LCD_SEG21;
    MFP_P60_LCD_SEG22;
    MFP_P61_LCD_SEG23;
    MFP_P62_LCD_SEG24;  //硬件接的是SEG5
		
	  LCD_Clock_Setting(LIRC, LCD_FREQ_DIV4);
    LCD_Open(TYPE_B, AVDD_PIN,LCD_CPVOL_3_4V, BIAS_1_3, LCD_4COM);
    LCD_Current_Mode( Buffer_Mode  , TurnOn_All);
	 //LCD_Blink(Enable,3);
    LCD_Enable();
    LCD_SetAllPixels(Enable);
	// printf("\n LCDINIT OK!");
}

void Lcd_Display(void)
{
LCD_SetAllPixels(Disable);
//LCD_SetOnePixel(LCD_COM0, SEG_CO2, Enable);         //点亮CO2图标


}

void ShowDigit(UINT8 digitPlace, UINT8 num, UINT8 tempUnit) 
	{



    UINT8 segABCD = 0;

    UINT8 segEFG = 0;



    // 参数检查

    //if ((digitPlace < 1 || digitPlace > 3) ||

     //   (num > 9)) {

       // return;

    // 根据实际的引脚连接，以及LCD模块的段码表，确定改变哪些SEG的数据

    // 每个SEG是一个8位数据，1/4Duty时，高4位不使用，低4位的数据就是段码表的COM1-4的数据

    switch (digitPlace)
    {
        case 1:
            segABCD = 2;
            segEFG = 1;
            break;

        case 2:
            segABCD = 4;
            segEFG = 3;
            break;

        case 3:
            segABCD = 6;
            segEFG = 24;
            break;
				
				case 4:
            segABCD = 8;
            segEFG = 7;
            break;
				case 5:
            segABCD = 11;
            segEFG = 10;
            break;
				case 6:
            segABCD = 13;
            segEFG = 12;
            break;
				case 7:
            //segABCD = 15;
				    segABCD =30;
            segEFG = 14;
            break;
				case 8:
            segABCD = 18;
            segEFG = 17;
            break;
				
				case 9:
            segABCD = 20;
            segEFG = 19;
            break;
				case 10:
            segEFG = 10;
            break;
				case 11:
            segEFG = 17;
            break;
        default:

            break;

    }



    // 确定了SEG以后，开始设置数字各段数据(其实可以全部计算出来结果放在一个code数组里即可)

    // 设置ABCD段数据(COM0-A,COM1-B,COM2-C,COM3-D)
    SFRS = 3;
    LCDPTR  = segABCD;

    switch (num)
    {
        case 0:
            LCDDAT  = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
            break;
        case 1:
					
            LCDDAT  = LCD_COM1 | LCD_COM2;
            break;
        case 2:
            LCDDAT  = LCD_COM0 | LCD_COM1 | LCD_COM3;
            break;
        case 3:
            LCDDAT  = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
				break;
        case 4:
            LCDDAT  = LCD_COM1 | LCD_COM2;
            break;
        case 5:
            LCDDAT  = LCD_COM0 | LCD_COM2 | LCD_COM3;
            break;
        case 6:
            LCDDAT  = LCD_COM0 | LCD_COM2 | LCD_COM3;
            break;
        case 7:
            LCDDAT  = LCD_COM0 | LCD_COM1 | LCD_COM2;
            break;
        case 8:
            LCDDAT  = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
            break;
        case 9:
            LCDDAT  = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
            break;



        default:

            break;

    }

    // 设置EFG段数据(COM1-F,COM2-G,COM3-E)

    SFRS = 3;LCDPTR  = segEFG;

    switch (num)

    {
        case 0:
            LCDDAT  = LCD_COM1 | LCD_COM3;
            break;
        case 1:
            LCDDAT  = 0x00;
            break;
        case 2:
            LCDDAT  = LCD_COM2 | LCD_COM3;
            break;
        case 3:
            LCDDAT  = LCD_COM2;
            break;
        case 4:
            LCDDAT  = LCD_COM1 | LCD_COM2;
            break;
        case 5:
            LCDDAT  = LCD_COM1 | LCD_COM2;
            break;
        case 6:
            LCDDAT  = LCD_COM1 | LCD_COM2 | LCD_COM3;
            break;
        case 7:
            LCDDAT  = 0x00;
            break;
        case 8:
            LCDDAT  = LCD_COM1 | LCD_COM2 | LCD_COM3;
            break;
       case 9:
            LCDDAT  = LCD_COM1 |LCD_COM2;
            break;
        default:

            break;

    }



    // 设置摄氏度符号段数据(COM0-SEG0,2,4)

    // 注意这里要用或计算，不然会覆盖之前设置好的数字部分的段码数据

    switch (tempUnit)

    {

       /* case 0xC:
            SFRS = 3;
            LCDPTR  = 31; // 4A段 set

            LCDDAT |= LCD_COM2|LCD_COM3;

            LCDPTR  = 14; // 4D段 set

            LCDDAT |= LCD_COM0;

            //LCDPTR  = 0; // 4G段 clear

            //LCDDAT &= ~COM0;

            break;

        case 0xF:

            LCDPTR  = 2; // 4A段 set

            LCDDAT |= LCD_COM0;

            LCDPTR  = 4; // 4D段 clear

            LCDDAT &= ~LCD_COM0;

            LCDPTR  = 0; // 4G段 set

            LCDDAT |= LCD_COM0;

            break;

        default:

           

            break;*/

    }

}


void LCD_Display_TempHub(uint16_t TValue,uint16_t HValue)
{
	   unsigned char 	tgw,tsw,tbw,tqw,hgw,hsw,hbw;//；,segAg,segFg,segAs,segFs,segAb,segFb,segFq;
	   unsigned int   test_value;
	   static unsigned int   test_time;
	   test_time++;
	   LCD_SetAllPixels(Disable);
	   if(Get_Unit_Sta()==Unit_C)
		 {
		  LCD_SetOnePixel(0, 14, Enable); //打开小数点
			LCD_SetOnePixel(2, 31, Enable); 
			LCD_SetOnePixel(3, 31, Enable);
		 }
		 else if(Get_Unit_Sta()==Unit_F)
		 {
		 TValue=TValue*1.8+32;
		 LCD_SetOnePixel(0, 14, Enable); //打开小数点
		 LCD_SetOnePixel(2, 31, Enable); 
		 LCD_SetOnePixel(1, 31, Enable);	 
			 
		 }
	   tqw = TValue/1000;
	   tbw = TValue%1000/100;
	   tsw = TValue%100/10;
	   tgw = TValue%10;
	
	   hbw = HValue/100;
	   hsw = HValue%100/10;
	   hgw = HValue%10;
	   
	
	   ShowDigit(7, tgw,0XC);
		 ShowDigit(6, tsw,0XC);
		 if(TValue>=100)ShowDigit(5, tbw,0XC);
		 if(TValue>=1000)
		 {
		 LCD_SetOnePixel(0, 10, Enable);
		 }
		 LCD_SetOnePixel(0, 14, Enable);
				
		 ShowDigit(9, hgw,0);
		 ShowDigit(8, hsw,0);	
		 if(HValue>=100)
		 {	 
		  LCD_SetOnePixel(0, 17, Enable);
		 }
		 LCD_SetOnePixel(0, 19, Enable);
		 if(((180<TValue)&&(TValue<=280))&&((400<=HValue)&&(HValue<=700)))	
     {
		 LCD_SetOnePixel(0, 9, Enable);
	
		 LCD_SetOnePixel(1, 21, Enable);
		 }	
     if(((120<TValue)&&(TValue<=180)||(280<TValue)&&(TValue<=320))||((250<=HValue)&&(HValue<=390))||((710<HValue)&&(HValue<=850)))	
     {
		 LCD_SetOnePixel(0, 16, Enable);
	
		 LCD_SetOnePixel(1, 21, Enable);
		 }
     if(((TValue<120)||(32<TValue))||((HValue<250))||((850<HValue)))	
     {
		 	 LCD_SetOnePixel(0, 21, Enable);
	
		 LCD_SetOnePixel(1, 21, Enable);
		 }			 
     			 
	
	#ifdef TEST_LCD
		 test_time++;
	   if(test_time==2)
		 {
		 test_value=GET_NtcTEMP();
		 LCD_SetOnePixel(0, 0, Enable);
		 }
		 else if(test_time==4)
		 {
		 test_value=GET_NtcTEMP();
     LCD_SetOnePixel(1, 0, Enable);			 
		 }
		 else if(test_time==6)
		 {
		 test_value=GET_NtcTEMP();		
     LCD_SetOnePixel(2, 0, Enable);			 
		 }
		 else if(test_time==8)
		 {
		 test_value=GET_NtcTEMP();	
     LCD_SetOnePixel(3, 0, Enable);		
     test_time=0;			 
		 }
		 
		 
	 
		 
		 
	
	
	
	
	
	#else
	   test_time++;
	   if(test_time==2)
		 {
		 LCD_SetOnePixel(0, 0, Enable);
		 }
		 else if(test_time==4)
		 {
		 
     LCD_SetOnePixel(1, 0, Enable);			 
		 }
		 else if(test_time==6)
		 {
		 
     LCD_SetOnePixel(2, 0, Enable);			 
		 }
		 else if(test_time==8)
		 {
		 
     LCD_SetOnePixel(3, 0, Enable);		
     test_time=0;			 
		 }
	   Display_Co2(Get_Co2DisMod());
		 
		 // LCD_SetOnePixel(0, 0, Enable);
	#endif
		if(Get_Light_Sta()==Light_off)
		{
		LCD_SetOnePixel(0, 7, Disable);
		}
		else if(Get_Light_Sta()==Light_on)
		{
		LCD_SetOnePixel(0, 7, Enable);
		}
		
		if(Get_Bell_Sta()==Bell_off)
		{
	   LCD_SetOnePixel(0, 24, Disable);
		}
		else if(Get_Bell_Sta()==Bell_on)
		{
		LCD_SetOnePixel(0, 24, Enable);
		}
		
		LCD_SetOnePixel(0, 1, Enable);
		LCD_SetOnePixel(2, 21, Enable);
		LCD_SetOnePixel(3, 21, Enable);
		LCD_SetOnePixel(2, 22, Enable);
		LCD_SetOnePixel(3, 22, Enable);
		
		
		
		Bat_PowerDisplay();
}
