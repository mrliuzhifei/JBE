#ifndef  RF_CONFIG_H
#define  RF_CONFIG_H

/*						RF SPI I/O Config							*/


#define   RF_INT       P13
/*------------------------------------------------------------------*/
#define		RF_SPI_line		3				// [3/4]line SPI                          //设置SPI的工作模式是3线的

/*------------------------------------------------------------------*/
/*						RF IRQ I/O Config							*/
/*------------------------------------------------------------------*/
//#define		RF_INT			_pa1
//#define		RF_INT_CTR		_pac1
//#define		RF_INT_PU		_papu1
//#define		RF_intxS0		_int0s0
//#define		RF_intxS1		_int0s1
//#define		RF_intxPS		_int0ps
//#define		RF_INT_EN		_int0e

/*------------------------------------------------------------------*/
/*							RF Config								*/
/*------------------------------------------------------------------*/
#define		RF_BAND						_433MHz_			// [315/433/470/868/915]MHz
#define		RF_TxPower					_10dBm_				// [0/5/10/13]dBm
#define		RF_Datarate					_50kbps_			// [2/10/50/125/250]kbps

#define		RF_Payload_Length			32					// [1-255]bytes
#define		RF_SYNC_Length				4					// [4/6/8]bytes
#define		RF_SYNC_BCHcoding			0					// [1:ON 0:OFF]		msut be set same in BCH32.asm
#define		RF_SYNC_L					0x12345678			// SYNC[3-0]bytes
#define		RF_SYNC_H					0x87654321			// SYNC[7-5]bytes

#define		RF_Trailer_EN				0					// [1:ON 0:OFF]
#define		RF_TxPreamble_Length		7					// [0-255]+1 bytes前导码长度
#define		RF_RxPreamble_Length		2					// [0/1/2/3]valur for [0/1/2/4]bytes
#define		RF_PLLEN_EN					0					// [1:ON 0:OFF]
#define		RF_Header_EN				0					// [1:ON 0:OFF]
#define		RF_Header_Length			0					// [1:2bytes 0:1ybte]

#define		RF_Manchester_EN			0					// [1:ON 0:OFF]
#define		RF_FEC_EN					0					// [1:ON 0:OFF]
#define		RF_CRC_EN					1					// [1:ON 0:OFF]
#define		RF_CRC_Format				0					// [1:IBC-16-CRC 0:CCITT-16-CRC]
#define		RF_whiteining_EN			0					// [1:ON 0:OFF]
#define		RF_whiteining_Seed			0x00				// [0-127]	

#define		RF_PLHAC_EN					0					// [1:ON 0:OFF]
#define		RF_PLHA_Address				0x00				// [bit5:bit0]

/*ATR MODE*/
#define		RF_ATR_Cycle				1000				// [1-8190]ms
#define		RF_ATR_RX_Window			9500				// [250-63750]us
#define		RF_ATR_RX_extra				250					// [1-16380]ms

/*ARK MODE*/
#define		RF_ARK_Enable				0					// [1:ON 0:OFF]		if ARK on,RF payload length must be set lesser than 64 .
#define		RF_ARK_Resen_Count			5					// [0-15] count
#define		RF_ARK_RX_Window			100000				// [250-63750]us


#endif
