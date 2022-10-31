#ifndef _CO2_H_
#define _CO2_H_
#include "KEY.h"
typedef enum CO2_Cmd_s	
{
	 R_CO2Valu=0x01,                 //读取CO2的值
	 Calibrate_Single=0x03,          //单点校正
	 R_Version_Num=0x1e,             //读固件版本号
	 OnOff_Self_calibrate0=0x10,     //开启和关闭零点自校准，以及零点自校准参数设置
	 R_Serial_Num=0x1f,             //读仪器编号
	
}CO2_Cmd_s;

typedef enum Uart1_Sta_s	
{
	uart_state_wait,                //等待状态
	uart_state_recievebusy,         //接收忙
	uart_state_recievefinish,       //接收完成状态
	uart_state_send_Coldata,        //
  uart_state_send_Paradata,       //
  uart_state_send_CommandReturn,  //
}Uart1_Sta_s;
void Get_CO2Valu(void);
void Get_CO2_VersionNum(void);
void Recieve_Data_Analysis(void);
uint8_t Get_Cs(uint8_t Lens);    //获取校验值
uint16_t Get_Co2Value(void);
void Display_Co2(Co2DisMod_s Dis_Mod);//co2显示

#endif

