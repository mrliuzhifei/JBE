#include "ML51.h"
/**********************************************************************
  PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
                = (24MHz/8)/(0x456 + 1)
                = 2.7KHz
***********************************************************************/
void LED_Init(void)
{ 
	  /*设置io口推挽输出*/
     // GPIO_SetMode(BLU_PORT_R ,BLU_R_PIN,GPIO_MODE_PUSHPULL);
     // GPIO_SetMode(BLU_PORT_GB ,BLU_G_PIN|BLU_B_PIN,GPIO_MODE_PUSHPULL);
    GPIO_SetMode(LAMP_PORT ,LAMP_R_PIN|LAMP_G_PIN|LAMP_B_PIN,GPIO_MODE_PUSHPULL);
	  // BLU_ON();
	  // LAMP_OFF();
	  PWM123_ClockSource(PWM2,8);
	  MFP_P43_PWM2_CH0;                               // multi function pin define P0.3 as PWM0 channel 2 output
    P43_PUSHPULL_MODE;
    PWM123_ConfigOutputChannel(PWM2,0,0x456,50);    // 2.7K输出PWM占空比50%
	  set_PWM2CON0_LOAD;                              //新的周期占空比载入  //这个是蜂鸣器的驱动
	  PWM0_ClockSource(PWM_FSYS,8); 
	
	  MFP_P05_PWM0_CH0;//G                                                // multi function pin define P0.5 as PWM0 channel 0 output
    P05_PUSHPULL_MODE;                                              
    PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,100);    // setting PWM channel 0 as 10% duty high of 0x6FF PWM period = 0x00B3
	  
	  MFP_P04_PWM0_CH1;//B                                               // multi function pin define P0.5 as PWM0 channel 0 output
    P04_PUSHPULL_MODE;                                              
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,100);    // setting PWM channel 0 as 10% duty high of 0x6FF PWM period = 0x00B3   
	  
	  MFP_P36_PWM0_CH5;//R                                                // multi function pin define P0.5 as PWM0 channel 0 output
    P36_PUSHPULL_MODE;                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,100);  
		
    
		//set_PWM0CON0_PWMRUN;//pwm0输出使能
		
		//set_PWM2CON0_PWMRUN;//pwm2输出使能
		clr_PWM2CON0_PWM2RUN;
    PWM0_RUN();
	
	
	
}

void LED_FLASH(unsigned char Times)
{
switch (Times)
{
	case 1:
		LAMP_GREEN_ON();
		break;
	case 2:
		LAMP_YELLOW_ON();
		break;
	case 3:
		LAMP_RED_ON();
		break;
	
	case 4:
		LAMP_RED_ON();
		break;

}

}

void LED_Buz(void)
{
	 static unsigned char BuzTime=0;
	if(Get_Light_Sta()==Light_on)
	{
	  if(Get_Co2Value()<=1000)
		{
		LAMP_GREEN_ON();
		}
		else if(Get_Co2Value()<=1400)
		{
		LAMP_YELLOW_ON();
		}
	  else if(Get_Co2Value()<=1800)
		{
		LAMP_RED_ON();
		}
		else
		{
		LAMP_RED_ON();
		if(Get_Bell_Sta()==Bell_off)
		{
		clr_PWM2CON0_PWM2RUN;
		}
		else if(Get_Bell_Sta()==Bell_on)
		{
			BuzTime++;
			if(BuzTime<50)
			{
			set_PWM2CON0_PWMRUN;
			}
			else if(BuzTime<100)
			{
			clr_PWM2CON0_PWM2RUN;
			}
			else BuzTime=0;
			}	
		}
	
	}
	else if(Get_Light_Sta()==Light_off)
	{
	LAMP_OFF();
	clr_PWM2CON0_PWM2RUN;
	}

	if(Get_Co2Value()>1800)
	{
		if(Get_Bell_Sta()==Bell_off)
		{
		//clr_PWM2CON0_PWM2RUN;
		PWM123_ConfigOutputChannel(PWM2,0,0x456,0);    // 2.7K输出PWM占空比50%
		set_PWM2CON0_LOAD; 
		set_PWM2CON0_PWMRUN;	
		}
		else if(Get_Bell_Sta()==Bell_on)
		{
			BuzTime++;
			if(BuzTime<50)
			{
			
			PWM123_ConfigOutputChannel(PWM2,0,0x456,50);    // 2.7K输出PWM占空比50% set_PWM2CON0_PWMRUN;
			set_PWM2CON0_LOAD;  
			set_PWM2CON0_PWMRUN;	
			}
			else if(BuzTime<100)
			{
			
			PWM123_ConfigOutputChannel(PWM2,0,0x456,0);    // 2.7K输出PWM占空比50%//clr_PWM2CON0_PWM2RUN;
			
			set_PWM2CON0_LOAD; 
			set_PWM2CON0_PWMRUN;	
			}
			else BuzTime=0;
		}	
  
 
		

  
}
void RGB_Color(Color_s RGBColor)
{
 PWM0_STOP();
 switch (RGBColor)
 {
	 case White:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,100);    //G                                  
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,100);    //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,100);    //R
   break;
	 case Red:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,0);      //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,0);      //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,100);    //R
   break;
   case Green:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,100);    //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,0);      //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,0);      //R
   break;
   case Blue:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,0);      //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,100);    //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,0);      //R
   break;
   case Yellow:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,100);    //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,0);      //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,100);    //R
   break;
   case Purple:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,0);      //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,100);    //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,100);    //R
   break;
   case Orange:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,70);     //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,20);      //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,100);    //R 
   break;
   case Cyan:
		PWM0_ConfigOutputChannel(0,Independent,EdgeAligned,0x456,100);    //G                                        
    PWM0_ConfigOutputChannel(1,Independent,EdgeAligned,0x456,100);    //B                                              
    PWM0_ConfigOutputChannel(5,Independent,EdgeAligned,0x456,0);      //R
	 break;
	 default:
	 break;
 }
 PWM0_RUN();
 

}
