#ifndef _Timer0_H
#define True 1
#define False 0

#define MeasurDlayTime   20 //  测量时间20ms
void Timer0_Init(void);

typedef enum Sys_Task_s	//
{
	 Task_Idle,
	 Task_Key_event,
	 Task_Measure_TempHut,
	 Task_Measure_CO2,
	 Task_LcdDisplay,
	
}Sys_Task_s;

typedef enum SavePage_s	//
{
	 CO2_Page1,
	 CO2_Page2,
	 SHTC3_Page1,
	 SHTC3_Page2,	
}SavePage_s	;

typedef enum TempHub_Sta_s	   //温湿度状态
{
 unstable,
 stable,	
} TempHub_Sta_s	;

UINT8 Get_TempHutStartFlag(void);

void Clr__TempHutStartFlag(void);
void Time_Process(void);
void Set_Save_AddrP(UINT8 Addr);
void Set_OneCircle_Flag(UINT8 Flag);
UINT8 Get_SaveAddr(void);         //获得存储地址
UINT16 GET_NtcTEMP(void);
TempHub_Sta_s GET_TempHub_Sta(void);
extern uint16_t Temp_Delt;
extern uint16_t Hub_Delt;
extern uint16_t Delt_last;





#endif
