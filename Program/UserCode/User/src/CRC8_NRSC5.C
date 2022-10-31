#include "ML51.h"
/******************************************************************************
 * Name:    CRC-8/NRSC5         x8+x5+x4+1
 * Poly:    0x31
 * Init:    0xFF
 * Refin:   
 * Refout:  
 * Xorout:  0x00
     
 *****************************************************************************/
 
uint8_t crc8_nrsc5(uint8_t *Data, uint16_t  length)
{
    uint8_t i=0;
    uint8_t crc=0xff;         // Initial value
    while(length--)
    {
        crc^= *Data++;        
	//crc ^= *Data; Data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0X80)
                crc = (crc<<1)^ 0x31;        // 
            else
                crc =crc<<1;
        }
    }
    return crc;
}










