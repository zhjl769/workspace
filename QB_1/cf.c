
/*******************************************************************************
* FILENAME
*   cf.c
*
* DESCRIPTION
*   主要完成CF卡的读写和FAT文件系统的建立
*	每个文件50MB，总共270个文件
*******************************************************************************/
#include "cf.h"
#define CF_SecByte_num 512 /*每扇区字节数*/
#define CF_DirSec_num 128 /*为CF卡根目录区预留两个簇*/
#define Clu_Sec_num 16 /*每个根目录扇区16个文件*/
#define File_Dir_num 32 /*每个文件DIR占用32字节*/
#define CF_File_num 270  /*CF卡写文件数*/
#define File_FATSec_num 55 //每个文件的大小，单位MB
#define CF_sectors_num 224  //每次写16*14=224个扇区，
#define CF_Write_Bytes 114688 //每次写224*512=114688字节数
#define FAT_sectors_num 15125 //15125*4/55=1100个文件
unsigned char CF_Wsector_buffer[CF_SecByte_num];
unsigned char CF_Wsectors_buffer[CF_Write_Bytes];
unsigned char CF_Rsector_buffer[CF_SecByte_num];
unsigned char Dir_buffer[File_Dir_num];

/*MBR和DBR的地址其实不一样*/
unsigned int MBRAdd = 0xE0000000;
unsigned int MBRResrSec = 0;
unsigned int DBRAdd = 0;
unsigned short DBRResrSec = 0;     /*CF卡保留扇区总数*/
unsigned int SecPerPart = 0;       /*CF卡的扇区总数*/
unsigned int SecPerFAT = 0;        /*CF卡每个FAT表的扇区总数*/
unsigned int RootClus = 0;         /*CF卡根目录DIR起始簇号*/
unsigned int FAT1Add = 0;
unsigned int FAT2Add = 0;
unsigned int DIRAdd = 0;
unsigned int DataAdd = 0;
unsigned char QB_Num = 0;
unsigned int QB_Num_LBAadd = 0xE1DD40AF;

int file_count=0;   // 当前文件号；

/****************************************************************************
*void Init_CF(void)                                                         *
*						                                                    *
*	作用：初始化CF卡,包括读取MBR信息、形成FAT表等 初始化完成之后就可以写文件*
*          			                                                        * 
*****************************************************************************/
void Init_CF(void)
{
	Read_MBR_sector();
	MBRResrSec = ConvertCharArrayToInt(&CF_Rsector_buffer[0x1C9]);
		
	if(CF_Rsector_buffer[0]==0xEB)//判读CF卡MBR区是否损坏
	{
		DBRAdd = MBRAdd;	
	}
	else
	{
		DBRAdd = MBRResrSec + MBRAdd;
	}

	CF_ReadSector(DBRAdd);//读取DBR数据
	
	CF_Addr_information();//获得CF卡地址信息

	//新卡要进行这个过程，格式化文件目录区-------------------------------------------------------------------------------
	//Clear_Dir();
//	Form_FAT1();//形成FAT1表
    Form_FAT();
	//旧卡之前的2个函数屏蔽就好--------------------------------------------------------------------------------------------

	File_num_test();  // 检测目录是否有旧文件；

//  写前标号,对于一张新卡只用写一次
//	Write_QB_num();  
//
	Read_QB_num();//读取潜标号
	
}

/****************************************************************************
*void Write_File_dir(void)                                                  *
*	作用：写文件的根目录区				                                    *
*  	说明： 实参File_num为形成的文件编号，从0开始                            *
*****************************************************************************/
void Write_File_dir(unsigned int File_num)
{
	int i;
	int j;
	int k;
	unsigned int temp;
	unsigned int Dir_Sector_index;//扇区索引
	unsigned int Dir_Block_index; //块索引

	Dir_Sector_index = File_num / Clu_Sec_num;
	Dir_Block_index = File_num % Clu_Sec_num;

	for(i=0;i<File_Dir_num;i++)
	{
		Dir_buffer[i]=0 ;
	}

	CF_ReadSector(DIRAdd + Dir_Sector_index);//读取DIR扇区数据
	
	for(k=0;k<CF_SecByte_num;k++)
	{
		CF_Wsector_buffer[k] = CF_Rsector_buffer[k];
	}
	
	temp = File_num;

	/*写根目录缓冲区*/
	Dir_buffer[0]='Q';//文件正名
	Dir_buffer[1]='B';
	Dir_buffer[2]=QB_Num;
	Dir_buffer[3]='_';
	
	Dir_buffer[4]=(temp / 1000) + 0x30;
	Dir_buffer[5]=(temp / 100) % 10 + 0x30;
	Dir_buffer[6]=(temp / 10) % 10 + 0x30;
	Dir_buffer[7]=(temp % 10) + 0x30;

	Dir_buffer[8]='D';//文件扩展名
	Dir_buffer[9]='A';
	Dir_buffer[10]='T';
	Dir_buffer[11]=0x20;//文件属性
	
//	temp = File_num * 0x1B80+0x04;//0x80*N 为N MB的文件
    temp = File_num * 0x6E0+0x04;//64G卡修改
	Dir_buffer[20]=(temp>>16) & 0xff;//文件起始簇编号的高16位
	Dir_buffer[21]=(temp>>24) & 0xff;
	Dir_buffer[26]= temp & 0xff;; //文件起始簇编号的低16位
	Dir_buffer[27]=(temp>>8) & 0xff;
	
	Dir_buffer[28]=0x00;//每个文件字节长度 1MB
	Dir_buffer[29]=0x00;//文件属性
	Dir_buffer[30]=0x70;//0x10*N 为N MB的文件
	Dir_buffer[31]=0x03;

	for(j=0;j<File_Dir_num;j++)
	{
		CF_Wsector_buffer[j + Dir_Block_index * File_Dir_num] = Dir_buffer[j];
	}
	
	CF_WriteSector(DIRAdd + Dir_Sector_index);

}

/****************************************************************************
*void Read_MBR_sector(void)                                                 *
*						                                                    *
*	作用：读取MBR信息                                                       *
*          			                                                        * 
*****************************************************************************/
void Read_MBR_sector(void)
{
	unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	unsigned short CF_SecRead_cnt = 0;
	int i;
	
 /*设定数据寄存器位数*/
	*(unsigned char *)(EMA_CE2+CF_FeaturesReg) = 0x01; //使能8bit数据传输

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //PIO默认模式

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = 0xE0; //write Drive Head Register and LBA27:24
	
	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0xEF; //write Command Register

	Detect_StatusReg_81();

 /*设定PIO模式数据传输*/
	*(unsigned char *)(EMA_CE2+CF_FeaturesReg) = 0x03; //Set Transfer Mode based on value and Sector Count register.

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x0A; //PIO Flow Control Transfer Mode 2 

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = 0xE0; //write Drive Head Register and LBA27:24

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0xEF; //write Command Register

	Detect_StatusReg_81();	
 
 
 /*读取CF卡MBR扇区*/
	ConvertIntToCharArray(MBRAdd, CF_LBAAddr_Buffers);

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //要读硬盘的扇区个数

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16数

	*(unsigned char *)(EMA_CE2+CF_CylindersLReg) = CF_LBAAddr_Buffers[1]; //write Cylinder Low Register or LBA15:8

	*(unsigned char *)(EMA_CE2+CF_SectorsNumReg) = CF_LBAAddr_Buffers[0]; //write Sector Number Register or LBA7:0

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0x21; //write Command Register

	Detect_StatusReg_58();

 /*读取cf卡的一个扇区*/ 
	CF_SecRead_cnt = CF_SecByte_num;		
   for(i=CF_SecRead_cnt;i>0;i--)
   {
   	
	CF_Rsector_buffer[CF_SecByte_num-i] = *(unsigned char *)(EMA_CE2+CF_DataReg);

   }
   Detect_StatusReg_50();
 
}

/****************************************************************************
*void CF_ReadSector(unsigned int sector_number)                             *
*						DSP读CF卡扇区函数                                   *
*				                                                            *
*				                                                    		* 
*****************************************************************************/
void CF_ReadSector(unsigned int sector_number)
{
	int i = 0;
	unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	unsigned short CF_SecRead_cnt = 0; 
	
	ConvertIntToCharArray(sector_number, CF_LBAAddr_Buffers);	

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //要读硬盘的扇区个数

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16数

	*(unsigned char *)(EMA_CE2+CF_CylindersLReg) = CF_LBAAddr_Buffers[1]; //write Cylinder Low Register or LBA15:8

	*(unsigned char *)(EMA_CE2+CF_SectorsNumReg) = CF_LBAAddr_Buffers[0]; //write Sector Number Register or LBA7:0

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0x21; //write Command Register

	Detect_StatusReg_58();

 /*读取cf卡的一个扇*/ 
	CF_SecRead_cnt = CF_SecByte_num;		
   for(i=CF_SecRead_cnt;i>0;i--)
   {
   	
	CF_Rsector_buffer[CF_SecByte_num-i] = *(unsigned char *)(EMA_CE2+CF_DataReg);

   }
   Detect_StatusReg_50();
 
}

/****************************************************************************
*void CF_Addr_information()                                                 *
*						                                                    *
*	作用：通过读取CF卡DBR扇区的信息，获得CF卡地址的基本信息                 *
*					                                                        * 
*****************************************************************************/
void CF_Addr_information(void)
{

 /*计算CF卡保留扇区数*/
	DBRResrSec = CF_Rsector_buffer[0x0F];
	DBRResrSec = (DBRResrSec<<8) + CF_Rsector_buffer[0x0E];

 /*计算CF卡分区的总扇区数*/
	SecPerPart = ConvertCharArrayToInt(&CF_Rsector_buffer[0x23]);

/*计算CF潜标号扇区数*/
//	QB_Num_LBAadd = MBRAdd+SecPerPart-2;//前标号在CF卡最后几个扇区
    QB_Num_LBAadd = DBRAdd+SecPerPart-2;//前标号在CF卡最后几个扇区,总扇区数不包括MBR区

 /*计算CF卡每个FAT表扇区数*/
	SecPerFAT = ConvertCharArrayToInt(&CF_Rsector_buffer[0x27]);

 /*计算CF卡根目录DIR起始簇号*/
	RootClus = ConvertCharArrayToInt(&CF_Rsector_buffer[0x2F]);

 /*计算FAT1地址,FAT1起始扇区地址=起始扇区地址+保留扇区数*/
	FAT1Add = DBRAdd + DBRResrSec ;

 /*计算FAT1地址,FAT1起始扇区地址=起始扇区地址+保留扇区数*/
	FAT2Add = FAT1Add + SecPerFAT ;

 /*计算DIR地址;DIR起始扇区地址= FAT2起始扇区地址+FAT表扇区数*/
	DIRAdd = FAT2Add + SecPerFAT ;

 /*计算数据区地址;Data起始扇区地址= DIR起始扇区地址+DIR扇区数*/
	DataAdd = DIRAdd + CF_DirSec_num ;
}

/****************************************************************************
*void Read_QB_num(void)                                                     *
*					                                                        *
*					作用：读取系统的前标号                                  *
*****************************************************************************/
void Read_QB_num(void)
{
	CF_ReadSector(QB_Num_LBAadd);
	//QB_Num = ConvertCharArrayToInt(&CF_Rsector_buffer[3]);
	QB_Num = CF_Rsector_buffer[0];
		
}

/****************************************************************************
*void Write_QB_num(void)                                                    *
*					                                                        *
*					作用：向CF卡中写入系统的前标号                          *
*****************************************************************************/
void Write_QB_num(void)
{
	int i;
	for (i=0;i<512;i++)
	   {
	  
	    CF_Wsector_buffer[i] =0;
		
	   }
	CF_Wsector_buffer[0]= '1';
	CF_Wsector_buffer[1]= '0';
	CF_Wsector_buffer[2]= '0';
	CF_Wsector_buffer[3]= '0';

	CF_WriteSector(QB_Num_LBAadd);
		
}

/****************************************************************************
*void CF_WriteSector(unsigned int sector_number)                            *
*						DSP写CF卡扇区函数                                   *
*					作用：DSP向CF卡中写入数据                               *
*					参数1：char *pCF_temp 待向CF卡中写入数据的其中地址      *
*					参数2：sector_number 待发向CF卡中写入数据的扇区号		* 
*****************************************************************************/
void CF_WriteSector(unsigned int sector_number)
{
    unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	unsigned short CF_SecWrite_cnt = 0;
	int i;
	
	ConvertIntToCharArray(sector_number, CF_LBAAddr_Buffers);

	Detect_StatusReg_81();

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //要写入CF卡的扇区个数

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16数

	*(unsigned char *)(EMA_CE2+CF_CylindersLReg) = CF_LBAAddr_Buffers[1]; //write Cylinder Low Register or LBA15:8

	*(unsigned char *)(EMA_CE2+CF_SectorsNumReg) = CF_LBAAddr_Buffers[0]; //write Sector Number Register or LBA7:0

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0x31; //write Command Register

	Detect_StatusReg_58();	

	CF_SecWrite_cnt = CF_SecByte_num;		
	for(i=CF_SecWrite_cnt;i>0;i--)
	{
   	
	 *(unsigned char *)(EMA_CE2+CF_DataReg) = CF_Wsector_buffer[CF_SecByte_num-i];

	}
    
	Detect_StatusReg_50();

}

/****************************************************************************
*void CF_WriteSectors(unsigned int sector_number)                            *
*						DSP写CF卡多扇区函数                                   *
*					作用：DSP向CF卡中写入224个扇区的数据                               *
*					参数1：char *pCF_temp 待向CF卡中写入数据的其中地址      *
*					参数2：sector_number 待发向CF卡中写入数据的扇区号		* 
*****************************************************************************/
void CF_WriteSectors(unsigned int sector_number)
{
    unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	int i;
	int j;
	
	ConvertIntToCharArray(sector_number, CF_LBAAddr_Buffers);

	Detect_StatusReg_81();

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0xE0; //要写入CF卡的扇区个数

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16数

	*(unsigned char *)(EMA_CE2+CF_CylindersLReg) = CF_LBAAddr_Buffers[1]; //write Cylinder Low Register or LBA15:8

	*(unsigned char *)(EMA_CE2+CF_SectorsNumReg) = CF_LBAAddr_Buffers[0]; //write Sector Number Register or LBA7:0

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0x31; //write Command Register

		
	for(i=0;i<CF_sectors_num;i++)
	{  
	   Detect_StatusReg_58();
	   for(j=512 * i;j<512 * i+512;j++)
	   { 
	      *(unsigned char *)(EMA_CE2+CF_DataReg) = CF_Wsectors_buffer[j];
	   }
	   CF_Main();
	}		

    
	Detect_StatusReg_50();

}

/****************************************************************************
*unsigned int ConvertCharArrayToInt(unsigned char *array)                    *
*						                                                    *
*	作用：不同数据类型之间的转换（char转int）                               *
*	说明：array	是最高字节的地址			                                * 
*****************************************************************************/
unsigned int ConvertCharArrayToInt(unsigned char *array)
{
	unsigned int temp;
	temp = *array;
	temp = (temp<<8) + *(array-1);
	temp = (temp<<8) + *(array-2); 
	temp = (temp<<8) + *(array-3);
	return temp;
}
/****************************************************************************
*void ConvertIntToCharArray(unsigned int value,unsigned char *array)        *
*						                                                    *
*	作用：不同数据类型之间的转换（int转char）                               *
*	说明：array	是最低字节的地址			                                * 
*****************************************************************************/
void ConvertIntToCharArray(unsigned int value,unsigned char *array)
{
	*array = value & 0xff;
	*(array + 1) = (value>>8) & 0xff;
	*(array + 2) = (value>>16) & 0xff;
	*(array + 3) = (value>>24) & 0xff;

}

/****************************************************************************
*void Detect_StatusReg_81(void)                                             *
*						                                                    *
*	作用：检测状态寄存器，判断CF卡是否已经执行完所发送的命令                *
*	说明：			                                                        * 
*****************************************************************************/
void Detect_StatusReg_81(void)
{
	
	unsigned char StatusReg_Buffer ;
	volatile unsigned char Busy_flag ;
	do{
	Busy_flag = 1;
	StatusReg_Buffer = *(unsigned char *)(EMA_CE2+CF_StatusReg);
	StatusReg_Buffer = StatusReg_Buffer & 0x81;

	if(StatusReg_Buffer == 0)
		Busy_flag = 0;
	else 
		Busy_flag = 1;		

	} while (Busy_flag == 1);
}

/****************************************************************************
*void Detect_StatusReg_58(void)                                             *
*						                                                    *
*	作用：检测状态寄存器，判断CF卡是否已经执行完所发送的命令                *
*	说明：			                                                        * 
*****************************************************************************/
void Detect_StatusReg_58(void)
{
	
	unsigned char StatusReg_Buffer ;
	volatile unsigned char Sector_WR_flag ;
	do{
	Sector_WR_flag = 0;
	StatusReg_Buffer = *(unsigned char *)(EMA_CE2+CF_StatusReg);

	if(StatusReg_Buffer == 0x58)
		Sector_WR_flag = 1;
	else 
		Sector_WR_flag = 0;		
	} while (Sector_WR_flag == 0); 
}

/****************************************************************************
*void Detect_StatusReg_50(void)                                             *
*						                                                    *
*	作用：检测状态寄存器，判断CF卡是否已经执行完所发送的命令                *
*	说明：			                                                        * 
*****************************************************************************/
void Detect_StatusReg_50(void)
{
	
	unsigned char StatusReg_Buffer ;
	volatile unsigned char Sector_WRok_flag ;
	do{
	Sector_WRok_flag = 0;
	StatusReg_Buffer = *(unsigned char *)(EMA_CE2+CF_StatusReg);

	if(StatusReg_Buffer == 0x50)
		Sector_WRok_flag = 1;
	else 
		Sector_WRok_flag = 0;		
	} while (Sector_WRok_flag == 0); 
}

/****************************************************************************
*void Clear_Dir(void)                                                       *
*						                                                    *
*	作用：清除根目录区                                                      *
*	说明：根目录区共占16个扇区			                                    * 
*****************************************************************************/

void Clear_Dir(void)
{
	int i = 0;
	unsigned int j = 0;
	
	for (i = CF_SecByte_num;i>0;i--)
	{
			
		CF_Wsector_buffer[CF_SecByte_num - i] = 0;

	}

	for (j = 0;j<CF_DirSec_num;j++)
	{
	
		CF_WriteSector(DIRAdd+j);
	}

}
/****************************************************************************
*void Form_FAT1(void)                                                       *
*						                                                    *
*	作用：生成FAT1表，                                                      *
*	说明：每个文件占FAT1表128个扇区			                                * 
*****************************************************************************/
void Form_FAT1(void)
{
	unsigned int FAT_Sector_num ;
	unsigned int FAT_Sector_index = 0 ;
	unsigned int FAT_FileEnd_num =0x0FFFFFFF ;//文件结束簇号
	unsigned int FAT_FirstSector0 =0x0FFFFFF8 ;
	unsigned int FAT_FirstSector1 =0x0FFFFFFF ;
	unsigned int FAT_FirstSector2 =0x00000003 ;
	unsigned int FAT_FirstSector3 =0x0FFFFFFF ;
	unsigned int WBuffer_index = 0x10 ;
	unsigned int i ;
	unsigned int k ;
    
	ConvertIntToCharArray(FAT_FirstSector0, &CF_Wsector_buffer[0x0]);
	ConvertIntToCharArray(FAT_FirstSector1, &CF_Wsector_buffer[0x4]);
	ConvertIntToCharArray(FAT_FirstSector2, &CF_Wsector_buffer[0x8]);
	ConvertIntToCharArray(FAT_FirstSector3, &CF_Wsector_buffer[0xC]);
    
    for(FAT_Sector_num=5;FAT_Sector_num<129;FAT_Sector_num++)//写FAT表的第一个扇区
	{
		ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
		WBuffer_index = WBuffer_index+sizeof(unsigned int);
	}

	CF_WriteSector(FAT1Add+FAT_Sector_index);
	FAT_Sector_index++;

	for(i=0;i<CF_File_num;i++) //共270个文件
	{
		unsigned int j;
		for(j=0;j<File_FATSec_num;j++)//每个文件55MB
		{
			WBuffer_index = 0;
			for(;FAT_Sector_num<(129+FAT_Sector_index*128);FAT_Sector_num++)//每个扇区512/4=128个FAT表
			{
				ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
				WBuffer_index = WBuffer_index+sizeof(unsigned int);

			}
			
			if(j==(File_FATSec_num-1))//每个文件的最后一个扇区写入文件结束簇号
			{
				ConvertIntToCharArray(FAT_FileEnd_num, &CF_Wsector_buffer[0xC]);
				CF_WriteSector(FAT1Add+FAT_Sector_index);
				FAT_Sector_index++;	
			}
		   
		   	else
			{
				CF_WriteSector(FAT1Add+FAT_Sector_index);
				FAT_Sector_index++;	
			}			
							
		}

	}
	
	
	WBuffer_index = 0x10;//改写最后一个FAT表扇区
	for(k=4;k<128;k++)
	{
		ConvertIntToCharArray(0, &CF_Wsector_buffer[WBuffer_index]);
		WBuffer_index = WBuffer_index+sizeof(unsigned int);
	}
	
	CF_WriteSector(FAT1Add+FAT_Sector_index-1);

}

/****************************************************************************
*void Form_FAT(void)                                                       *
*						                                                    *
*	作用：生成FAT1表，                                                      *
*	说明：每个文件占FAT1表128个扇区			                                * 
*****************************************************************************/
void Form_FAT(void)
{
	unsigned int FAT_Sector_num ;
	unsigned int FAT_Sector_index = 0 ;
	unsigned int FAT_FileEnd_num =0x0FFFFFFF ;//文件结束簇号
	unsigned int FAT_FirstSector0 =0x0FFFFFF8 ;
	unsigned int FAT_FirstSector1 =0x0FFFFFFF ;
	unsigned int FAT_FirstSector2 =0x00000003 ;
	unsigned int FAT_FirstSector3 =0x0FFFFFFF ;
	unsigned int WBuffer_index = 0x10 ;
	unsigned int k ;
    
	ConvertIntToCharArray(FAT_FirstSector0, &CF_Wsector_buffer[0x0]);
	ConvertIntToCharArray(FAT_FirstSector1, &CF_Wsector_buffer[0x4]);
	ConvertIntToCharArray(FAT_FirstSector2, &CF_Wsector_buffer[0x8]);
	ConvertIntToCharArray(FAT_FirstSector3, &CF_Wsector_buffer[0xC]);
    
    for(FAT_Sector_num=5;FAT_Sector_num<129;FAT_Sector_num++)//写FAT表的第一个扇区
	{
		ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
		WBuffer_index = WBuffer_index+sizeof(unsigned int);
	}

	CF_WriteSector(FAT1Add+FAT_Sector_index);
	FAT_Sector_index++;

	for(;FAT_Sector_index<=FAT_sectors_num;FAT_Sector_index++)
	{
		WBuffer_index = 0;
        for(;FAT_Sector_num<(129+FAT_Sector_index*128);FAT_Sector_num++)//每个扇区512/4=128个FAT表
		{
			if((FAT_Sector_num%1760)==4)
               ConvertIntToCharArray(FAT_FileEnd_num, &CF_Wsector_buffer[WBuffer_index]);
			else  
			   ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
			WBuffer_index = WBuffer_index+sizeof(unsigned int);
			
		}
		if(FAT_Sector_index==FAT_sectors_num)
        {
		    WBuffer_index = 0x10;//改写最后一个FAT表扇区
	        for(k=4;k<128;k++)
	        {
	        	ConvertIntToCharArray(0, &CF_Wsector_buffer[WBuffer_index]);
		        WBuffer_index = WBuffer_index+sizeof(unsigned int);
        	}
        }
		CF_WriteSector(FAT1Add+FAT_Sector_index);
	}
/*	for(i=0;i<CF_File_num;i++) //共270个文件
	{
		unsigned int j;
		for(j=0;j<File_FATSec_num;j++)//每个文件7MB
		{
			WBuffer_index = 0;
			for(;FAT_Sector_num<(129+FAT_Sector_index*128);FAT_Sector_num++)//每个扇区512/4=128个FAT表
			{
				ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
				WBuffer_index = WBuffer_index+sizeof(unsigned int);

			}
			
			if(j==(File_FATSec_num-1))//每个文件的最后一个扇区写入文件结束簇号
			{
				ConvertIntToCharArray(FAT_FileEnd_num, &CF_Wsector_buffer[0xC]);
				CF_WriteSector(FAT1Add+FAT_Sector_index);
				FAT_Sector_index++;	
			}
		   
		   	else
			{
				CF_WriteSector(FAT1Add+FAT_Sector_index);
				FAT_Sector_index++;	
			}			
							
		}

	}*/
	
	
/*	WBuffer_index = 0x10;//改写最后一个FAT表扇区
	for(k=4;k<128;k++)
	{
		ConvertIntToCharArray(0, &CF_Wsector_buffer[WBuffer_index]);
		WBuffer_index = WBuffer_index+sizeof(unsigned int);
	}
	
	CF_WriteSector(FAT1Add+FAT_Sector_index-1);
*/
}


void File_num_test()
{   
  int i=1;
  int j=0;
  int k=0;
  CF_ReadSector(DIRAdd);//读取DIR数据
  while(i)
  {  
if(CF_Rsector_buffer[j]!=0x51)
{   i=0;    }
	   
else
{   file_count++;
    j+=0x20;
	  if(j==512) //检测是否为该DIR扇区最后一文件名
	  {  j=0;
		  k++;
		  if(k<80)
          {  CF_ReadSector(DIRAdd+k);  }
          else 
		  {  file_count = 0;  //如果DIR32个扇区已存满，则从数据区起始位置开始存数
		     i=0;
		  }
	 }
}
  }  
}


