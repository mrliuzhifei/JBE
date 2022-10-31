#ifndef BC3602_H
#define BC3602_H
#include "RF_CONFIG.h"

extern unsigned char RF_TXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
extern unsigned char RF_RXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
extern unsigned char RF_RXFIFOSAVE[RF_Payload_Length];		//FIFO length defined in Configuration.h
extern unsigned char RF_IRQ;
void RF_Init(void);
void SpiWriteStrobeCMD(unsigned char CMD);
void ResetLvRF(void);
void BC3602_Config(void);
void RFWriteSyncword(void);
void KEY_BC3602(unsigned char key_status);
unsigned char Get_RF_Mode(void);
void BC3602_RF_Process(void);
#endif