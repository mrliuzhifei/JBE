#ifndef _SHTC3_H
#define _SHTC3_H

//#define test
#define print_function 
#define I2C_CLOCK                 13
#define I2C_SLAVE_ADDRESS         0xe0
#define I2C_WR                    0
#define I2C_RD                    1
#define LOOP_SIZE                 10

#define SHTC3_Sleep_Cmd           0xB098
#define SHTC3_WakeUp_Cmd          0x3517
#define Normal_RT_CLKEN_Cmd       0x7CA2
#define Normal_RH_CLKDISEN_Cmd    0x5c24

void Init_I2C(void);
void I2C_Error(void);
void I2C_Write_Process(UINT8 u8DAT);
void I2C_Read_Process(UINT8 u8DAT);
void Init_SHTC3(void);
void Get_TempHut(void);
void I2C_Read(UINT8 *pbuf,len);
uint16_t Get_TempValue(void);
uint16_t Get_HubValue(void);
void Is_IIC_OverTime(void);
uint8_t Get_TempPositive(void);
void Set_TempPositive(void);
#endif