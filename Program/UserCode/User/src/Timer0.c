#include "ML51.h"
 uint16_t Temp_Delt=0;
 uint16_t Hub_Delt=0;
 uint16_t Delt_last=0;	
 uint8_t  OutTempIsHigh=0;
typedef struct Sys_TimeCount_t
{
UINT8 count1ms;
UINT8 count10ms;	
UINT8 count20ms;	
UINT8 count30ms;	
UINT8 count40ms;
UINT8 count50ms;
UINT8	count100ms;
UINT8	count200ms;	
UINT16	count500ms;
UINT16	count1s;
UINT16	count5s;	
}Sys_TimeCount_t;

typedef struct TimeEvent_t
{
UINT8 Time_1ms_Event;
UINT8 Time_10ms_Event;	
UINT8 Time_20ms_Event;	
UINT8 Time_30ms_Event;
UINT8 Time_40ms_Event;	
UINT8 Time_50ms_Event;	
UINT8 Time_100ms_Event;	
UINT8 Time_200ms_Event;
UINT8 Time_500ms_Event;	
UINT8 Time_1s_Event;
UINT8 Time_5s_Event;	
}TimeEvent_t;

typedef struct Sys_TimePar_t
{
 Sys_TimeCount_t Sys_TimeCount;
 TimeEvent_t TimeEvent_t;
 UINT8 IsTempHut_MeasureStart;
 UINT8 Sys_Second;
 UINT8 Sys_Minute;
 UINT8 IsNeedCO2Save;              //�Ƿ���Ҫ��CO2��ֵ
 UINT8 ISNeedPageSave;             //�Ƿ�洢��ǰҳ��ַ
 UINT8 SaveAddr_P;
 UINT8 ISOneCircleSave;  	         //�Ƿ����һҳ
 UINT16 NTC_Value;	
 UINT16 Temp ;       
}Sys_TimePar_t;                 
Sys_TimePar_t Sys_TimePar;
#define JBE Sys_TimePar


/* if define TIMER0_FSYS_DIV12, timer = (0x1FFF-0x1000)*12/24MHz = 4.08ms */
/* if define TIMER0_FSYS, timer = (0x1FFF-0x0010)/24MHz = 340us */
#define TH0_INIT        0xC1
#define TL0_INIT        0x0F

void Timer0_Init(void)
{
    ENABLE_TIMER0_MODE0;                        //Timer 0 mode configuration
    TIMER0_FSYS_DIV12;
	  SFRS=0;
    TH0 = TH0_INIT;                             //��ʱ1ms
    TL0 = TL0_INIT;  
    ENABLE_TIMER0_INTERRUPT;                    //enable Timer0 interrupt
    ENABLE_GLOBAL_INTERRUPT;                    //enable interrupts
    set_TCON_TR0;  
}



void Timer0_ISR (void) interrupt 1           //interrupt address is 0x000B
{
    _push_(SFRS);
    SFRS=0;
    TH0 = TH0_INIT;
    TL0 = TL0_INIT;
    TF0 = 0 ;
    JBE.Sys_TimeCount.count1ms++;
		JBE.Sys_TimeCount.count10ms++;
		JBE.Sys_TimeCount.count20ms++;
		JBE.Sys_TimeCount.count30ms++;
		JBE.Sys_TimeCount.count40ms++;
		JBE.Sys_TimeCount.count50ms++;
		JBE.Sys_TimeCount.count100ms++;
		JBE.Sys_TimeCount.count200ms++;
		JBE.Sys_TimeCount.count500ms++;
		JBE.Sys_TimeCount.count1s++;	
    JBE.Sys_TimeCount.count5s++;	
	 if(JBE.Sys_TimeCount.count1ms>=1)
	 {
	 JBE.TimeEvent_t.Time_1ms_Event=True ;
	 JBE.Sys_TimeCount.count1ms=0;
	 }
	 if(JBE.Sys_TimeCount.count10ms>=10)
	 {
	 JBE.TimeEvent_t.Time_10ms_Event=True ;
	 JBE.Sys_TimeCount.count10ms=0;
	 }
	 if(JBE.Sys_TimeCount.count20ms>=20)
	 {
	 JBE.TimeEvent_t.Time_20ms_Event=True ;
	 JBE.Sys_TimeCount.count20ms=0;
	 }
	 if(JBE.Sys_TimeCount.count30ms>=30)
	 {
	 JBE.TimeEvent_t.Time_30ms_Event=True ;
	 JBE.Sys_TimeCount.count30ms=0;
	 }
	 if(JBE.Sys_TimeCount.count40ms>=40)
	 {
	 JBE.TimeEvent_t.Time_40ms_Event=True ;
	 JBE.Sys_TimeCount.count40ms=0;
	 }
	 if(JBE.Sys_TimeCount.count50ms>=50)
	 {
	 JBE.TimeEvent_t.Time_50ms_Event=True ;
	 JBE.Sys_TimeCount.count50ms=0;
	 }
	if(JBE.Sys_TimeCount.count100ms>=100)
	 {
	 JBE.TimeEvent_t.Time_100ms_Event=True ;
	 JBE.Sys_TimeCount.count100ms=0;
	 }
	 if(JBE.Sys_TimeCount.count200ms>=200)
	 {
	 JBE.TimeEvent_t.Time_200ms_Event=True ;
	 JBE.Sys_TimeCount.count200ms=0;
	 }
	 if(JBE.Sys_TimeCount.count500ms>=500)
	 {
	 JBE.TimeEvent_t.Time_500ms_Event=True ;
	 JBE.Sys_TimeCount.count500ms=0;
	 }
	 if(JBE.Sys_TimeCount.count1s>=1000)
	 {
	 JBE.TimeEvent_t.Time_1s_Event=True ;
	 JBE.Sys_TimeCount.count1s=0;
	 }
	 if(JBE.Sys_TimeCount.count5s>=5000)
	 {
	 JBE.TimeEvent_t.Time_5s_Event=True ;
	 JBE.Sys_TimeCount.count5s=0;
	 }
    //P5 = ~P5;                              // GPIO toggle when interrupt   
    _pop_(SFRS);
}	
  
void Time_Process(void)
{
if(JBE.TimeEvent_t.Time_1ms_Event==True)
{
if(JBE.IsTempHut_MeasureStart==True)
{
Get_TempHut();
//JBE.IsTempHut_MeasureStart=False;
}
//Bat_PowerDisplay();
Recieve_Data_Analysis();
JBE.TimeEvent_t.Time_1ms_Event=False;

}
else if(JBE.TimeEvent_t.Time_10ms_Event==True)
{
 
 //Lcd_Display();
KeyProcess();
//.LED_Buz();
RGB_Color(Get_Color_Sta());
JBE.TimeEvent_t.Time_10ms_Event=False;
}
else if(JBE.TimeEvent_t.Time_20ms_Event==True)
{
//if(JBE.IsTempHut_MeasureStart==True)Get_TempHut();
  Get_Temp_Bat(Channel_Temp);
	Get_Temp_Bat(Channel_BatVolt);

if(JBE.IsNeedCO2Save==True)
{
	 if(JBE.ISNeedPageSave==True)
	 {
	 EEPROM_Write_SensorData(CO2,PAGE);
	 JBE.ISNeedPageSave=False;
	 }
   EEPROM_Write_SensorData(CO2,Sensor);	
	 Get_Max_Valu_24h();
	 Get_Avg_Valu_24h();
   JBE.SaveAddr_P=JBE.SaveAddr_P+2;	
	 if(JBE.SaveAddr_P>95)
	 {
	 JBE.ISNeedPageSave=True;		
   JBE.ISOneCircleSave=True;	 
	 JBE.SaveAddr_P=0;
	 }	 
	 JBE.IsNeedCO2Save=False;
}

JBE.TimeEvent_t.Time_20ms_Event=False;
}	


else if(JBE.TimeEvent_t.Time_1s_Event==True)
{
	
static uint8_t time_cyc;
static uint16_t Temp_Delt_last;

//LCD_Display_TempHub(Get_TempValue(),TEMP);
//EEPROM_Write_SensorData(CO2, PAGE);	
JBE.Temp=Get_NTC_Temp();
JBE.IsTempHut_MeasureStart=True;

	
//	if(JBE.ISOneCircleSave==0&&JBE.SaveAddr_P==0)JBE.IsNeedCO2Save=True;	//�ո��ϵ��Դ�һ�ζ�����ֵ̼
	
JBE.Sys_Second++;
if(JBE.Sys_Second>=60)	
{
 JBE.Sys_Minute++;
 time_cyc++;
 Minit_Times++;	
 Minit_Times_Hub++;
 if(Get_TempValueMid()>Get_TempValueInit())
 {
 Temp_Delt=Get_TempValueMid()-Get_TempValueInit();
 }
 
 if(Get_Hub_value_Inti()>Get_Hub_value_Mid())
 {
 Hub_Delt=Get_Hub_value_Inti()-Get_Hub_value_Mid();
 }
 else Hub_Delt=Get_Hub_value_Mid()-Get_Hub_value_Inti();
 
 if(time_cyc==1)
 {
 Temp_Delt_last=Temp_Delt;
 }
 else if(time_cyc==10)
 {
 if(Temp_Delt>Temp_Delt_last)
 {
  Delt_last=Temp_Delt-Temp_Delt_last;
	if(Delt_last>15)OutTempIsHigh=1; 
 }
 else  Delt_last=Temp_Delt_last-Temp_Delt;
 
 time_cyc=0;
 }
 /*if( Minit_Times_Hub==10)
 {
 if(Hub_Delt<1)  Minit_Times_Hub=90;
 else if(Hub_Delt<3) Minit_Times_Hub=60;
 else if(Hub_Delt<5) Minit_Times_Hub=20;
 }*/
 //else Temp_Delt=0;

 if(Minit_Times==10)
 {
 //if(Temp_Delt<1) Minit_Times=90;
 //else if(Temp_Delt<10)Minit_Times=60;	 
 //else if(Temp_Delt<15)Minit_Times=40;
 }
if(Minit_Times>190)Minit_Times=190;
if(Minit_Times_Hub>90)Minit_Times_Hub=90;
JBE.Sys_Second=0; 
JBE.IsNeedCO2Save=True;
}


if(JBE.Sys_Minute>30)         //30���Ӵ�һ��
{
JBE.Sys_Minute=0;
JBE.IsNeedCO2Save=True;	

}

Get_CO2Valu();

JBE.TimeEvent_t.Time_1s_Event=False;
}
else if(JBE.TimeEvent_t.Time_5s_Event==True)
{

JBE.TimeEvent_t.Time_5s_Event=False;
}

}



UINT8 Get_TempHutStartFlag(void)
{
 return JBE.IsTempHut_MeasureStart;
}

void Clr__TempHutStartFlag(void)
{
JBE.IsTempHut_MeasureStart=False;
}

UINT8 Get_SaveAddr(void)
{
return JBE.SaveAddr_P;
}

UINT8 Get_OneCircleSaveFlag(void)
{
return JBE.ISOneCircleSave;
}

void Set_OneCircle_Flag(UINT8 Flag)
{
JBE.ISOneCircleSave=Flag;
}

void Set_Save_AddrP(UINT8 Addr)
{
JBE.SaveAddr_P=Addr;
}

UINT16 GET_NtcTEMP(void)
{
return JBE.Temp;
}
