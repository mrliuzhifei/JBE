#ifndef _LED_H_
#define _LED_H_

#define BLU_PORT_R   Port3
#define BLU_PORT_GB  Port0
#define LAMP_PORT    Port5
#define BLU_R_PIN    BIT6
#define BLU_G_PIN    BIT5
#define BLU_B_PIN    BIT4
#define LAMP_R_PIN   BIT4
#define LAMP_G_PIN   BIT5
#define LAMP_B_PIN   BIT7

#define BLU_ON()      P04=1;P05=1;P36=1  //可以为寻址
#define BLU_OFF()     P04=0;P05=0;P36=0  //可以为寻址
#define LAMP_GREEN_ON()  SFRS=0; P5&=0x4F; P5|=0x20   //不可位寻址
#define LAMP_YELLOW_ON() SFRS=0; P5&=0x4F; P5|=0x30
#define LAMP_RED_ON()    SFRS=0; P5&=0x4F; P5|=0x10
#define LAMP_OFF()       SFRS=0; P5&=0x4f   //不可位寻址
void LED_Init(void);
typedef enum Color_s	
{
	 White ,                 //白色
	 Red,                    //红色
	 Green,                  //绿色
	 Blue,                   //蓝色
	 Yellow,                 //黄色
	 Purple,                 //紫色
   Orange,                 //橙色
	 Cyan,	                 //青色
}Color_s	;

void RGB_Color(Color_s	RGBColor);
void LED_Buz(void);
void LED_FLASH(unsigned char Times);
#endif