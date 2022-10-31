#include "BC3602_SPI.h "
#include "ML51.h"
void BC3602_SPI_IOConfig(void)
{
/*先把IO口设置为输出并且初始化输出高电瓶*/
GPIO_SetMode(RFCSN_PORT ,RFCSN_PIN,GPIO_MODE_PUSHPULL);
Set_RFSDIO_Dir(Output);
GPIO_SetMode(RFSCK_PORT,RFSCK_PIN ,GPIO_MODE_PUSHPULL);
BC3602CSN=1 ;	
BC3602MOSI=1;
BC3602SCK=1 ;				
}

void Set_RFSDIO_Dir(uint8_t Director)//设置RFSDIO口的方向
{
	if(Director==Output)
	{
   GPIO_SetMode(RFSDIO_PORT,RFSDIO_PIN ,GPIO_MODE_PUSHPULL);
		
	}
	else if(Director==Input)
	{
   GPIO_SetMode(RFSDIO_PORT,RFSDIO_PIN ,GPIO_MODE_INPUT );
	 GPIO_Pull_Enable(RFSDIO_PORT,RFSDIO_PIN,PULLUP);
	}
}

void SpiWrite(unsigned char SIMD)
{
//#if (RF_SPI_line==3)
	unsigned char	cnt;
	//RF_SDIO_CTR = 0;
	Set_RFSDIO_Dir(Output);//设置为输出
	for	(cnt=0;cnt<8;cnt++)
	{
		//RF_SCK = 0;
		BC3602SCK=0;
		if( SIMD & 0x80)
			BC3602MOSI = 1; 
		else
			BC3602MOSI= 0;
		SIMD = SIMD<<1;
		//RF_SCK = 1;
		BC3602SCK=1;
	}
	//RF_SCK = 0;
	BC3602SCK=0;
	
}

//*******************************//
// name		:	SpiRead
// input	:	none
// output	:	SPIdata
// stack	:	1
//*******************************//
unsigned char SpiRead(void)
{
//#if (RF_SPI_line==3)
	unsigned char cnt,SIMD=0;
	//RF_SDIO_CTR = 1;
	Set_RFSDIO_Dir(Input);
	//RF_SCK	= 0;
	BC3602SCK=0;
	for	(cnt=0;cnt<8;cnt++)
	{
		SIMD = SIMD<<1;
		//RF_SCK	= 1;
		BC3602SCK=1;
		if(BC3602MOSI)
			SIMD |= 0x01; 
		//RF_SCK	= 0;
		BC3602SCK=0;
	}
	return SIMD;

}