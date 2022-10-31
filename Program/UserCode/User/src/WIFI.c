#include "ML51.h"
typedef struct DeviceData_t	   //设备数据
{
	uint8_t  Data_Length;              //数据长度
	uint8_t  Command_Word;             //命令字
	uint8_t  GateWay_ID;               //网关ID
	uint16_t Device_ID;                //设备ID
	uint16_t Device_SN;                //设备SN码
	uint8_t  Protocol_Versions;        //协议版本号
	uint8_t  Device_Type;              //设备类型
	uint8_t  Device_Num;               //设备型号
	uint8_t Software_Versions ;        //软件版本
	uint8_t Hardware_Versions ;        //硬件版本号
	uint8_t Customer_ID;               //客户ID
	uint8_t Produc_Date;               //生产日期
}DeviceData_t;



typedef struct Sensor_Data_t	   //传感器数据
{
	uint16_t Temp;               //温度
	uint16_t Humi;               //湿度
	uint16_t CO2_PecentMax;      //二氧化碳浓度最大值
	uint16_t CO2_PecentAvr;      //二氧化碳浓度平均值
	uint16_t CO2_PecentCurrent;  //二氧化碳浓度当前值
}Sensor_Data_t;





typedef struct WIFICmdData_t   //WIFI命令字
{
	uint8_t Buzzer_OnOff;        //蜂鸣器开关
	uint8_t Blu_OnOff;           //背光开关	
}WIFICmdData_t;	

typedef struct WIFIData_t   //WIFI命令字
{
	DeviceData_t  DeviceData ;        //蜂鸣器开关
	WIFICmdData_t WIFICmdData;           //背光开关	
}WIFIData_t;	

void Init_Wifi(void)
{
	GPIO_SetMode(WIFI_EN_PORT,WIFI_EN_PIN,GPIO_MODE_PUSHPULL);//设置为推挽输出
	WIFI_Disable(); 
}