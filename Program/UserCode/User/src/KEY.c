#include "ML51.h"
#include "BC3602.h"
#include "BC3602_CMD_REG.h"
typedef struct KeyEvent_t
{
  uint8_t PressTimeCount;
  uint8_t ReleaseTimeCount;
  uint8_t ClickSingleTimeCount;
  uint8_t ClickSingleTimes;
  uint8_t KeyPressFlag;
	uint8_t Key_Conti_Press;
  uint8_t KeyPress;
  uint8_t KeyDate;
	uint8_t KeyDate_pro;
  uint8_t KeyValue_pro;
	Key_Value_s KeyValue;
	DIS_MODE_e Key_DisMod;
	DIS_MODE_e Key_DisModOlde;
	Light_Sw_s	Light_Sw;
	Bell_Sw_s   Bell_Sw;
	Unit_Sw_s   Unit_Sw;
	Color_s     Color;
	Co2DisMod_s Co2DisMod;
//	NetWorkMode_s NetWorkMode;
	uint8_t Longpress;
	uint8_t Wifi_Key_Sta;
}KeyEvent_t;
KeyEvent_t KeyEvent;
#define JBE KeyEvent

void Init_KeyPort(void)
{
 P63_QUASI_MODE;    //P6.3设置为准双向模式，作为按键输入
 ENABLE_P63_PULLUP;
 P56_QUASI_MODE;
 ENABLE_P56_PULLUP;
 JBE.Light_Sw=Light_on;
 JBE.Bell_Sw=Bell_on;
 JBE.Color=Orange;
}

void Get_KeyValue(void)
{
 uint8_t KeyDataTemporary;
 SFRS=2;
 JBE.KeyDate=P6;
 JBE.KeyDate=(~(JBE.KeyDate)>>3)&0x01;
 SFRS=0;
 KeyDataTemporary=JBE.KeyDate;
 JBE.KeyDate=P5;
 JBE.KeyDate=(~(JBE.KeyDate)>>5)&0X02;
 JBE.KeyDate=JBE.KeyDate+KeyDataTemporary;
 if(JBE.KeyDate!=0)
 {
 JBE.PressTimeCount++;
 if(JBE.PressTimeCount>5)
 {
  if(JBE.KeyDate!=0)
	{
	 if(JBE.KeyPressFlag==True )  //按键已经按下还没有做键值的判断
	 {
	 JBE.Key_Conti_Press=True;    //这是一次连续按键
	 }
	 else JBE.KeyPressFlag=True; //是否有按键按下  
	 JBE.KeyValue_pro=Key_Null;     
	 JBE.KeyDate_pro=JBE.KeyDate;
	 JBE.PressTimeCount=0; 
	 JBE.ClickSingleTimeCount=0;
	}
	else JBE.PressTimeCount=0;
 }
 }
 else JBE.PressTimeCount=0;

 if(JBE.KeyPressFlag==True)
 {
	 JBE.ReleaseTimeCount++;
	 switch (JBE.KeyDate_pro)
	 {
	 case 0x01:
   if(JBE.KeyDate==0)
	 { 
		if(JBE.ReleaseTimeCount>200)JBE.ReleaseTimeCount=201;
		if(JBE.KeyValue_pro==Key1_ClickSingle)
		{	
			  JBE.ClickSingleTimeCount++;		
		 if(JBE.ClickSingleTimeCount>100)
		 {
		  if(JBE.ClickSingleTimes==1)JBE.KeyValue=Key1_ClickSingle_1;
			else if(JBE.ClickSingleTimes==2)
			{
			JBE.KeyValue=Key1_ClickSingle_2;
			}
			else if(JBE.ClickSingleTimes==3)
      {
			JBE.KeyValue=Key1_ClickSingle_3;
			}
			JBE.ClickSingleTimes=0;
			JBE.ClickSingleTimeCount=0;
			JBE.KeyPressFlag=False;	 
			JBE.KeyValue_pro=Key_Null;
			JBE.Key_Conti_Press=False;
		 }
		}
		else
    {
		if(JBE.ReleaseTimeCount<200&&JBE.Key_Conti_Press==False&&JBE.Longpress==0)
		{
		JBE.KeyValue_pro=Key1_ClickSingle;	
    JBE.ClickSingleTimes=1;			
		JBE.ReleaseTimeCount=0;	
		}
		else if(JBE.ReleaseTimeCount<200&&JBE.Key_Conti_Press==True&&JBE.Longpress==0)
		{
		JBE.KeyValue_pro=Key1_ClickSingle;	
    JBE.ClickSingleTimes++;			
		JBE.ReleaseTimeCount=0;	
		JBE.Key_Conti_Press=False;
		}
		
		else
		{
		 JBE.Longpress=0;
		//JBE.KeyValue=	Key1_LongPress;
    JBE.ReleaseTimeCount=0;	
    JBE.KeyPressFlag=False;		
    JBE.Key_Conti_Press=False;			
		}
		}  
	 }
	 else if(JBE.KeyDate==0x01)
	 {
		if(JBE.ReleaseTimeCount>200) 
		{
			
		 JBE.KeyValue=Key1_LongPress;
     JBE.ReleaseTimeCount=0;	
    // JBE.KeyPressFlag=False;		
     JBE.Key_Conti_Press=False;		
	   JBE.ClickSingleTimeCount=0;
		 JBE.KeyValue_pro=0;
		 JBE.ClickSingleTimeCount=0;
		 JBE.Longpress=1;	
		}
	 // JBE.Key_Conti_Press=False;		
	 }		 
	 break;
	 case 0x02: 
	 if(JBE.KeyDate==0)
	 {
		if(JBE.ReleaseTimeCount>200)JBE.ReleaseTimeCount=201;
		//if(JBE.Longpress==1)
		//{
		//JBE.Longpress=0;
		//return;
		//}
		if(JBE.KeyValue_pro==Key2_ClickSingle)
		{	
			  JBE.ClickSingleTimeCount++;		
		 if(JBE.ClickSingleTimeCount>100)
		 {
		  if(JBE.ClickSingleTimes==1)JBE.KeyValue=Key2_ClickSingle_1;
			else if(JBE.ClickSingleTimes==2)
			{
			JBE.KeyValue=Key2_ClickSingle_2;
			}
			else if(JBE.ClickSingleTimes==3)
      {
			JBE.KeyValue=Key2_ClickSingle_3;
			}
			JBE.ClickSingleTimes=0;
			JBE.ClickSingleTimeCount=0;
			JBE.KeyPressFlag=False;	 
			JBE.KeyValue_pro=Key_Null;
			JBE.Key_Conti_Press=False;
		 }
		}
		else
    {
		if(JBE.ReleaseTimeCount<200&&JBE.Key_Conti_Press==False&&JBE.Longpress==0)
		{
		JBE.KeyValue_pro=Key2_ClickSingle;	
    JBE.ClickSingleTimes=1;			
		JBE.ReleaseTimeCount=0;	
		}
		else if(JBE.ReleaseTimeCount<200&&JBE.Key_Conti_Press==True&&JBE.Longpress==0)
		{
		JBE.KeyValue_pro=Key2_ClickSingle;	
    JBE.ClickSingleTimes++;			
		JBE.ReleaseTimeCount=0;	
		JBE.Key_Conti_Press=False;
		}
		
		else
		{
		//JBE.KeyValue=	Key2_LongPress;
    JBE.Longpress=0;
		//JBE.KeyValue=	Key1_LongPress;
    JBE.ReleaseTimeCount=0;	
    JBE.KeyPressFlag=False;		
    JBE.Key_Conti_Press=False;	
		}
		}  
	 }
	 else if(JBE.KeyDate==0x02)
	 {
	  if(JBE.ReleaseTimeCount>200&&JBE.Longpress==0) 
		{
		JBE.KeyValue=	Key2_LongPress;	
		JBE.ReleaseTimeCount=0;	
    // JBE.KeyPressFlag=False;		
     JBE.Key_Conti_Press=False;		
	   JBE.ClickSingleTimeCount=0;
		 JBE.KeyValue_pro=0;
		 JBE.ClickSingleTimeCount=0;
		 JBE.Longpress=1;	
		}
		//JBE.Key_Conti_Press=False;	
	 }
	 break;
	 default:
	 break;
 }
 }
 if(JBE.KeyValue_pro==Key1_ClickSingle)
 {
  JBE.ClickSingleTimeCount++;
 }

}

void KeyProcess(void)
{
 Get_KeyValue();
if(JBE.KeyValue==	Key1_ClickSingle_1)
{
 if(JBE.Wifi_Key_Sta==0)
 {
  JBE.Wifi_Key_Sta=1;
 }
 else if(JBE.Wifi_Key_Sta==1)
 {
 JBE.Wifi_Key_Sta=2;
 }
 else if(JBE.Wifi_Key_Sta==2)
 {
 JBE.Wifi_Key_Sta=3;
 }
 else if(JBE.Wifi_Key_Sta==3)
 {
 JBE.Wifi_Key_Sta=4;
 }
 else if(JBE.Wifi_Key_Sta==4)
 {
 JBE.Wifi_Key_Sta=1;
 }
 if(Get_RF_Mode()!= RF_TX)KEY_BC3602(JBE.Wifi_Key_Sta);
}
else if(JBE.KeyValue==	Key1_ClickSingle_2)  //双击二氧化碳显示模式切换
{
	if(JBE.Co2DisMod==Current_Valu)
	{
	JBE.Co2DisMod=Max_Valu_24h;
	}
	else if(JBE.Co2DisMod==Max_Valu_24h)
	{
	JBE.Co2DisMod=Avg_Valu_24h;
	}
	else if(JBE.Co2DisMod==Avg_Valu_24h)
	{
	JBE.Co2DisMod=Current_Valu;
	}
 
}
else if(JBE.KeyValue==	Key1_ClickSingle_3)
{

}
else if(JBE.KeyValue==	Key1_LongPress)
{
if(JBE.Unit_Sw==Unit_C)
	{  
		JBE.Unit_Sw=Unit_F;
	}
	else if(JBE.Unit_Sw==Unit_F)
	{  
		JBE.Unit_Sw=Unit_C;
	}
}
else if(JBE.KeyValue==	Key2_ClickSingle_1)
{
	if(JBE.Bell_Sw==Bell_off)
	{  
		JBE.Bell_Sw=Bell_on;
	}
	else if(JBE.Bell_Sw==Bell_on)
	{  
		JBE.Bell_Sw=Bell_off;
	}
}

else if(JBE.KeyValue==	Key2_ClickSingle_2)
{
	 switch (JBE.Color)
  {
	 case White:
		JBE.Color=Red;
   break;
	 case Red:
		JBE.Color=Green;
   break;
   case Green:
		JBE.Color=Blue;
   break;
   case Blue:
		JBE.Color=Yellow;
   break;
   case Yellow:
		JBE.Color=Purple;
   break;
   case Purple:
		JBE.Color=Orange;
   break;
   case Orange:
		JBE.Color=Cyan;
   break;
   case Cyan:
		JBE.Color=White;
	 break;
	 default:
	 break;
  }
	/*
	if(JBE.Light_Sw==Light_off)
	{  
		JBE.Light_Sw=Light_on;
	}
	else if(JBE.Light_Sw==Light_on)
	{  
		JBE.Light_Sw=Light_off;
	}
	*/
}
else if(JBE.KeyValue==	Key2_ClickSingle_3)
{

}
else if(JBE.KeyValue==	Key2_LongPress)
{
if(JBE.Light_Sw==Light_off)
	{  
		JBE.Light_Sw=Light_on;
	}
	else if(JBE.Light_Sw==Light_on)
	{  
		JBE.Light_Sw=Light_off;
	}
	
}

JBE.KeyValue=Key_Null; 
}

uint8_t Get_Light_Sta(void)
{
 return JBE.Light_Sw;
}	

uint8_t Get_Bell_Sta(void)
{
 return JBE.Bell_Sw;
}	

uint8_t Get_Unit_Sta(void)
{
 return JBE.Unit_Sw;
}	

Color_s Get_Color_Sta(void)
{
 return JBE.Color;
}	

Co2DisMod_s Get_Co2DisMod(void)  //函数返回二氧化碳显示模式
{
return JBE.Co2DisMod;

}

uint8_t Get_Wifi_Key_Sta(void)
{
return JBE.Wifi_Key_Sta;
}
