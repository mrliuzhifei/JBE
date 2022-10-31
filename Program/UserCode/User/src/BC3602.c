#include "BC3602.h"
#include "BC3602_CMD_REG.h"
#include "BC3602_SPI.h"
#include "ML51.h"
#include "BC3602_CONFIG_TABLE.h"


unsigned char RF_TXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_RXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_RXFIFOSAVE[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_IRQ;



unsigned char mRFSync[RF_SYNC_Length];			//SYNC length defined in Configuration.h
struct define_flag
{
	unsigned char Key_Stas;																	// KEY states
	unsigned char LED_Stas;																	// LED states
	unsigned char RFmode;																	// RF_MODE
	unsigned char RFstep;																	// RF process step
	unsigned char fRFINT:1;																	// RF interrupt flag
	unsigned char f8ms:1;																	// MCU timebase time-out 8ms flag
};
struct define_flag Mflag;


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	SpiWriteRegCMD
// input	:	CMD,DATA
// output	:	none
//*******************************//
void SpiWriteRegCMD(unsigned char CMD,unsigned char DATA)
{
	//SpiCSN_low();
	BC3602CSN=0 ;
	SpiWrite(CMD);
	SpiWrite(DATA);
	//SpiCSN_high();
	BC3602CSN=1 ;
}

//*******************************//
// name		:	SpiWriteByte
// input	:	CMD
// output	:	ReadDATA
//*******************************//
unsigned char SpiReadRegCMD(unsigned char CMD)
{
	unsigned char ReadDATA;
	//SpiCSN_low();
	BC3602CSN=0 ;
	SpiWrite(CMD);
	ReadDATA = SpiRead();
	//SpiCSN_high();
	BC3602CSN=1 ;
	return	ReadDATA;
}

//*******************************//
// name		:	SpiWriteStrobeCMD
// input	:	CMD
// output	:	none
//*******************************//
void SpiWriteStrobeCMD(unsigned char CMD)
{
	//SpiCSN_low();
	BC3602CSN=0 ;
	SpiWrite(CMD);
	//SpiCSN_high();
	BC3602CSN=1 ;
}



//*******************************//
// name		:	RFWriteFreq
// input	:	none
// output	:	none
//*******************************//
void RFWriteFreq(void)
{
	unsigned char a=0;
	a = SpiReadRegCMD(READ_REGS_CMD|OM_REGS);
	a = (a&0x9f)|BAND_SEL;
	SpiWriteRegCMD(WRITE_REGS_CMD|OM_REGS,a);
	
	for(a=0;a<(sizeof Frequency_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(Frequency_REGS_TALBE[a]>>8)), Frequency_REGS_TALBE[a]);
}

//*******************************//
// name		:	RFSetDRPram
// input	:	none
// output	:	none
//*******************************//
void RFSetDRPram(void)
{
	unsigned char a=0;
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	for(a=0;a<(sizeof DM_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(DM_REGS_TALBE[a]>>8)), DM_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	for(a=0;a<(sizeof FCF_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(FCF_REGS_TALBE[a]>>8)), FCF_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK2);
	for(a=0;a<(sizeof CBPF_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(CBPF_REGS_TALBE[a]>>8)), CBPF_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
}

void RFSetPower(unsigned char band_sel,unsigned char power)
{
	SpiWriteRegCMD(WRITE_REGS_CMD|TX2_REGS,TxPowerValue[band_sel][power]);
}

//*******************************//
// name		:	RFXtalReady
// input	:	none
// output	:	none
//*******************************//
void RFXtalReady(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|RC1_REGS);
		a &= 0x20;
	}
}

//*******************************//
// name		:	RFCalibration
// input	:	none
// output	:	none
//*******************************//
void RFCalibration(void)
{
	unsigned char a=0;
	a=SpiReadRegCMD(READ_REGS_CMD|OM_REGS);
	a |= 0x08;
	SpiWriteRegCMD(WRITE_REGS_CMD|OM_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|OM_REGS);
		a &= 0x08;
	}
}


//*******************************//
// name		:	LircCalibration
// input	:	none
// output	:	none
//*******************************//
void LircCalibration(void)
{
	unsigned char a=0;
	a=SpiReadRegCMD(READ_REGS_CMD|XO3_REGS);
	a |= 0x81;
	SpiWriteRegCMD(WRITE_REGS_CMD|XO3_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|XO3_REGS);
		a &= 0x80;
	}
}

//*******************************//
// name		:	RFGetClrRFIrq
// input	:	none
// output	:	irq_status
//*******************************//
unsigned char RFGetClrRFIrq(void)
{
	unsigned char irq;
	
	irq=SpiReadRegCMD(READ_REGS_CMD|IRQ3_REGS);
	SpiWriteRegCMD(WRITE_REGS_CMD|IRQ3_REGS,irq);
	
	return irq;
}

void RF_Init(void)
{
	//SpiIOInti();												     // RF SPI I/O Initialization
	 BC3602_SPI_IOConfig();                    // SPI端口初始化	
	 SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);		   // 进入轻睡眠模式	
	 //RFXtalReady();												   // Whaiting XTAL ready
	 ResetLvRF();												       // Reset LV
	 SpiWriteStrobeCMD(SOFT_RESET_CMD);			   // RF software reset软复位	
	//RFXtalReady();												   // Whaiting XTAL ready
	//_emi = 0;													       // Disable EMI
	 DISABLE_GLOBAL_INTERRUPT;                 // 关闭全局中断
	 	/**********开启下降沿中断**************************/
	  MFP_P13_GPIO;
    GPIO_SetMode(Port1,BIT3,GPIO_MODE_INPUT);                              /* set P2.5 as input output mode */
    GPIO_Pull_Enable(Port1,BIT3,PULLUP );
    GPIO_EnableInt(PIT0,FALLING,EDGE,1,3);         /* Setting pin intterrupt channel 0 as P2.5 falling edge trigger. */
	  /*************************************/ 
	 BC3602_Config();											     // RF register configuration
	 RFWriteSyncword();											   // Set RF SYNC
	//RFReadSyncword();
	 RFWriteFreq();												     // Set RF working frequency
	 RFSetDRPram();												     // Set RF datarate
	 RFSetPower(RF_BAND,RF_TxPower);			     // Set RF output power
	//_emi = 1;													       // Enable EMI
	 ENABLE_GLOBAL_INTERRUPT;                  // 开全局中断
	 RFXtalReady();												     // Whaiting XTAL ready
	 RFCalibration();											     // RF Calibration
	 LircCalibration();										 	   // LIRC Calibration
	 RFGetClrRFIrq();											     // Clear RF IRQ status
	 SpiWriteStrobeCMD(DEEP_SLEEP_CMD);				 // Entry Deepsleep mode*/
	 Mflag.RFmode = RF_RX;
}






void ResetLvRF(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|RC1_REGS);
		a |= 0x01;
		SpiWriteRegCMD(WRITE_REGS_CMD|RC1_REGS,a);
		a=SpiReadRegCMD(READ_REGS_CMD|RC1_REGS);
		a &= 0xFE;
		SpiWriteRegCMD(WRITE_REGS_CMD|RC1_REGS,a);
	}
}

//*******************************//
// name		:	BC3602_Config
// input	:	none
// output	:	none
//*******************************//
//：应为数组是16位，这样sizeof 以后获得的数其实是数组个数的两倍，所以这里要除以二，才能的到数组的格式
void BC3602_Config(void)
{
	unsigned char a=0;
	//	BC3601 IRQ/IO Configure	//
	for(a=0;a<(sizeof IRQIO_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(IRQIO_REGS_TALBE[a]>>8)), IRQIO_REGS_TALBE[a]);
	//	BC3602 packet format Configure	//
	for(a=0;a<(sizeof PACKET_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(PACKET_REGS_TALBE[a]>>8)), PACKET_REGS_TALBE[a]);
	//	BC3602 common Configure	//
	for(a=0;a<(sizeof COMMON_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(COMMON_REGS_TALBE[a]>>8)), COMMON_REGS_TALBE[a]);
	
	//	BC3602 Bank0 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	for(a=0;a<(sizeof BANK0_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(BANK0_REGS_TALBE[a]>>8)), BANK0_REGS_TALBE[a]);
	//	BC3602 Bank1 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	for(a=0;a<(sizeof BANK1_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(BANK1_REGS_TALBE[a]>>8)), BANK1_REGS_TALBE[a]);
	//	BC3602 Bank2 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK2);
	for(a=0;a<(sizeof BANK2_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(BANK2_REGS_TALBE[a]>>8)), BANK2_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
}


//*******************************//
// name		:	RFWriteBuf
// input	:	CMD,length,data[]
// output	:	none
//*******************************//
void RFWriteBuf(unsigned char CMD,unsigned char length,unsigned char Data[])
{
	unsigned char a;
	//SpiCSN_low();
	BC3602CSN=0 ;	
	SpiWrite(CMD);
	for(a=0;a<length;a++) SpiWrite(Data[a]);
	//SpiCSN_high();
	BC3602CSN=1 ;	
}


//*******************************//
// name		:	RFWriteSyncword
// input	:	none
// output	:	none
//*******************************//
void RFWriteSyncword(void)
{
	unsigned char a;
	
#if RF_SYNC_BCHcoding
	
	#if (RF_SYNC_Length != 4)
	unsigned char temp1[RF_SYNC_Length];
	#endif
	
	#if (RF_SYNC_Length == 4)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
	#if (RF_SYNC_Length == 6)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a]=mRFSync[a];
	for(a=0;a<2;a++) mLap[a]=BC3602_SYNCWORD[a+4];
	mLap[2]=0;
	mLap[3]=0;
	BCH32_Syncword();
	for(a=0;a<2;a++) temp1[a+4]=mRFSync[a];
	for(a=0;a<6;a++) mRFSync[a]=temp1[a];
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
	#if (RF_SYNC_Length == 8)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a]=mRFSync[a];
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a+4];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a+4]=mRFSync[a];
	for(a=0;a<8;a++) mRFSync[a]=temp1[a];
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
#else
	
	for(a=0;a<RF_SYNC_Length;a++)
		mRFSync[a] = BC3602_SYNCWORD[a];
	RFWriteBuf(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);
	
#endif
	
}

//*********************************************//
// name		:	DATA_Process
// input	:	RF_TXFIFO
// output	:	none
//*********************************************//
void DATA_Process(unsigned char *FIFO)
{
	FIFO++;
	*FIFO = 0x22;
	//
	//
	//
	FIFO++;
	*FIFO = 0x33;
}

//*********************************************//
// name		:	KEY_Process
// input	:	key_status
// output	:	none
//*********************************************//
void KEY_BC3602(unsigned char key_status)
{
	switch(key_status)
	{
		case 1:
			RF_TXFIFO[0] = 0x01;															// put data into RF_TXFIFO
			DATA_Process(RF_TXFIFO);														// put data into RF_TXFIFO
			Mflag.RFstep = 0;																// reset RF step
			Mflag.RFmode = RF_TX;															// set RF mode flag as TX															
			break;
			
		case 2:
			RF_TXFIFO[0] = 0x02;
			DATA_Process(RF_TXFIFO);
			Mflag.RFstep = 0;
			Mflag.RFmode = RF_TX;
			break;
			
		case 3:
			RF_TXFIFO[0] = 0x03;
			DATA_Process(RF_TXFIFO);
			Mflag.RFstep = 0;
			Mflag.RFmode = RF_TX;
			break;
			
		case 4:
			RF_TXFIFO[0] = 0x04;
			DATA_Process(RF_TXFIFO);
			Mflag.RFstep = 0;
			Mflag.RFmode = RF_TX;
			break;	
			
		default:
			break;
	}
}

void PinInterrupt_ISR (void) interrupt 7
{
    _push_(SFRS);
  
    SFRS = 0;
    switch(PIF)
    {
      case 0x01:
			Mflag.fRFINT = 1; 
			break;
      case 0x02:// printf("\n PIT1 interrupt!"); break;
      default: break;
    }                 
    PIF = 0; 
  
    _pop_(SFRS);
}

unsigned char Get_RF_Mode(void) 
{
return Mflag.RFmode;
}



//*******************************//
// name		:	WriteFIFO
// input	:	none
// output	:	none
//*******************************//
void WriteFIFO(unsigned char *rf_fifo,unsigned char payload_length)
{
	unsigned char a=0;	
	BC3602CSN=0;
	SpiWrite(WRITE_FIFO_CMD);
	for(a=0;a<payload_length;a++)
	{
		SpiWrite(*rf_fifo);
		rf_fifo++;
	}
	BC3602CSN=1;
}

//*******************************//
// name		:	ReadFIFO
// input	:	none
// output	:	none
//*******************************//
void ReadFIFO(unsigned char *rf_fifo,unsigned char length)
{
	unsigned char a=0;	
	BC3602CSN=0;
	SpiWrite(READ_FIFO_CMD);
	for(a=0;a<length;a++)
	{
		*rf_fifo = SpiRead();
		rf_fifo++;
	}
	BC3602CSN=1;
}


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	SimpleFIFO_TX_Process
// input	:	step
// output	:	RF_Mode
//*******************************//
void SimpleFIFO_TX_Process(unsigned char *step,unsigned char *IRQ,unsigned char *Mode)
{
	switch(*step)
	{
		case 0:
			SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);													// RF entry LightSleep mode
			RFXtalReady();																		// waitting XTAL ready
			
			SpiWriteRegCMD(WRITE_REGS_CMD|IRQ2_REGS,0x01);										// enable TXCMPIF
			
			SpiWriteStrobeCMD(REST_TX_POS_CMD);													// reset RF TX FIFO
			SpiWriteRegCMD(WRITE_REGS_CMD|PKT5_REGS,RF_Payload_Length);							// set RF TX payload length
			WriteFIFO(RF_TXFIFO,RF_Payload_Length);												// write data to RF FIFO
			
			SpiWriteStrobeCMD(TX_MODE_CMD);														// RF entry TX mode
			(*Mode) = RF_TX;																	// set RF mode flag as TX
			(*step)++;																			// increase RF step
			break;
			
		case 1:
			if(RF_INT==0)																		// while RF IQR line being low
				(*IRQ) = RFGetClrRFIrq();														// read RF IRQ status
			
			if(RF_IRQ&0x01)																		// while RF IQR equal TXCMPIF
			{
				SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);												// RF entry LightSleep mode
				(*Mode) = RF_LightSleep;														// set RF mode flag as Lightsleep
				(*step) = 0;																	// reset RF step
			}
			break;
			
		default:
			(*step) = 0;
			break;
	}
}


//*******************************//
// name		:	SimpleFIFO_RX_Process
// input	:	step
// output	:	RF_Mode
//*******************************//
void SimpleFIFO_RX_Process(unsigned char *step,unsigned char *IRQ,unsigned char *Mode)
{
	unsigned char a;
	
	switch(*step)
	{
		case 0:
			SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);													// RF entry LightSleep mode
			RFXtalReady();																		// waitting XTAL ready
			
			SpiWriteRegCMD(WRITE_REGS_CMD|IRQ2_REGS,0x12);										// enable RXERRIF & RXCMPIF
			
			for(a=0;a<RF_Payload_Length;a++)													// clear RF_RXFIFO
				RF_RXFIFO[a] = 0;	
			
			SpiWriteStrobeCMD(REST_RX_POS_CMD);													// reset RF RX FIFO
			SpiWriteRegCMD(WRITE_REGS_CMD|PKT6_REGS,RF_Payload_Length);							// set RF RX payload length
			
			SpiWriteStrobeCMD(RX_MODE_CMD);														// RF entry RX mode
			(*Mode) = RF_RX;																	// set RF mode flag as RX
			(*step)++;																			// increase RF step
			break;
			
		case 1:
			if(RF_INT==0)																		// while RF IQR line being low
				(*IRQ) = RFGetClrRFIrq();														// read RF IRQ status
			
			if(RF_IRQ&0x12)																		// while RF IQR equal RXERRIF or RXCMPIF
			{
				if(RF_IRQ & 0x02)																// while RF IQR equal RXCMPIF
					ReadFIFO(RF_RXFIFO,RF_Payload_Length);										// read RX FIFO
					memcpy(RF_RXFIFOSAVE,RF_RXFIFO,RF_Payload_Length);
				SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);												// RF entry LightSleep mode
				(*Mode) = RF_LightSleep;														// set RF mode flag as Lightsleep
				(*step) = 0;																	// reset RF step
			}
			break;
			
		default:
			(*step) = 0;
			break;
	}
}
 



//*********************************************//
// name		:	RF_Finished_Process
// input	:	key_status
// output	:	none
//*********************************************//
void RF_Finished_Process(void)
{
	if(RF_IRQ & 0x01)																		// while TX finished
	{
		RF_IRQ = 0;																			// clear IRQ flag
		Mflag.RFmode = RF_RX;																// set RF mode flag as RX
		
		switch(RF_TXFIFO[0])																// display LED show TX success
		{
			case 0x01:
//				LEDx_Flash(LED1,1,10);
			  LED_FLASH(1);
				break;
			case 0x02:
//				LEDx_Flash(LED2,1,10);
			  LED_FLASH(2);
				break;
			case 0x03:
//				LEDx_Flash(LED3,1,10);
			  LED_FLASH(3);
				break;
			case 0x04:
//				LEDx_Flash(LED4,1,10);
			  LED_FLASH(4);
				break;
			default:
				break;
		}
		
	}
	
	if(RF_IRQ & 0x12)																		// while RX finished
	{
		RF_IRQ = 0;																			// clear IRQ flag
		Mflag.RFmode = RF_RX;																// set RF mode flag as RX
		if(RF_IRQ & 0x10)																	// while RX finished but CRC error
		{
			
			
			
		}
		else																				// while RX finished and CRC OK
		{
			switch(RF_RXFIFO[0])															// check RF DATA
			{
				case 0x01:
					//LEDx_Flash(LED1,1,10);
				  LED_FLASH(1);
					break;
				case 0x02:
					//LEDx_Flash(LED2,1,10);
				  LED_FLASH(2);
					break;
				case 0x03:
				//	LEDx_Flash(LED3,1,10);
				  LED_FLASH(3);
					break;
				case 0x04:
					//LEDx_Flash(LED4,1,10);
				  LED_FLASH(4);
					break;
				default:
					break;
			}
		}
	}
}


void BC3602_RF_Process(void)
{
	
	if(Mflag.RFmode == RF_TX)															                  // while RF mode flag = TX
		SimpleFIFO_TX_Process(&Mflag.RFstep,&RF_IRQ,&Mflag.RFmode);						// RF Simple FIFO TX porcess	
	
	if(Mflag.RFmode == RF_RX)															                  // while RF mode flag = RX
		SimpleFIFO_RX_Process(&Mflag.RFstep,&RF_IRQ,&Mflag.RFmode);						// RF Simple FIFO RX porcess			
  	RF_Finished_Process();
	 // RF_Finished_Process(RF_IRQ)
}



