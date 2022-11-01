#ifndef _ADC_USER_H_
#define _ADC_USER_H_

#define BatVoltCheck_PORT    Port5
#define BatVoltCheck_PIN     BIT1
#define BatVoltCheck_OFF       SFRS=0; P5|=0x02   //????¦Ë??
#define BatVoltCheck_ON        SFRS=0; P5&=0xFD

#define Channel_Temp 4
#define Channel_BatVolt 0
void ADC_Init(void);
uint32_t Get_NtcValu(void);

void Get_Temp_Bat(uint8_t Channel);
uint16_t Get_NTC_Temp(void);
uint16_t Get_NTC_Init(void);
void Bat_PowerDisplay(void);
#define IsChargOn P24
//#define BAT_TEST_ON()     SFRS=0; P5&=0xFD; P5|=0x02
//#define BAT_TEST_OFF()    SFRS=0; P5&=0xFD

#define BAT_POWER_OFF()     P4&=0xFB; P4|=0x04
#define BAT_POWER_ON()    P4&=0xFB;

typedef enum BatMod_s	   //?????
{
	 Charg_Mode,           //?????       
	 DisCharg_Mode,        //?????
}BatMod_s		;


#endif
