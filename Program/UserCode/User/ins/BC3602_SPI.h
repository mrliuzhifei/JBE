#ifndef BC3602_SPI_H
#define BC3602_SPI_H
#include "TYPE.h"
#define RFCSN_PORT   Port1  
#define RFSDIO_PORT  Port0  
#define RFSCK_PORT   Port0    


#define RFCSN_PIN    BIT0 
#define RFSDIO_PIN   BIT1  
#define RFSCK_PIN    BIT0 

#define BC3602CSN   P10
#define BC3602SCK   P00
#define BC3602MOSI  P01




#define Input  1
#define Output 0
void Set_RFSDIO_Dir(uint8_t Director);
void BC3602_SPI_IOConfig(void);
void SpiWrite(unsigned char SIMD);
unsigned char SpiRead(void);


#endif