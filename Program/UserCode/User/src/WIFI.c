#include "ML51.h"
typedef struct DeviceData_t	   //�豸����
{
	uint8_t  Data_Length;              //���ݳ���
	uint8_t  Command_Word;             //������
	uint8_t  GateWay_ID;               //����ID
	uint16_t Device_ID;                //�豸ID
	uint16_t Device_SN;                //�豸SN��
	uint8_t  Protocol_Versions;        //Э��汾��
	uint8_t  Device_Type;              //�豸����
	uint8_t  Device_Num;               //�豸�ͺ�
	uint8_t Software_Versions ;        //����汾
	uint8_t Hardware_Versions ;        //Ӳ���汾��
	uint8_t Customer_ID;               //�ͻ�ID
	uint8_t Produc_Date;               //��������
}DeviceData_t;



typedef struct Sensor_Data_t	   //����������
{
	uint16_t Temp;               //�¶�
	uint16_t Humi;               //ʪ��
	uint16_t CO2_PecentMax;      //������̼Ũ�����ֵ
	uint16_t CO2_PecentAvr;      //������̼Ũ��ƽ��ֵ
	uint16_t CO2_PecentCurrent;  //������̼Ũ�ȵ�ǰֵ
}Sensor_Data_t;





typedef struct WIFICmdData_t   //WIFI������
{
	uint8_t Buzzer_OnOff;        //����������
	uint8_t Blu_OnOff;           //���⿪��	
}WIFICmdData_t;	

typedef struct WIFIData_t   //WIFI������
{
	DeviceData_t  DeviceData ;        //����������
	WIFICmdData_t WIFICmdData;           //���⿪��	
}WIFIData_t;	

void Init_Wifi(void)
{
	GPIO_SetMode(WIFI_EN_PORT,WIFI_EN_PIN,GPIO_MODE_PUSHPULL);//����Ϊ�������
	WIFI_Disable(); 
}