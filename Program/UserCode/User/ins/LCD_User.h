#ifndef _LCD_USER_H_
#define _LCD_USER_H_

#define LCD_COM0 0x01
#define LCD_COM1 0x02
#define LCD_COM2 0x04
#define LCD_COM3 0x08
#define TEMP     0
#define HUB      1
#define SEG_CO2 0

void Init_Lcd(void);
void Lcd_Display(void);
void LCD_Display_TempHub(uint16_t TValue,uint16_t HValue);
void ShowDigit(UINT8 digitPlace, UINT8 num, UINT8 tempUnit); 





#endif