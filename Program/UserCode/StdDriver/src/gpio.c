/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//***********************************************************************************************************

#include "ML51.h"

//****************************************************************************************************************  
//**** GPIO interrupt enable setting step
//**** 1. Select pin interrupt channel PIT0~PIT7
//**** 2. Select trig status HIGH,LEVEL / LOW,LEVEL / RISING,EDGE / FALLING,EDGE / BOTH,EDGE /
//**** 3. Define interrupt port (PORT, BIT)
//**** For example: GPIO_EnableInt(PIT0,BOTH,EDGE,Port1,0) means P1.0 falling edge trig pin intterrupt channel 0 (PIT0)
//GPIO_EnableInt(PIT0,FALLING,EDGE,1,3);  
void GPIO_EnableInt(unsigned char u8PIT, unsigned char u8IntStatus,unsigned char u8IntMode, unsigned char u8Port, unsigned char u8Bit)
{
    switch ((u8IntMode<<4)+u8PIT)
    {
      case 0x00:    clr_PICON_PIT0;  break;
      case 0x01:    clr_PICON_PIT1;  break;
      case 0x02:    clr_PICON_PIT2;  break;
      case 0x03:    clr_PICON_PIT3;  break;
      case 0x04:    clr_PICON_PIT4;  break;
      case 0x05:    clr_PICON_PIT5;  break;
      case 0x06:    clr_PICON_PIT6;  break;
      case 0x07:    clr_PICON_PIT7;  break;
      
      case 0x10:    set_PICON_PIT0;  break;
      case 0x11:    set_PICON_PIT1;  break;
      case 0x12:    set_PICON_PIT2;  break;
      case 0x13:    set_PICON_PIT3;  break;
      case 0x14:    set_PICON_PIT4;  break;
      case 0x15:    set_PICON_PIT5;  break;
      case 0x16:    set_PICON_PIT6;  break;
      case 0x17:    set_PICON_PIT7;  break;
      default: break;
    }
    switch((u8IntStatus<<4)+u8PIT)
    {
      case 0x00: set_PINEN_PINEN0;clr_PIPEN_PIPEN0; break;
      case 0x01: set_PINEN_PINEN1;clr_PIPEN_PIPEN1; break;
      case 0x02: set_PINEN_PINEN2;clr_PIPEN_PIPEN2; break;
      case 0x03: set_PINEN_PINEN3;clr_PIPEN_PIPEN3; break;
      case 0x04: set_PINEN_PINEN4;clr_PIPEN_PIPEN4; break;
      case 0x05: set_PINEN_PINEN5;clr_PIPEN_PIPEN5; break;
      case 0x06: set_PINEN_PINEN6;clr_PIPEN_PIPEN6; break;
      case 0x07: set_PINEN_PINEN7;clr_PIPEN_PIPEN7; break; 
      
      case 0x10: clr_PINEN_PINEN0;set_PIPEN_PIPEN0; break;
      case 0x11: clr_PINEN_PINEN1;set_PIPEN_PIPEN1; break;
      case 0x12: clr_PINEN_PINEN2;set_PIPEN_PIPEN2; break;
      case 0x13: clr_PINEN_PINEN3;set_PIPEN_PIPEN3; break;
      case 0x14: clr_PINEN_PINEN4;set_PIPEN_PIPEN4; break;
      case 0x15: clr_PINEN_PINEN5;set_PIPEN_PIPEN5; break;
      case 0x16: clr_PINEN_PINEN6;set_PIPEN_PIPEN6; break;
      case 0x17: clr_PINEN_PINEN7;set_PIPEN_PIPEN7; break;  
      
      case 0x20: set_PINEN_PINEN0;set_PIPEN_PIPEN0; break; 
      case 0x21: set_PINEN_PINEN1;set_PIPEN_PIPEN1; break; 
      case 0x22: set_PINEN_PINEN2;set_PIPEN_PIPEN2; break; 
      case 0x23: set_PINEN_PINEN3;set_PIPEN_PIPEN3; break; 
      case 0x24: set_PINEN_PINEN4;set_PIPEN_PIPEN4; break; 
      case 0x25: set_PINEN_PINEN5;set_PIPEN_PIPEN5; break; 
      case 0x26: set_PINEN_PINEN6;set_PIPEN_PIPEN6; break; 
      case 0x27: set_PINEN_PINEN7;set_PIPEN_PIPEN7; break; 
 
      default: break;
    }
    switch(u8PIT)
    {
        case PIT0:PIPS0=0x00;PIPS0=(u8Port<<4)+(u8Bit&0x0F);break;      
        case PIT1:PIPS1=0x00;PIPS1=(u8Port<<4)+(u8Bit&0x0F);break;                    
        case PIT2:PIPS2=0x00;PIPS2=(u8Port<<4)+(u8Bit&0x0F);break;  
        case PIT3:PIPS3=0x00;PIPS3=(u8Port<<4)+(u8Bit&0x0F);break;   
        case PIT4:PIPS4=0x00;PIPS4=(u8Port<<4)+(u8Bit&0x0F);break;   
        case PIT5:PIPS5=0x00;PIPS5=(u8Port<<4)+(u8Bit&0x0F);break;
        case PIT6:PIPS6=0x00;PIPS6=(u8Port<<4)+(u8Bit&0x0F);break;
        case PIT7:PIPS7=0x00;PIPS7=(u8Port<<4)+(u8Bit&0x0F);break;   
        default: break;
    }
    set_EIE0_EPI;        //Pin interrupt enable bit
}

 /**
  * @brief GPIO Pull up or Pull down enable setting
  * @param[in] u8Port Decides the GPIO port number Port0 ~ Port5
  * @param[in] u8PinMask Decides  bit of the port (SET_BIT0~SET_BIT7 use "|" to define multi bit).
  * @param[in] u8PullMode Decides the GPIO Pull up or pull down (PullUp/PullDown)
  * @return  None
  * @note none
  * @exmaple :   GPIO_Pull_Enable(P1,BIT0|BIT5|BIT7,PullUp);
  */
void GPIO_Pull_Enable(unsigned char u8Port, unsigned char u8PinMask, unsigned char u8PullMode)
{
  SFRS=1;
  switch (u8PullMode)
  {
    case PULLUP:
        switch(u8Port)
        {
          case Port0:  P0UP |= u8PinMask;  break;
          case Port1:  P1UP |= u8PinMask;  break;
          case Port2:  P2UP |= u8PinMask;  break;
          case Port3:  P3UP |= u8PinMask;  break;
          case Port4:  P4UP |= u8PinMask;  break;
          case Port5:  P5UP |= u8PinMask;  break;
          case Port6:  P6UP |= u8PinMask;  break;
        }
    break;
    case PULLDOWN:
        switch(u8Port)
        {
          case Port0:  P0DW |= u8PinMask;  break;
          case Port1:  P1DW |= u8PinMask;  break;
          case Port2:  P2DW |= u8PinMask;  break;
          case Port3:  P3DW |= u8PinMask;  break;
          case Port4:  P4DW |= u8PinMask;  break;
          case Port5:  P5DW |= u8PinMask;  break;
          case Port6:  P6DW |= u8PinMask;  break;
        }
    break;
    }
}


 /**
  * @brief GPIO Pull up or Pull down disable 
  * @param[in] u8Port Decides the GPIO port number Port0 ~ Port5
  * @param[in] u8PinMask Decides  bit of the port (SET_BIT0~SET_BIT7 use "|" to define multi bit).
  * @param[in] u8PullMode Decides the GPIO Pull up or pull down (PullUp/PullDown)
  * @return  None
  * @note none
  * @exmaple :   GPIO_Pull_Enable(P1,BIT0|BIT5,PullUp);
  */
void GPIO_Pull_Disable(unsigned char u8Port, unsigned char u8PinMask, unsigned char u8PullMode)
{
  SFRS=1;
  switch (u8PullMode)
  {
    case PULLUP:
        switch(u8Port)
        {
          case Port0:  P0UP &= ~u8PinMask;  break;
          case Port1:  P1UP &= ~u8PinMask;  break;
          case Port2:  P2UP &= ~u8PinMask;  break;
          case Port3:  P3UP &= ~u8PinMask;  break;
          case Port4:  P4UP &= ~u8PinMask;  break;
          case Port5:  P5UP &= ~u8PinMask;  break;
          case Port6:  P6UP &= ~u8PinMask;  break;
        }
    break;
    case PULLDOWN:
        switch(u8Port)
        {
          case Port0:  P0DW &= ~u8PinMask;  break;
          case Port1:  P1DW &= ~u8PinMask;  break;
          case Port2:  P2DW &= ~u8PinMask;  break;
          case Port3:  P3DW &= ~u8PinMask;  break;
          case Port4:  P4DW &= ~u8PinMask;  break;
          case Port5:  P5DW &= ~u8PinMask;  break;
          case Port6:  P6DW &= ~u8PinMask;  break;
        }
    break;
    }
}


 /**
  * @brief GPIO mode setting
  * @param[in] u8Port Decides the GPIO port number Port0 ~ Port5
  * @param[in] u8PinMask Decides  bit of the port ( from SET_BIT0~SET_BIT7 use "|" to define multi bit)
  * @param[in] u8Mode Decides the GPIO mode GPIO_MODE_INPUT / GPIO_MODE_PUSHPULL / GPIO_MODE_QUASI / GPIO_MODE_OPENDRAI for mode to select.
  * @return  None
  * @note Confirm multi function pin is defined as GPIO first. call function_define_ML51.h to define.
  * @exmaple :   GPIO_SetMode(Port1,BIT0|BIT7,GPIO_MODE_QUASI);
  */
void GPIO_SetMode(unsigned char u8Port, unsigned char u8PinMask, unsigned char u8Mode)
{
    unsigned char u8PnM1, u8PnM2;

    SFRS = 1;
    switch(u8Port)
    {
        case Port0:  u8PnM1 = P0M1;  u8PnM2 = P0M2;  break;
        case Port1:  u8PnM1 = P1M1;  u8PnM2 = P1M2;  break;
        case Port2:  u8PnM1 = P2M1;  u8PnM2 = P2M2;  break;
        case Port3:  u8PnM1 = P3M1;  u8PnM2 = P3M2;  break;
        case Port4:  u8PnM1 = P4M1;  u8PnM2 = P4M2;  break;
        case Port5:  u8PnM1 = P5M1;  u8PnM2 = P5M2;  break;
        case Port6:  u8PnM1 = P6M1;  u8PnM2 = P6M2;  break;
    }
    switch(u8Mode)
    {
        case GPIO_MODE_QUASI:
            u8PnM1 &= ~u8PinMask;
            u8PnM2 &= ~u8PinMask;
            break;
        case GPIO_MODE_PUSHPULL:
            u8PnM1 &= ~u8PinMask;
            u8PnM2 |= u8PinMask;
            break;
        case GPIO_MODE_INPUT:
            u8PnM1 |= u8PinMask; 
            u8PnM2 &= ~u8PinMask;
            break;
        case GPIO_MODE_OPENDRAIN:
            u8PnM1 |= u8PinMask;
            u8PnM2 |= u8PinMask;
            break;
    }
    switch(u8Port)
    {
        case Port0:  P0M1 = u8PnM1;  P0M2 = u8PnM2;  break;
        case Port1:  P1M1 = u8PnM1;  P1M2 = u8PnM2;  break;
        case Port2:  P2M1 = u8PnM1;  P2M2 = u8PnM2;  break;
        case Port3:  P3M1 = u8PnM1;  P3M2 = u8PnM2;  break;
        case Port4:  P4M1 = u8PnM1;  P4M2 = u8PnM2;  break;
        case Port5:  P5M1 = u8PnM1;  P5M2 = u8PnM2;  break;
        case Port6:  P6M1 = u8PnM1;  P6M2 = u8PnM2;  break;
    }
	 SFRS = 0;	
		
}
