#ifndef _KEY_H_
#define _KEY_H_



typedef enum Key_Value_s	//键值
{
	 Key_Null,              //无按键
	 Key1_ClickSingle,       //单击
	 Key1_ClickSingle_1,     //单击1次
	 Key1_ClickSingle_2,     //连续单击3次
	 Key1_ClickSingle_3,     //连续单击3次
	 Key1_LongPress,         //长按键
	
	 Key2_ClickSingle,       //单击
	 Key2_ClickSingle_1,     //单击1次
	 Key2_ClickSingle_2,     //连续单击3次
	 Key2_ClickSingle_3,     //连续单击3次
	 Key2_LongPress,         //长按键
}Key_Value_s;


typedef enum Light_Sw_s	   //灯开关
{
	 Light_off,                    
	 Light_on,
}Light_Sw_s	;

typedef enum Bell_Sw_s	   //铃声开关
{
	 Bell_off,                    
	 Bell_on,
}Bell_Sw_s		;

typedef enum Unit_Sw_s	   //单位切换
{
	 Unit_C,                    
	 Unit_F, 
}Unit_Sw_s	;


typedef enum Co2DisMod_s	   //二氧化碳显示模式
{
	 Current_Valu,             //当前值                
	 Max_Valu_24h,             //24小时最大值
	 Avg_Valu_24h,             //24小时平均值
}Co2DisMod_s	;


typedef enum Key_Task_s	//键值
{
	 Task_Null,              //无任务
	 Task_OpenBell, 
	 Task_CloseBell,
	 Task_OnLight,
	 Task_OffLight,
	 Task_SwitchUnit,
}Key_Task_s;

/*typedef enum NetWorkMode_s
{
	
 NULL,
 NetWork_Entry, //入网
 NetWork,

}NetWorkMode_s;*/


typedef enum DIS_MODE_e   //显示模式
{                   
  DisMode_Null,
} 
DIS_MODE_e;
void Get_KeyValue(void);
void KeyProcess(void);
void Init_KeyPort(void);
uint8_t Get_Light_Sta(void);
uint8_t Get_Bell_Sta(void);
uint8_t Get_Unit_Sta(void);
Color_s Get_Color_Sta(void);
Co2DisMod_s Get_Co2DisMod(void);
uint8_t Get_Wifi_Key_Sta(void);
#endif
