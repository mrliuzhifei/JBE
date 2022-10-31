#ifndef _EEPROM_USER_H_
#define _EEPROM_USER_H_

#define BaseAddress 0xfe80
#define BaseAddress_Page 0xff80
#define SaveFlagAddr 0xff86
typedef enum EEPROM_SaveType_s	
{
	 CO2,                     //二氧化碳
	 SHTC3,                   //温湿度
	 HCHO,                    //甲醛
	 PM25,                    //PM2.5
	 TVCO,                    //一氧化碳
}EEPROM_SaveType_s;

typedef enum DataType_s	
{
	 PAGE,                     //页
	 Sensor,                   //传感器数据
}DataType_s	;


UINT8 Get_OneCircleSaveFlag(void);
void EEPROMSAVE( EEPROM_SaveType_s SaveType,uint16_t WriteData );
void Write_DATAFLASH (unsigned int u16EPAddr, unsigned char *u8EPData,unsigned char len);
void EEPROM_Write_SensorData(EEPROM_SaveType_s SaveType, DataType_s DataType);
extern volatile unsigned char xdata page_buffer[128];
void EEPROM_ReadDataInit(void);
void Get_Max_Valu_24h(void);
void Get_Avg_Valu_24h(void);
unsigned int Get_Avg_Valu_CO2_24h(void);
unsigned int Get_Max_Valu_CO2_24h(void);
#endif



