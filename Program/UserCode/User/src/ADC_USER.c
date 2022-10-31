#include "ML51.h"
/*NTC查表数据*/
const unsigned int  NTC_ADCValue[81]={
	                                   3950,3890,3829,3766,3703,3638,3572,3506,3439,3371,
                                     3302,3233,3164,3094,3024,2954,2884,2814,2745,2676,
                                     2607,2539,2471,2403,2337,2272,2207,2144,2081,2019,
                                     1959,1899,1841,1784,1728,1674,1620,1569,1518,1469,
	                                   1421,1374,1328,1284,1241,1200,1159,1120,1082,1046,
	                                   1010, 976, 941, 910, 879, 848, 820, 791, 764, 737,
                                      712, 687, 664, 641, 619, 599, 578, 558, 539, 521,
	                                    503, 486, 470, 454, 438, 424, 410, 396, 383, 370,
	                                    357	
                                    } ;

typedef struct NTC_Data_s
{
	uint16_t Min_NTC_Val;          //NTC最大值
	uint16_t Max_NTC_Val;          //NTC最小值
	uint16_t NTC_Val_Sum;          //NTC16次的采样和
	uint16_t NTC_Val;              //NTC滤波后的值
	uint16_t NTC_Val_Last;         //NTC上一次的采样值
	uint16_t Adval_Ntc[18];	       //NTC采样后存储数组
  uint8_t  Adval_Ntc_Idx;        //NTC数组下标
	uint16_t NTC_Temp;             //NTC温度值
	uint16_t NTC_Init;             //NTC初始值
	uint8_t NTC_Temp_i;
	uint8_t IsFistGetTemp;
}NTC_Data_s;

typedef struct Bat_Data_s
{
	uint16_t Min_Bat_Val;          //Bat最大值
	uint16_t Max_Bat_Val;          //Bat最小值
	uint32_t Bat_Val_Sum;          //Bat16次的采样和
	uint16_t Bat_Val;              //Bat滤波后的值
	uint16_t Bat_Val_Last;         //Bat上一次的采样值
	uint16_t Adval_Bat[18];	       //Bat采样后存储数组
  uint8_t  Adval_Bat_Idx;        //Bat数组下标
	uint32_t Bat_Voltage;          //Bat电压值
	uint8_t  IsNeedCheckBatvolt;   //是否需要检测电池电压
	BatMod_s BatMod;               //电池模式
	uint8_t  BatPowerPecent;       //电量百分比
	//uint16_t NTC_Init;             Bat初始值
	//uint8_t NTC_Temp_i;
	//uint8_t IsFistGetTemp;
}Bat_Data_s;

typedef struct ADC_Data_s
{
NTC_Data_s NTC_Data;
Bat_Data_s Bat_Data;	
uint8_t Is_Bat_ChargFull;
}ADC_Data_s;

ADC_Data_s ADC_Data;
#define JBE ADC_Data
	
void ADC_Init(void)
{
ENABLE_ADC_CH4;	
ADC_Open(ADC_SINGLE,ADC_CHANNEL_4 );     //单次转换。通道4通道。
ADC_SamplingTime(3,7);                   //设置采样时间
ADC_Vref_Set(Enable,Enable,Level_2_560V);//设置内部采样基准电压
clr_ADCCON0_ADCF;               	       //转换完成标志位清零
GPIO_SetMode(BatVoltCheck_PORT ,BatVoltCheck_PIN,GPIO_MODE_PUSHPULL);	
P24_QUASI_MODE;                          //P2.4设置为准双向模式，检测是否有充电
//ENABLE_P63_PULLUP;
BatVoltCheck_ON;
JBE.NTC_Data.IsFistGetTemp=True;	
JBE.Bat_Data.IsNeedCheckBatvolt=False;
}

uint32_t Get_NtcValu(void)
{  
	  uint32_t ADCdataAIN;
	  uint32_t Ntc_valu;
    clr_ADCCON0_ADCF;
    set_ADCCON0_ADCS;                  // ADC start trig signal
    while(!(ADCCON0&SET_BIT7));
    ADCdataAIN = (ADCRH<<4)|ADCRL;
	  JBE.NTC_Data.Adval_Ntc[JBE.NTC_Data.Adval_Ntc_Idx]= ADCdataAIN;
	  JBE.NTC_Data.Adval_Ntc_Idx++;
	  Ntc_valu=ADCdataAIN;
    clr_ADCCON0_ADCF;
    clr_ADCCON0_ADCS;
    return Ntc_valu;
}

void ADC_RUN(uint8_t Channel)
{
    uint32_t ADCdataAIN;
	  if(Channel==Channel_Temp)
		{
		ENABLE_ADC_CH4;	
		clr_ADCCON0_ADCF;
    set_ADCCON0_ADCS;                  // ADC start trig signal
    while(!(ADCCON0&SET_BIT7));
    ADCdataAIN = (ADCRH<<4)|ADCRL;
	  JBE.NTC_Data.Adval_Ntc[JBE.NTC_Data.Adval_Ntc_Idx]= ADCdataAIN;
	  JBE.NTC_Data.Adval_Ntc_Idx++;
    clr_ADCCON0_ADCF;
    clr_ADCCON0_ADCS;
		}
		else if(Channel==Channel_BatVolt)
		{
		ENABLE_ADC_CH0;	
		clr_ADCCON0_ADCF;
    set_ADCCON0_ADCS;                  // ADC start trig signal
    while(!(ADCCON0&SET_BIT7));
    ADCdataAIN = (ADCRH<<4)|ADCRL;
	  JBE.Bat_Data.Adval_Bat[JBE.Bat_Data.Adval_Bat_Idx]= ADCdataAIN;
	  JBE.Bat_Data.Adval_Bat_Idx++;
    clr_ADCCON0_ADCF;
    clr_ADCCON0_ADCS;
		}
    
}

void Get_Temp_Bat(uint8_t Channel)
{
	 uint8_t i;
	 switch (Channel)
	 {
		 case Channel_Temp:
		 {
		 	 ADC_RUN(Channel_Temp);
			 if(JBE.NTC_Data.Adval_Ntc_Idx>=18)
			 {
			 JBE.NTC_Data.Adval_Ntc_Idx=0;
			 JBE.NTC_Data.Min_NTC_Val=JBE.NTC_Data.Adval_Ntc[0]; //给最大最小值赋初值
			 JBE.NTC_Data.Max_NTC_Val=JBE.NTC_Data.Adval_Ntc[0]; 
			 JBE.NTC_Data.NTC_Val_Sum=0;                //给求和变量清零
			 for(i= 1;i<18;i++)		           //找出最大最小值
			 {
				if(JBE.NTC_Data.Max_NTC_Val<JBE.NTC_Data.Adval_Ntc[i])
					 JBE.NTC_Data.Max_NTC_Val=JBE.NTC_Data.Adval_Ntc[i];
				if(JBE.NTC_Data.Min_NTC_Val>JBE.NTC_Data.Adval_Ntc[i])
					 JBE.NTC_Data.Min_NTC_Val=JBE.NTC_Data.Adval_Ntc[i];        
			 }
				for(i= 0;i< 18;i++)		  //求和
				{
					JBE.NTC_Data.NTC_Val_Sum+=JBE.NTC_Data.Adval_Ntc[i];    
				}
				JBE.NTC_Data.NTC_Val_Sum =JBE.NTC_Data.NTC_Val_Sum -JBE.NTC_Data.Min_NTC_Val -JBE.NTC_Data.Max_NTC_Val;					  //减去最大最小值
				JBE.NTC_Data.NTC_Val_Sum>>= 4;					                                                //采样值已舍去最低两位，这里除以4
				JBE.NTC_Data.NTC_Val=JBE.NTC_Data.NTC_Val_Sum;
				JBE.NTC_Data.NTC_Val=0.9*JBE.NTC_Data.NTC_Val_Sum+0.1*JBE.NTC_Data.NTC_Val_Last;
				JBE.NTC_Data.NTC_Val_Last=JBE.NTC_Data.NTC_Val;	
				for(i=0;i<80;i++)                                                            //查表确定温度
				{
				if((JBE.NTC_Data.NTC_Val<=NTC_ADCValue[i])&&(JBE.NTC_Data.NTC_Val>NTC_ADCValue[i+1]))           //得到温度范围
				{
				 if(i<20)                                                                     //是负温度
				 {
					JBE.NTC_Data.NTC_Temp=(19-i)*10+10*(NTC_ADCValue[i]-JBE.NTC_Data.NTC_Val)/(NTC_ADCValue[i]-NTC_ADCValue[i+1]);
					
				 }
				 else                                                                         //是正温度
				 {
				 JBE.NTC_Data.NTC_Temp=(i-20)*10+(10*(NTC_ADCValue[i]-JBE.NTC_Data.NTC_Val))/(NTC_ADCValue[i]-NTC_ADCValue[i+1]);
				 JBE.NTC_Data.NTC_Temp_i=i;
				 JBE.NTC_Data.NTC_Temp_i=i;
				 return; 
				 }
				}
			 } 
		 }
		 break;
		 }
		 case  Channel_BatVolt:
		 {   
			   uint8_t i;
		 	 	ADC_RUN(Channel_BatVolt);
			 if(JBE.Bat_Data.Adval_Bat_Idx>=18)
			 {
			 JBE.Bat_Data.Adval_Bat_Idx=0;
			 JBE.Bat_Data.Min_Bat_Val=JBE.Bat_Data.Adval_Bat[0]; //给最大最小值赋初值
			 JBE.Bat_Data.Max_Bat_Val=JBE.Bat_Data.Adval_Bat[0]; 
			 JBE.Bat_Data.Bat_Val_Sum=0;                //给求和变量清零
			 for(i= 1;i<18;i++)		           //找出最大最小值
			 {
				if(JBE.Bat_Data.Max_Bat_Val<JBE.Bat_Data.Adval_Bat[i])
					 JBE.Bat_Data.Max_Bat_Val=JBE.Bat_Data.Adval_Bat[i];
				if(JBE.Bat_Data.Min_Bat_Val>JBE.Bat_Data.Adval_Bat[i])
					 JBE.Bat_Data.Min_Bat_Val=JBE.Bat_Data.Adval_Bat[i];        
			 }
				for(i= 0;i< 18;i++)		  //求和
				{
					JBE.Bat_Data.Bat_Val_Sum+=JBE.Bat_Data.Adval_Bat[i];    
				}
				JBE.Bat_Data.Bat_Val_Sum =JBE.Bat_Data.Bat_Val_Sum -JBE.Bat_Data.Min_Bat_Val -JBE.Bat_Data.Max_Bat_Val;					  //减去最大最小值
				JBE.Bat_Data.Bat_Val_Sum>>= 4;					                                                                          //采样值已舍去最低两位，这里除以4
				//JBE.Bat_Data.Bat_Val=JBE.Bat_Data.Bat_Val_Sum;
				JBE.Bat_Data.Bat_Val_Sum=0.9*JBE.Bat_Data.Bat_Val_Sum+0.1*JBE.Bat_Data.Bat_Val_Last;
				JBE.Bat_Data.Bat_Val_Last=JBE.Bat_Data.Bat_Val_Sum;	
				JBE.Bat_Data.Bat_Val=JBE.Bat_Data.Bat_Val_Sum*256*20/4095;
				
				
		 }
		 break;
		 
		 
		 }
	 
	 
	 }

	

}

void Bat_PowerDisplay(void)
{  
	 static uint16_t lowpowertime=0;
  if(IsChargOn==True)          //如果正在充电
	{
	 LCD_SetOnePixel(1, 22, Enable);  //显示充电图标
	 LCD_SetOnePixel(3, 23, Enable);
	 if(JBE.Bat_Data.Bat_Val>4100)   //电压大于4v表示充满了
	 {
		JBE.Is_Bat_ChargFull=True;
	  LCD_SetOnePixel(0, 22, Enable); 
	  LCD_SetOnePixel(0, 23, Enable);
		LCD_SetOnePixel(1, 23, Enable);
		LCD_SetOnePixel(2, 23, Enable); 
	 
	 }
	 /*两种情况1.没有充满的情况下小于4.1v，显示在充电.2.已经充满一次的情况下如果电量掉到3.9v后再次显示充电*/
	 else if((JBE.Bat_Data.Bat_Val<4100&&JBE.Is_Bat_ChargFull==False)&&(JBE.Bat_Data.Bat_Val<3900&&JBE.Is_Bat_ChargFull==True))
	 {
	 lowpowertime++;
   if(lowpowertime==1)
	 {
	  LCD_SetOnePixel(0, 22, Enable);
	  LCD_SetOnePixel(0, 23, Enable);
		LCD_SetOnePixel(2, 23, Disable);
		LCD_SetOnePixel(1, 23, Disable); 
	 }
	 else if(lowpowertime==2)
	 {
		LCD_SetOnePixel(0, 22, Enable); 
	  LCD_SetOnePixel(0, 23, Enable);
		LCD_SetOnePixel(1, 23, Enable);
		LCD_SetOnePixel(2, 23, Disable);
	 }
	 else if(lowpowertime==3)
	 {
		LCD_SetOnePixel(0, 22, Enable); 
	  LCD_SetOnePixel(0, 23, Enable);
		LCD_SetOnePixel(1, 23, Enable);
		LCD_SetOnePixel(2, 23, Enable); 
	 }
	 else 
	 {
	  LCD_SetOnePixel(0, 22, Enable);
	  LCD_SetOnePixel(2, 23, Disable);
		LCD_SetOnePixel(1, 23, Disable);
		LCD_SetOnePixel(0, 23, Disable); 
	  lowpowertime=0;
	 }
  }	 
	if(JBE.Bat_Data.Bat_Val<3681)	  
	{
	 JBE.Bat_Data.BatPowerPecent=0;
	}
	else if(JBE.Bat_Data.Bat_Val<3743)
	{
	JBE.Bat_Data.BatPowerPecent=20;
	}
	else if(JBE.Bat_Data.Bat_Val<3824)
	{
	JBE.Bat_Data.BatPowerPecent=40;
	}
	else if(JBE.Bat_Data.Bat_Val<3981)
	{
	JBE.Bat_Data.BatPowerPecent=60;
	}
	else if(JBE.Bat_Data.Bat_Val<4150)
	{
	JBE.Bat_Data.BatPowerPecent=80;
	}
	else
	{
	JBE.Bat_Data.BatPowerPecent=100;
	}	 
	}
	else if(IsChargOn==False)    //如果正在放电
	{
	 static uint8_t Pecent80Flag=0;
	 static uint8_t Pecent60Flag=0;
	 static uint8_t Pecent40Flag=0;
	 static uint8_t Pecent20Flag=0;
	 static uint8_t Pecent0Flag=0;
	 JBE.Is_Bat_ChargFull=False;   //充电器拔掉充满标志位清零
	 LCD_SetOnePixel(1, 22, Disable);  //关闭充电图标
	 	
		
	 if(JBE.Bat_Data.Bat_Val>3900)
	 {
		if(Pecent80Flag==0)
		{
		JBE.Bat_Data.BatPowerPecent=100;
		LCD_SetOnePixel(0, 22, Enable);
		LCD_SetOnePixel(2, 23, Enable);
		LCD_SetOnePixel(3, 23, Enable);
		LCD_SetOnePixel(1, 23, Enable);
		LCD_SetOnePixel(0, 23, Enable);
		}
		else if(Pecent80Flag==1&&JBE.Bat_Data.Bat_Val>3920)
		{
		JBE.Bat_Data.BatPowerPecent=100;
		LCD_SetOnePixel(0, 22, Enable);
		LCD_SetOnePixel(2, 23, Enable);
		LCD_SetOnePixel(3, 23, Enable);
		LCD_SetOnePixel(1, 23, Enable);
		LCD_SetOnePixel(0, 23, Enable);
		Pecent80Flag=0;
		Pecent60Flag=0;
		Pecent40Flag=0;
		Pecent20Flag=0;
		Pecent0Flag=0;	
		}
		else
		{
		JBE.Bat_Data.BatPowerPecent=80;
	  LCD_SetOnePixel(0, 22, Enable);
	  LCD_SetOnePixel(2, 23, Disable);
	  LCD_SetOnePixel(3, 23, Enable);
	  LCD_SetOnePixel(1, 23, Enable);
	  LCD_SetOnePixel(0, 23, Enable);		
		}
	
	 }
	 else if(JBE.Bat_Data.Bat_Val>3705)
	 {
	   Pecent80Flag=1;
	 if(Pecent60Flag==0)
		 {
		 JBE.Bat_Data.BatPowerPecent=80;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Enable);
		 LCD_SetOnePixel(0, 23, Enable);	 
		 }
	 else if(Pecent60Flag==1&&JBE.Bat_Data.Bat_Val>3725)
		 {
		 JBE.Bat_Data.BatPowerPecent=80;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Enable);
		 LCD_SetOnePixel(0, 23, Enable);
		 //Pecent80Flag=0;
		 Pecent60Flag=0;
		 Pecent40Flag=0;
		 Pecent20Flag=0;
		 Pecent0Flag=0;	
		 }
	 else
		 {
		 JBE.Bat_Data.BatPowerPecent=60;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Enable);  
		 }

	 }
	 else if(JBE.Bat_Data.Bat_Val>3600)
	 {
		Pecent60Flag=1; 
		if(Pecent40Flag==0) 
		{
		 JBE.Bat_Data.BatPowerPecent=60;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Enable);		
		}
		else if(Pecent40Flag==1&&JBE.Bat_Data.Bat_Val>3610)
		{
		 JBE.Bat_Data.BatPowerPecent=60;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Enable);
		 Pecent40Flag=0;
		 Pecent20Flag=0;
		 Pecent0Flag=0;	
		}
		else
		{
		JBE.Bat_Data.BatPowerPecent=40;
	  LCD_SetOnePixel(0, 22, Enable);
	  LCD_SetOnePixel(2, 23, Disable);
	  LCD_SetOnePixel(3, 23, Enable);
	  LCD_SetOnePixel(1, 23, Disable);
	  LCD_SetOnePixel(0, 23, Disable);	
		}
 
	 }
	 else if(JBE.Bat_Data.Bat_Val>3528)
	 {
	 Pecent40Flag=1; 
	 if(Pecent20Flag==0)
	 {
		 JBE.Bat_Data.BatPowerPecent=40;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Disable);	 
	 }
	 else if(Pecent20Flag==1&&JBE.Bat_Data.Bat_Val>3550)
	 {
	   JBE.Bat_Data.BatPowerPecent=40;
		 LCD_SetOnePixel(0, 22, Enable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Disable);
		 Pecent20Flag=0;
		 Pecent0Flag=0;	
	 }
	 else
	 {
	  JBE.Bat_Data.BatPowerPecent=20;
	  LCD_SetOnePixel(0, 22, Disable);
	  LCD_SetOnePixel(2, 23, Disable);
	  LCD_SetOnePixel(3, 23, Enable);
	  LCD_SetOnePixel(1, 23, Disable);
	  LCD_SetOnePixel(0, 23, Disable);	 
	 }
	
	 }
	 else if(JBE.Bat_Data.Bat_Val>3120)
	 {
	 Pecent20Flag=1;
	 if(Pecent0Flag==0)
	 {
		 JBE.Bat_Data.BatPowerPecent=20;
		 LCD_SetOnePixel(0, 22, Disable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Disable);
	 }
	 else if(Pecent0Flag==1&&JBE.Bat_Data.Bat_Val>3140)
	 {
	   JBE.Bat_Data.BatPowerPecent=20;
		 LCD_SetOnePixel(0, 22, Disable);
		 LCD_SetOnePixel(2, 23, Disable);
		 LCD_SetOnePixel(3, 23, Enable);
		 LCD_SetOnePixel(1, 23, Disable);
		 LCD_SetOnePixel(0, 23, Disable);
		 Pecent0Flag=0;
	 }
	 else
	 {
	 lowpowertime++; 
	 JBE.Bat_Data.BatPowerPecent=0;
	 LCD_SetOnePixel(0, 22, Disable);
	 LCD_SetOnePixel(2, 23, Disable);
	 //LCD_SetOnePixel(3, 23, Enable);
	 LCD_SetOnePixel(1, 23, Disable);
	 LCD_SetOnePixel(0, 23, Disable);
	 if(lowpowertime==1)
	 {
	  LCD_SetOnePixel(1, 22, Enable);  //显示充电图标
	  LCD_SetOnePixel(3, 23, Enable);
	 }
	 else if(lowpowertime==2)
	 {
	  LCD_SetOnePixel(1, 22, Disable); 
	  LCD_SetOnePixel(3, 23, Disable);
		lowpowertime=0; 
	 }
	 }
	 }
	 else 
	 {
	 Pecent0Flag=1;
	 lowpowertime++;
	 JBE.Bat_Data.BatPowerPecent=0;
	 LCD_SetOnePixel(0, 22, Disable);
	 LCD_SetOnePixel(2, 23, Disable);
	 //LCD_SetOnePixel(3, 23, Enable);
	 LCD_SetOnePixel(1, 23, Disable);
	 LCD_SetOnePixel(0, 23, Disable);
	 if(lowpowertime==1)
	 {
	  LCD_SetOnePixel(1, 22, Enable);  //显示充电图标
	  LCD_SetOnePixel(3, 23, Enable);
	 }
	 else if(lowpowertime==2)
	 {
	  LCD_SetOnePixel(1, 22, Disable); 
	  LCD_SetOnePixel(3, 23, Disable);
		lowpowertime=0; 
	 }
	  //if(JBE.Bat_Data.Bat_Val<3000)低电量关机    后面补充
	 }
	}

}
 
uint16_t Get_NTC_Temp(void)
{
if(JBE.NTC_Data.IsFistGetTemp==True)
{
JBE.NTC_Data.NTC_Init=JBE.NTC_Data.NTC_Temp;
JBE.NTC_Data.IsFistGetTemp=False;
}
return JBE.NTC_Data.NTC_Temp;
}

uint16_t Get_NTC_Init(void)
{
return JBE.NTC_Data.NTC_Init;

}	
