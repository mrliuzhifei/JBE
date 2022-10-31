#include "ML51.h"
typedef struct SensorData_t
{
 SavePage_s SavePage;                    //存在哪一页
 UINT8 Write_SavePageBuf[6];             //存储当前数据存储的位置
 UINT8 Read_SavePageBuf[6];	
 UINT8 Write_CO2SaveBuf[128];            //二氧化碳数据存储数组
 UINT8 Read_CO2SaveBuf[128];             //二氧化碳数据存储数组
 UINT8 IsCO2Save1Cycle;                  //是否二氧化碳数据存储满24小时
 UINT8 ArrIndex	;	//数组下标
 UINT32 Max_Valu_CO2_24h;  //24小时最大值
 UINT32 Avg_Valu_CO2_24h;  //24小时平均值
 unsigned int Crc16;
}SensorData_t;  
SensorData_t SensorData;
#define JBE SensorData
volatile unsigned char xdata page_buffer[128];
void Write_DATAFLASH (unsigned int u16EPAddr, unsigned char *u8EPData,unsigned char len)
{
    unsigned char looptmp = 0;
    unsigned int u16_addrl_r;
    //unsigned int RAMtmp;
    //Check page start address
    u16_addrl_r = (u16EPAddr / 128) * 128;

    //Save APROM data to XRAM0
    for (looptmp = 0; looptmp < len; looptmp++)
    {
        //RAMtmp = Read_APROM_BYTE((unsigned int code *)(u16_addrl_r + looptmp));
			  if(looptmp<len)
				{
				page_buffer[looptmp] =*u8EPData;
				u8EPData++;
				}
				else page_buffer[looptmp] =0;
        
    }
    //Modify customer data in XRAM
    //page_buffer[u16EPAddr & 0x7f] = u8EPData;
    //Erase APROM DATAFLASH page
    IAPAL = u16_addrl_r & 0xff;
    IAPAH = (u16_addrl_r >> 8) & 0xff;
    IAPFD = 0xFF;
    set_CHPCON_IAPEN;
    set_IAPUEN_APUEN;
    IAPCN = 0x22;
    set_IAPTRG_IAPGO;

    //Save changed RAM data to APROM DATAFLASH
    set_CHPCON_IAPEN;
    set_IAPUEN_APUEN;
    IAPCN = 0x21;

    for (looptmp = 0; looptmp < len
		; looptmp++)
    {
        IAPAL = (u16_addrl_r & 0xff) + looptmp;
        IAPAH = (u16_addrl_r >> 8) & 0xff;
        IAPFD = page_buffer[looptmp];
        set_IAPTRG_IAPGO;
    }

    clr_IAPUEN_APUEN;
    clr_CHPCON_IAPEN;
}

void EEPROM_Write_SensorData(EEPROM_SaveType_s SaveType, DataType_s DataType)
{
switch (SaveType)
{
	unsigned int Crc16;
	case CO2:
	{
		if(DataType==PAGE)
		{
			if(JBE.SavePage==CO2_Page1)  
			{
//		 JBE.ISOneCircleSave=True;
			 JBE.SavePage=CO2_Page2;                                                                     //当第一页存满以后，从第二页开始存
			 JBE.Write_SavePageBuf[0]=CO2_Page2; 	  
			}	 
			else if(JBE.SavePage==CO2_Page2)
			{
			JBE.SavePage=CO2_Page1; 
			JBE.Write_SavePageBuf[0]=CO2_Page1;                                                          //当第二页存满以后，从第一页开始存 循环往复
			}	
			JBE.Write_SavePageBuf[5]=crc8_nrsc5(&JBE.Write_SavePageBuf, 5);
			EA=0;//关中断
			Write_DATAFLASH (BaseAddress_Page,&JBE.Write_SavePageBuf,sizeof(JBE.Write_SavePageBuf));     //存PAGE
			Read_DATAFLASH_ARRAY(BaseAddress_Page,&JBE.Read_SavePageBuf,sizeof(JBE.Read_SavePageBuf));   //读取写入的数据和原始数据对比，是否有错，如果有错在写一次
			if(JBE.Read_SavePageBuf[5]!=crc8_nrsc5(&JBE.Read_SavePageBuf,5))
			{
			Write_DATAFLASH (BaseAddress_Page,&JBE.Write_SavePageBuf,sizeof(JBE.Write_SavePageBuf));     //存PAGE
			}	 
			EA=1;//开中断	
		}
     if(DataType==Sensor)	
    {
  	 JBE.Write_CO2SaveBuf[Get_SaveAddr()]  =Get_Co2Value()>>8;                                         //计算CRC 校验码
		 JBE.Write_CO2SaveBuf[Get_SaveAddr()+1]=Get_Co2Value();
		 JBE.Write_CO2SaveBuf[124]=Get_OneCircleSaveFlag();
		 JBE.Write_CO2SaveBuf[125]=Get_SaveAddr();
		 Crc16=CRC16MODBUS(&JBE.Write_CO2SaveBuf[0], (sizeof(JBE.Write_CO2SaveBuf)-2));	
		 JBE.Write_CO2SaveBuf[126]=Crc16>>8;
		 JBE.Write_CO2SaveBuf[127]=Crc16;
		 EA=0;//关中断
		 Write_DATAFLASH (BaseAddress+JBE.SavePage*128,&JBE.Write_CO2SaveBuf,sizeof(JBE.Write_CO2SaveBuf));  //存CO2数据
		 
		 Read_DATAFLASH_ARRAY(BaseAddress+JBE.SavePage*128,&JBE.Read_CO2SaveBuf,sizeof(JBE.Read_CO2SaveBuf));//读取写入的数据和原始数据对比，是否有错，如果有错在写一次	
		 JBE.Crc16=CRC16MODBUS(&JBE.Read_CO2SaveBuf[0],(sizeof(JBE.Read_CO2SaveBuf)-2));
     if((JBE.Read_CO2SaveBuf[127])!=JBE.Crc16)
			{
			Write_DATAFLASH (BaseAddress+JBE.SavePage*128,&JBE.Write_CO2SaveBuf,sizeof(JBE.Write_CO2SaveBuf)); //存CO2数据
			}	 			
		 EA=1;//开中断 	
		}			
	}
	break;
	case SHTC3:
		
	break;
	case HCHO:
		
	break;
	case PM25:
		
	break;
	case TVCO:
		
	break;
	default: break;	
}
}


void EEPROM_ReadDataInit(void)
{
 unsigned char OneCircle_Flag;
 unsigned char Save_AddrP;
 unsigned char CO2Value_H;
 unsigned char CO2Value_L;
 unsigned char i;	 
 EA=0;
 Read_DATAFLASH_ARRAY(BaseAddress_Page,&JBE.Read_SavePageBuf,sizeof(JBE.Read_SavePageBuf)); 
 if(JBE.Read_SavePageBuf[5]!=crc8_nrsc5(&JBE.Read_SavePageBuf,5))                                   //读page页如果不成功再读一次
	{
	Read_DATAFLASH_ARRAY(BaseAddress_Page,&JBE.Read_SavePageBuf,sizeof(JBE.Read_SavePageBuf));        //还是不成功就把页数组初始化为0
  if(JBE.Read_SavePageBuf[5]!=crc8_nrsc5(&JBE.Read_SavePageBuf,5))
	{
	JBE.Write_SavePageBuf[0]=0;
	JBE.Write_SavePageBuf[1]=0;	
	JBE.Write_SavePageBuf[2]=0;	
	JBE.Write_SavePageBuf[3]=0;	
	JBE.Write_SavePageBuf[4]=0;	
	JBE.SavePage=JBE.Write_SavePageBuf[0];		
	}
	else
	{
	JBE.Write_SavePageBuf[0]=JBE.Read_SavePageBuf[0];                                                  //成功读出page页
	JBE.Write_SavePageBuf[1]=JBE.Read_SavePageBuf[1];	
	JBE.Write_SavePageBuf[2]=JBE.Read_SavePageBuf[2];	
	JBE.Write_SavePageBuf[3]=JBE.Read_SavePageBuf[3];	
	JBE.Write_SavePageBuf[4]=JBE.Read_SavePageBuf[4];	
	JBE.Write_SavePageBuf[5]=JBE.Read_SavePageBuf[5];	
  JBE.SavePage=JBE.Write_SavePageBuf[0];		
	}
		
	}
	else
	{
	JBE.Write_SavePageBuf[0]=JBE.Read_SavePageBuf[0];                                                   //成功读出page页
	JBE.Write_SavePageBuf[1]=JBE.Read_SavePageBuf[1];	
	JBE.Write_SavePageBuf[2]=JBE.Read_SavePageBuf[2];	
	JBE.Write_SavePageBuf[3]=JBE.Read_SavePageBuf[3];	
	JBE.Write_SavePageBuf[4]=JBE.Read_SavePageBuf[4];	
	JBE.Write_SavePageBuf[5]=JBE.Read_SavePageBuf[5];	
	JBE.SavePage=JBE.Write_SavePageBuf[0];	
	}
 
	Read_DATAFLASH_ARRAY(BaseAddress+JBE.SavePage*128,&JBE.Read_CO2SaveBuf,sizeof(JBE.Read_CO2SaveBuf));
	JBE.Crc16=CRC16MODBUS(&JBE.Read_CO2SaveBuf,(sizeof(JBE.Read_CO2SaveBuf)-2));	
	JBE.Crc16=JBE.Read_CO2SaveBuf[126];
	JBE.Crc16=JBE.Crc16<<8;
	JBE.Crc16=JBE.Crc16+JBE.Read_CO2SaveBuf[127];
		
	if(JBE.Crc16!=CRC16MODBUS(&JBE.Read_CO2SaveBuf,(sizeof(JBE.Read_CO2SaveBuf)-2)))                    //如果读不成功
	{
	Read_DATAFLASH_ARRAY(BaseAddress+JBE.SavePage*128,&JBE.Read_CO2SaveBuf,sizeof(JBE.Read_CO2SaveBuf));//再读一次
	
	JBE.Crc16=JBE.Read_CO2SaveBuf[126];
	JBE.Crc16=JBE.Crc16<<8;
	JBE.Crc16=JBE.Crc16+JBE.Read_CO2SaveBuf[127];	
	if(JBE.Crc16!=CRC16MODBUS(&JBE.Read_CO2SaveBuf,(sizeof(JBE.Read_CO2SaveBuf)-2)))                    //如果还不成功
	{
	for(i=0;i<128;i++)
		{
		JBE.Write_CO2SaveBuf[i]=0;                                                                        //把所有值清零
		}
	}
	else                                                                                                //如果成功了 把读到的值赋值给Page
	{
	for(i=0;i<128;i++)
		{
		JBE.Write_CO2SaveBuf[i]=JBE.Read_CO2SaveBuf[i]; 
		}
		OneCircle_Flag=JBE.Write_CO2SaveBuf[124];
		Save_AddrP=JBE.Write_CO2SaveBuf[125];
		CO2Value_H=JBE.Write_CO2SaveBuf[Save_AddrP] ;
		CO2Value_L=JBE.Write_CO2SaveBuf[Save_AddrP+1];
		Set_OneCircle_Flag(OneCircle_Flag);
		Set_Save_AddrP(Save_AddrP);
	}
	}
	else
	{
	for(i=0;i<128;i++)
		{
		JBE.Write_CO2SaveBuf[i]=JBE.Read_CO2SaveBuf[i]; 
		}
	  OneCircle_Flag=JBE.Write_CO2SaveBuf[124];
		Save_AddrP=JBE.Write_CO2SaveBuf[125];
		CO2Value_H=JBE.Write_CO2SaveBuf[Save_AddrP] ;
		CO2Value_L=JBE.Write_CO2SaveBuf[Save_AddrP+1];
		Set_OneCircle_Flag(OneCircle_Flag);
		Set_Save_AddrP(Save_AddrP);
	}
	EA=1;
}


void Get_Max_Valu_24h(void)
{
unsigned char i;	
unsigned int Max_Valu_24h;
unsigned int Max_Valu_24h_Temp;	
for(i=0;i<96;i=i+2)
	{	
	Max_Valu_24h_Temp=JBE.Write_CO2SaveBuf[i];
	Max_Valu_24h_Temp=Max_Valu_24h_Temp<<8;
	Max_Valu_24h_Temp=Max_Valu_24h_Temp+JBE.Write_CO2SaveBuf[i+1];	
	if(Max_Valu_24h<Max_Valu_24h_Temp)	
	{
	Max_Valu_24h=Max_Valu_24h_Temp;
	}	
	}
  JBE.Max_Valu_CO2_24h=Max_Valu_24h;
}

void Get_Avg_Valu_24h(void)
{
	unsigned char i;	
  unsigned int  Avg_Valu_24h=0;
  unsigned int  Avg_Valu_24h_Temp=0;	
	unsigned long Avg_Valu_24h_Sum=0;
  if(JBE.Write_CO2SaveBuf[124]==0)
	{
		for(i=0;i<(JBE.Write_CO2SaveBuf[125]+2);i=i+2)
		{
		 Avg_Valu_24h_Temp=JBE.Write_CO2SaveBuf[i];
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp<<8;
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp+JBE.Write_CO2SaveBuf[i+1];	
		 Avg_Valu_24h_Sum=Avg_Valu_24h_Sum+ Avg_Valu_24h_Temp;
		}
		Avg_Valu_24h=Avg_Valu_24h_Sum*2/(JBE.Write_CO2SaveBuf[125]+2);
			  
	}
	else 
	{
		for(i=0;i<96;i=i+2)
		{
		 Avg_Valu_24h_Temp=JBE.Write_CO2SaveBuf[i];
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp<<8;
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp+JBE.Write_CO2SaveBuf[i+1];	
		 Avg_Valu_24h_Sum=Avg_Valu_24h_Sum+ Avg_Valu_24h_Temp;
		}
		Avg_Valu_24h=Avg_Valu_24h_Sum/48;
	
	}
	
	
	
  JBE.Avg_Valu_CO2_24h=Avg_Valu_24h;
}

unsigned int Get_Max_Valu_CO2_24h(void)
{
 return JBE.Max_Valu_CO2_24h;
}

unsigned int Get_Avg_Valu_CO2_24h(void)
{
 return JBE.Avg_Valu_CO2_24h;
}



