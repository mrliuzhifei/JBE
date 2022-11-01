#include "ML51.h"

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
 uint16_t Hub_value;
 uint8_t crc8;	
 uint8_t OverTimeStartCount;  //开始检测是否超时
 uint8_t TimeIsOver;
 uint8_t ThisPositiveTemp;
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
	  while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
		 {
		 JBE.OverTimeStartCount=True;    //开始检测是否超时
		 if(JBE.TimeIsOver==True)            //如果超时了
		 {
		  JBE.OverTimeStartCount=False;
			JBE.TimeIsOver=False;
			I2C_Error(); 
      return;			 
		 }
		 }
		 JBE.OverTimeStartCount=False;
		 JBE.TimeIsOver=False;
	  if (I2C0STAT != 0x08)                                 /*状态码为0x08的时候说明起始信号发送成功*/                /*Check status value after every step   */
        I2C_Error();
    /*发送从机地址和写标志*/
    /* Write Step2 */
    clr_I2C0CON_STA;                                      /*STA=0*/
    I2C0DAT = (I2C_SLAVE_ADDRESS | I2C_WR);
    clr_I2C0CON_SI;
   // while (!(I2C0CON&SET_BIT3));                          /*Check SI set or not */
		while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
		 {
		 JBE.OverTimeStartCount=True;    //开始检测是否超时
		 if(JBE.TimeIsOver==True)            //如果超时了
		 {
		  JBE.OverTimeStartCount=False;
			JBE.TimeIsOver=False;
			I2C_Error(); 
      return;			 
		 }
		 }
		 JBE.OverTimeStartCount=False;
		 JBE.TimeIsOver=False;
    if (I2C0STAT != 0x18)                                 /*从机地址已发，收到了应答，状态为0x18*/
        I2C_Error();

    /* Write Step3 */
    for (u8Count = 0; u8Count < len; u8Count++)     /*循环发送自定义数据*/
    {
			 
        I2C0DAT = *pbuf;
			  pbuf++;
        clr_I2C0CON_SI;
       // while (!(I2C0CON&SET_BIT3));                       /*Check SI set or not*/
			    while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
					 {
					 JBE.OverTimeStartCount=True;    //开始检测是否超时
					 if(JBE.TimeIsOver==True)            //如果超时了
					 {
						JBE.OverTimeStartCount=False;
						JBE.TimeIsOver=False;
						I2C_Error(); 
						return;			 
					 }
					 }
					 JBE.OverTimeStartCount=False;
					 JBE.TimeIsOver=False;
        if (I2C0STAT != 0x28)
            I2C_Error();
    }

    /* Write Step4 */
    set_I2C0CON_STO;    /*停止标志位，主机模式下设置为1时向总线发送停止信号*/
    clr_I2C0CON_SI;
   // while (I2C0CON&SET_BIT4); 
		  while (!(I2C0CON&SET_BIT4))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
					 {
					 JBE.OverTimeStartCount=True;    //开始检测是否超时
					 if(JBE.TimeIsOver==True)            //如果超时了
					 {
						JBE.OverTimeStartCount=False;
						JBE.TimeIsOver=False;
						I2C_Error(); 
						return;			 
					 }
					 }
					 JBE.OverTimeStartCount=False;
					 JBE.TimeIsOver=False;

}

void I2C_Read(UINT8 *pbuf,len)
{
    unsigned char  u8Count;
    /* Read Step1 */
    set_I2C0CON_STA;
    clr_I2C0CON_SI;          
   // while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
	  while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
		 {
		 JBE.OverTimeStartCount=True;    //开始检测是否超时
		 if(JBE.TimeIsOver==True)            //如果超时了
		 {
			JBE.OverTimeStartCount=False;
			JBE.TimeIsOver=False;
			I2C_Error(); 
			return;			 
		 }
		 }
		 JBE.OverTimeStartCount=False;
		 JBE.TimeIsOver=False;
	if (I2C0STAT != 0x08)                         /*起始位发送成功*///Check status value after every step
        I2C_Error();

    /* Step13 */
    clr_I2C0CON_STA;                                    //STA needs to be cleared after START codition is generated
	  I2C0DAT = (I2C_SLAVE_ADDRESS | I2C_RD);       /*发送从机地址，和读标志位*/
    clr_I2C0CON_SI;
    //while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
	  while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
		 {
		 JBE.OverTimeStartCount=True;    //开始检测是否超时
		 if(JBE.TimeIsOver==True)            //如果超时了
		 {
			JBE.OverTimeStartCount=False;
			JBE.TimeIsOver=False;
			I2C_Error(); 
			return;			 
		 }
		 }
		 JBE.OverTimeStartCount=False;
		 JBE.TimeIsOver=False;
	if (I2C0STAT != 0x40)                         /*发送成功收到从机应答*/
        I2C_Error();
    
    /* Step14 */
    for (u8Count = 0; u8Count <len; u8Count++)
    {
        set_I2C0CON_AA;
        clr_I2C0CON_SI;        
       //while (!(I2C0CON&SET_BIT3));                            //Check SI set or not
         while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
					 {
					 JBE.OverTimeStartCount=True;    //开始检测是否超时
					 if(JBE.TimeIsOver==True)            //如果超时了
					 {
						JBE.OverTimeStartCount=False;
						JBE.TimeIsOver=False;
						I2C_Error(); 
						return;			 
					 }
					 }
					 JBE.OverTimeStartCount=False;
					 JBE.TimeIsOver=False;
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
    //while (!(I2C0CON&SET_BIT3));                                //Check SI set or not
		  while (!(I2C0CON&SET_BIT3))//;                          /*SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
					 {
					 JBE.OverTimeStartCount=True;    //开始检测是否超时
					 if(JBE.TimeIsOver==True)            //如果超时了
					 {
						JBE.OverTimeStartCount=False;
						JBE.TimeIsOver=False;
						I2C_Error(); 
						return;			 
					 }
					 }
					 JBE.OverTimeStartCount=False;
					 JBE.TimeIsOver=False;
    if (I2C0STAT != 0x58)
        I2C_Error();

    /* Step16 */
    set_I2C0CON_STO;
    clr_I2C0CON_SI;
   //while (I2C0CON&SET_BIT4);                                /* Check STOP signal */ 
		  while (!(I2C0CON&SET_BIT4))//;                          /* SI置1说明起始信号发送成功*/                            /*Check SI set or not  */
					 {
					 JBE.OverTimeStartCount=True;    //开始检测是否超时
					 if(JBE.TimeIsOver==True)            //如果超时了
					 {
						JBE.OverTimeStartCount=False;
						JBE.TimeIsOver=False;
						I2C_Error(); 
						return;			 
					 }
					 }
					 JBE.OverTimeStartCount=False;
					 JBE.TimeIsOver=False;
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
   I2C_Read(&JBE.ReadBuf,sizeof(JBE.ReadBuf));
	 JBE.SHTC3_Sta=Readover; 
	  JBE.crc8=crc8_nrsc5(&JBE.ReadBuf[0],2);
	  JBE.crc8=crc8_nrsc5(&JBE.ReadBuf[3],2);
	 if(crc8_nrsc5(&JBE.ReadBuf[0],2)==JBE.ReadBuf[2]&&crc8_nrsc5(&JBE.ReadBuf[3],2)==JBE.ReadBuf[5])
	 {
	 JBE.Temp_value_mid=JBE.ReadBuf[0];
	 JBE.Temp_value_mid=JBE.Temp_value_mid<<8;
	 JBE.Temp_value_mid=JBE.Temp_value_mid+JBE.ReadBuf[1];
	 
	 
	 JBE.Temp_value_mid=(float)JBE.Temp_value_mid/65536*175*10-15;
	 //JBE.Temp_value_mid=234; 
	 if(JBE.Temp_value_mid>=450)                                  //说明获得的是正温度
	 {
	 JBE.Temp_value_mid=JBE.Temp_value_mid-450;    
	 JBE.ThisPositiveTemp=True;
	 }
	 else 
	 {
	 JBE.Temp_value_mid=450-JBE.Temp_value_mid;
	 JBE.ThisPositiveTemp=False;
	 }
	 
	 JBE.Temp_value=JBE.Temp_value_mid;//(JBE.Temp_value_mid-((GET_NtcTEMP()+5-JBE.Temp_value_mid)));
		 
	 JBE.Hub_value=JBE.ReadBuf[3];
	 JBE.Hub_value=JBE.Hub_value<<8;
	 JBE.Hub_value=JBE.Hub_value+JBE.ReadBuf[4];
	 JBE.Hub_value=(float)JBE.Hub_value/65536*100+10;	 
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

uint16_t Get_HubValue(void)
{
  return JBE.Hub_value;
}

void Is_IIC_OverTime(void)
{
 static uint8_t countT=0;
 if(JBE.OverTimeStartCount==True) //开始计数
 {
 countT++;
 if(countT>10)                    //等待超出10ms
 {
 JBE.TimeIsOver=True;              //已超时
 countT=0; 
 }
	 
 }

}

uint8_t Get_TempPositive(void)
{
return JBE.ThisPositiveTemp;
}

void Set_TempPositive(void)
{
JBE.ThisPositiveTemp=True;
}



