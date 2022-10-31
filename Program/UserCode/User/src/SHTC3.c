#include "ML51.h"
uint8_t Minit_Times;
uint8_t Minit_Times_Hub=0;
typedef enum SHTC3_Sta_s	//???????
{
	 Sleep,
	 Wakeup,
	 Mesured,
	 Readstart,
	 Readover,
}SHTC3_Sta_s;

typedef struct SHTC3_Data_t
{
 uint8_t CmdBuf [2];
 uint8_t ReadBuf[6];
 SHTC3_Sta_s 	SHTC3_Sta;
 uint16_t Temp_value;
 uint16_t Temp_value_mid;	
 uint16_t Temp_value_Intinal;	
 uint8_t FirstTimeGetTempFlag;	
 uint16_t Hub_value;
 uint16_t Hub_value_mid;	
 uint16_t Hub_value_Intinal;
 uint8_t crc8;	
}SHTC3_Data_t;

SHTC3_Data_t SHTC3_Data;
#define JBE SHTC3_Data

void Init_SHTC3(void)
{
JBE.SHTC3_Sta=Sleep;
}
//========================================================================================================


//========================================================================================================
void I2C_Error(void)
{
   // while (1);    
	//Get_TempHut();
}
//========================================================================================================

//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
/*主机发送模式*/
void I2C_Write(UINT8 *pbuf,UINT8 len )
{
     unsigned char  u8Count;
	  
    /* Write Step1 */
	  set_I2C0CON_STA;                                      /*STA置1位主机发送模式，硬件发出起始信号，如果起始信号发送成功，SI 置1*/                                /* Send Start bit to I2C EEPROM */
    clr_I2C0CON_SI;
	  while (!(I2C0CON&SET_BIT3));                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
	 
	  if (I2C0STAT != 0x08)                                 /*状态码为0x08的时候说明起始信号发送成功*/                /*Check status value after every step   */
        I2C_Error();
    /*发送从机地址和写标志*/
    /* Write Step2 */
    clr_I2C0CON_STA;                                      /*STA=0*/
    I2C0DAT = (I2C_SLAVE_ADDRESS | I2C_WR);
    clr_I2C0CON_SI;
    while (!(I2C0CON&SET_BIT3));                          /*Check SI set or not */
    if (I2C0STAT != 0x18)                                 /*从机地址已发，收到了应答，状态为0x18*/
        I2C_Error();

    /* Write Step3 */
    for (u8Count = 0; u8Count < len; u8Count++)     /*循环发送自定义数据*/
    {
			 
        I2C0DAT = *pbuf;
			  pbuf++;
        clr_I2C0CON_SI;
        while (!(I2C0CON&SET_BIT3));                       /*Check SI set or not*/
        if (I2C0STAT != 0x28)
            I2C_Error();
    }

    /* Write Step4 */
    set_I2C0CON_STO;    /*停止标志位，主机模式下设置为1时向总线发送停止信号*/
    clr_I2C0CON_SI;
    while (I2C0CON&SET_BIT4); 

}

void I2C_Read(UINT8 *pbuf,len)
{
    unsigned char  u8Count;
    /* Read Step1 */
    set_I2C0CON_STA;
    clr_I2C0CON_SI;          
    while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
	if (I2C0STAT != 0x08)                         /*起始位发送成功*///Check status value after every step
        I2C_Error();

    /* Step13 */
    clr_I2C0CON_STA;                                    //STA needs to be cleared after START codition is generated
	  I2C0DAT = (I2C_SLAVE_ADDRESS | I2C_RD);       /*发送从机地址，和读标志位*/
    clr_I2C0CON_SI;
    while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
	if (I2C0STAT != 0x40)                         /*发送成功收到从机应答*/
        I2C_Error();
    
    /* Step14 */
    for (u8Count = 0; u8Count <len; u8Count++)
    {
        set_I2C0CON_AA;
        clr_I2C0CON_SI;        
        while (!(I2C0CON&SET_BIT3));                            //Check SI set or not

        if (I2C0STAT != 0x50)              
            I2C_Error();
        
        // if (I2C0DAT != u8DAT)             
        //    I2C_Error();
        *pbuf=I2C0DAT;
				 pbuf++;
    } 
    
    /* Step15 */
    clr_I2C0CON_AA;
    clr_I2C0CON_SI;
    while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
    if (I2C0STAT != 0x58)
        I2C_Error();

    /* Step16 */
    set_I2C0CON_STO;
    clr_I2C0CON_SI;
    while (I2C0CON&SET_BIT4);                                /* Check STOP signal */ 
}

void Init_I2C(void)
{
    MFP_P53_I2C0_SCL;
    P53_OPENDRAIN_MODE ;          // Modify SCL pin to Open drain mode. don't forget the pull high resister in circuit
    MFP_P52_I2C0_SDA;
    P52_OPENDRAIN_MODE ;          // Modify SDA pin to Open drain mode. don't forget the pull high resister in circuit

    SFRS = 0;
    /* Set I2C clock rate */
    I2C0CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2C0CON_I2CEN;  
    JBE.SHTC3_Sta=Sleep;
		JBE.CmdBuf [0]=0x35;     //睡眠模式下发送唤醒命令0x3517                                                                                                                                                                   
		JBE.CmdBuf [1]=0x17;
		I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));
    JBE.CmdBuf [0]=0x80;                                                                                                                                                                     
	  JBE.CmdBuf [1]=0x5D;
	  I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));	
}


void Get_TempHut(void)
{
 #ifdef test
	
	JBE.CmdBuf [0]=0x35;     //睡眠模式下发送唤醒命令0x3517                                                                                                                                                                   
	JBE.CmdBuf [1]=0x17;
	I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));
	
	JBE.CmdBuf [0]=0xEF;     //发送读取ID的命令0xEFC8   测试i2c的通信                                                                                                                                                           
	JBE.CmdBuf [1]=0xC8;
	I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));
	I2C_Read(&JBE.ReadBuf,3);
//	printf("\n READ ID!");
	
 #else	
 if(JBE.SHTC3_Sta==Sleep)
 {
	JBE.CmdBuf [0]=0x35;     //睡眠模式下发送唤醒命令0x3517                                                                                                                                                                   
	JBE.CmdBuf [1]=0x17;
	I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));
	JBE.SHTC3_Sta=Wakeup;
 }
 else if(JBE.SHTC3_Sta== Wakeup)
 {
	JBE.CmdBuf [0]=0x7c;     //唤醒以后发送0x7CA2选择普通模式，高精度读取
	JBE.CmdBuf [1]=0xa2;
	I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));
	JBE.SHTC3_Sta=Mesured;
 }
 else if(JBE.SHTC3_Sta== Mesured)
 {
	 static unsigned char  DelayTime;
	 DelayTime++;
	 if(DelayTime>MeasurDlayTime)
	 {
	 DelayTime=0;
	 JBE.SHTC3_Sta=Readstart;
	 }
 }
 else if(JBE.SHTC3_Sta== Readstart)
 {
	 uint8_t Temp_const=0;
   I2C_Read(&JBE.ReadBuf,sizeof(JBE.ReadBuf));
	 JBE.SHTC3_Sta=Readover; 
	 JBE.crc8=crc8_nrsc5(&JBE.ReadBuf[0],2);
	 JBE.crc8=crc8_nrsc5(&JBE.ReadBuf[3],2);
	 if(crc8_nrsc5(&JBE.ReadBuf[0],2)==JBE.ReadBuf[2]&&crc8_nrsc5(&JBE.ReadBuf[3],2)==JBE.ReadBuf[5])
	 {
	 JBE.Temp_value_mid=JBE.ReadBuf[0];
	 JBE.Temp_value_mid=JBE.Temp_value_mid<<8;
	 JBE.Temp_value_mid=JBE.Temp_value_mid+JBE.ReadBuf[1];	 
	 JBE.Temp_value_mid=(float)JBE.Temp_value_mid/65536*175*10;
	 if(JBE.Temp_value_mid>=450)                                  //说明获得的是正温度
	 {
	 JBE.Temp_value_mid=JBE.Temp_value_mid-450;    
	 }
	 else 
	 {
	 JBE.Temp_value_mid=450-JBE.Temp_value_mid;
	 }
	 if(Minit_Times<10)
	 {
	 if(OutTempIsHigh==1)JBE.Temp_value=JBE.Temp_value_mid-Temp_Delt*Minit_Times/10;
	 else JBE.Temp_value=JBE.Temp_value_mid-15;
	 }
	 else if(Minit_Times<190)
	 {
		if(Minit_Times<20)
    {
		Temp_const=16;
		}
		else if(Minit_Times<30)	
		{
		Temp_const=21;
		}	
		else if(Minit_Times<40)	
		{
		Temp_const=25;
		}	
		else if(Minit_Times<50)	
		{
		Temp_const=28;
		}	
		else if(Minit_Times<60)	
		{
		Temp_const=30;
		}	
		else if(Minit_Times<70)	
		{
		Temp_const=31;
		}	
		else if(Minit_Times<80)	
		{
		Temp_const=32;
		}	
		else if(Minit_Times<90)	
		{
		Temp_const=33;
		}	
		else if(Minit_Times<100)	
		{
		Temp_const=34;
		}	
		else if(Minit_Times<110)	
		{
		Temp_const=35;
		}
    else if(Minit_Times<120)	
		{
		Temp_const=36;
		}	
    else if(Minit_Times<130)	
		{
		Temp_const=37;
		}	
    else if(Minit_Times<140)	
		{
		Temp_const=38;
		}		
    else if(Minit_Times<150)	
		{
		Temp_const=39;
		}
    else if(Minit_Times<160)	
		{
		Temp_const=40;
		}	
    else if(Minit_Times<170)	
		{
		Temp_const=41;
		}
    else 	Temp_const=42;	
		if(OutTempIsHigh==1)
		{
		
		Temp_const=42;
		if(Delt_last<1)JBE.Temp_value=JBE.Temp_value_mid-Temp_const;
		else if(Delt_last<2)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const; 
	
	  
		//else JBE.Temp_value=JBE.Temp_value_mid-Temp_Delt;		//如果外部温度急剧增这时温度不做修正
		else 
		{
	  if(Delt_last<3)JBE.Temp_value=JBE.Temp_value_mid-42;
		else if(Delt_last<=5) JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-3)/5)*2-40;
		else if(Delt_last<=10) JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-5)/5)*5-35;
		else if(Delt_last<=16)JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-10)/6)*10-30;
		else if(Delt_last<=26)JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-16)/20)*10-25;	
		else if(Delt_last<=36)JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-16)/20)*10-20;			
		else if(Delt_last<=46)JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-36)/10)*10-15;
    else if(Delt_last<=56)JBE.Temp_value=JBE.Temp_value_mid-(1-(Delt_last-46)/10)*10-10;			
		else JBE.Temp_value=JBE.Temp_value_mid-10*(1-(Delt_last-56)/100);
		}
		}
		else
		{
		if(Delt_last<1)JBE.Temp_value=JBE.Temp_value_mid-Temp_const;
		else if(Delt_last<2)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const; 
	  else if(Delt_last<3)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const; 
		else if(Delt_last<4)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const;  
		else if(Delt_last<5)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const; 
		else if(Delt_last<6)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const;  
		else if(Delt_last<7)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const;  
		else if(Delt_last<8)JBE.Temp_value=JBE.Temp_value_mid-(1-Delt_last/15)*Temp_const;  
		else if(Delt_last<9)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const;  
	  else if(Delt_last<10)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const; 
		else if(Delt_last<11)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const;  
		else if(Delt_last<12)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const; 
		else if(Delt_last<13)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const;  
		else if(Delt_last<14)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const;  
		else if(Delt_last<15)JBE.Temp_value=JBE.Temp_value_mid-(Delt_last/15)*Temp_const;   
		//else JBE.Temp_value=JBE.Temp_value_mid-Temp_Delt;		//如果外部温度急剧增这时温度不做修正
		else JBE.Temp_value=JBE.Temp_value_mid-Temp_Delt;	
		
		}

	 }

	// else if(Minit_Times<90)JBE.Temp_value=JBE.Temp_value_mid-Temp_Delt-55*((80-Temp_Delt)/80);
	 else
	 {
		 JBE.Temp_value=JBE.Temp_value_mid-Minit_Times*42/190;//(JBE.Temp_value_mid-((GET_NtcTEMP()+5-JBE.Temp_value_mid)));
		 OutTempIsHigh=0;
	 }
		 
	 JBE.Hub_value_mid=JBE.ReadBuf[3];
	 JBE.Hub_value_mid=JBE.Hub_value_mid<<8;
	 JBE.Hub_value_mid=JBE.Hub_value_mid+JBE.ReadBuf[4];
	 JBE.Hub_value_mid=(float)JBE.Hub_value_mid/65536*100;
	 if(JBE.FirstTimeGetTempFlag==0)
	 {
	 JBE.Temp_value_Intinal=JBE.Temp_value_mid;
	 JBE.Hub_value_Intinal =JBE.Hub_value_mid;
	 JBE.FirstTimeGetTempFlag=1; 
	 }
	 if(Minit_Times<90)
	 {
	 if(JBE.Hub_value_mid>30)
	 {
	 JBE.Hub_value=JBE.Hub_value_mid+Hub_Delt;
	 }
	 else JBE.Hub_value=JBE.Hub_value_mid+Hub_Delt;
   }
	 else
	 {
	  if(JBE.Hub_value_mid>30)
	 {
	 JBE.Hub_value=JBE.Hub_value_mid+Minit_Times*16/90;
	 }
	 else JBE.Hub_value=JBE.Hub_value_mid+Minit_Times*8/90;
	 
	 }
	 LCD_Display_TempHub(JBE.Temp_value,JBE.Hub_value);	 
	 
	 
   }
	 //LCD_Display_TempHub(JBE.Hub_value,HUB);
 }
 else if(JBE.SHTC3_Sta== Readover)
 {
  JBE.CmdBuf [0]=0xB0;
	JBE.CmdBuf [1]=0x98;
	I2C_Write(&JBE.CmdBuf,sizeof(JBE.CmdBuf));
	JBE.SHTC3_Sta=Sleep;
	Clr__TempHutStartFlag();            //温湿度测量启动标志清零，等待下一秒开启
 }
 #endif
 
}
void I2C_Write_Process(UINT8 u8DAT)
{
    unsigned char  u8Count;
    /* Write Step1 */
	  set_I2C0CON_STA;                                      /*STA置1位主机发送模式，硬件发出起始信号，如果起始信号发送成功，SI 置1*/                                /* Send Start bit to I2C EEPROM */
    clr_I2C0CON_SI;
	  while (!(I2C0CON&SET_BIT3));                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
	  if (I2C0STAT != 0x08)                                 /*状态码为0x08的时候说明起始信号发送成功*/                /*Check status value after every step   */
        I2C_Error();
    /*发送从机地址和写标志*/
    /* Write Step2 */
    clr_I2C0CON_STA;                                      /*STA=0*/
    I2C0DAT = (I2C_SLAVE_ADDRESS | I2C_WR);
    clr_I2C0CON_SI;
    while (!(I2C0CON&SET_BIT3));                          /*Check SI set or not */
    if (I2C0STAT != 0x18)                                 /*从机地址已发，收到了应答，状态为0x18*/
        I2C_Error();

    /* Write Step3 */
    for (u8Count = 0; u8Count < LOOP_SIZE; u8Count++)     /*循环发送自定义数据*/
    {
        I2C0DAT = u8DAT;
        clr_I2C0CON_SI;
        while (!(I2C0CON&SET_BIT3));                       /*Check SI set or not*/
        if (I2C0STAT != 0x28)
            I2C_Error();

        u8DAT = ~u8DAT;
    }

    /* Write Step4 */
    set_I2C0CON_STO;    /*停止标志位，主机模式下设置为1时向总线发送停止信号*/
    clr_I2C0CON_SI;
    while (I2C0CON&SET_BIT4);                               /* Check STOP signal */
}
  
//--------------------------------------------------------------------------------------------
//----  Page Read ----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void I2C_Read_Process(UINT8 u8DAT)
{
    unsigned char  u8Count;
    /* Read Step1 */
    set_I2C0CON_STA;
    clr_I2C0CON_SI;          
    while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
	  if (I2C0STAT != 0x08)                         /*起始位发送成功*///Check status value after every step
        I2C_Error();

    /* Step13 */
    clr_I2C0CON_STA;                                    //STA needs to be cleared after START codition is generated
	  I2C0DAT = (I2C_SLAVE_ADDRESS | I2C_RD);       /*发送从机地址，和读标志位*/
    clr_I2C0CON_SI;
    while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
	if (I2C0STAT != 0x40)                         /*发送成功收到从机应答*/
        I2C_Error();
    
    /* Step14 */
    for (u8Count = 0; u8Count <LOOP_SIZE; u8Count++)
    {
        set_I2C0CON_AA;
        clr_I2C0CON_SI;        
        while (!(I2C0CON&SET_BIT3));                            //Check SI set or not

        if (I2C0STAT != 0x50)              
            I2C_Error();
        
        if (I2C0DAT != u8DAT)             
            I2C_Error();
        u8DAT = ~u8DAT; 
    } 
    
    /* Step15 */
    clr_I2C0CON_AA;
    clr_I2C0CON_SI;
    while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
    if (I2C0STAT != 0x58)
        I2C_Error();

    /* Step16 */
    set_I2C0CON_STO;
    clr_I2C0CON_SI;
    while (I2C0CON&SET_BIT4);                                /* Check STOP signal */ 
}
//========================================================================================================

uint16_t Get_TempValue(void)
{
  return JBE.Temp_value;
}


uint16_t Get_TempValueMid(void)
{
  return JBE.Temp_value_mid;
}

uint16_t Get_TempValueInit(void)
{
  return JBE.Temp_value_Intinal;
}



uint16_t Get_Hub_value_Mid(void)
{
  return JBE.Hub_value_mid;
}

uint16_t Get_Hub_value_Inti(void)
{
  return JBE.Hub_value_Intinal;
}











