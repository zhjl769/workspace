
/*******************************************************************************
* FILENAME
*   cf.c
*
* DESCRIPTION
*   ��Ҫ���CF���Ķ�д��FAT�ļ�ϵͳ�Ľ���
*	ÿ���ļ�50MB���ܹ�270���ļ�
*******************************************************************************/
#include "cf.h"
#define CF_SecByte_num 512 /*ÿ�����ֽ���*/
#define CF_DirSec_num 128 /*ΪCF����Ŀ¼��Ԥ��������*/
#define Clu_Sec_num 16 /*ÿ����Ŀ¼����16���ļ�*/
#define File_Dir_num 32 /*ÿ���ļ�DIRռ��32�ֽ�*/
#define CF_File_num 270  /*CF��д�ļ���*/
#define File_FATSec_num 55 //ÿ���ļ��Ĵ�С����λMB
#define CF_sectors_num 224  //ÿ��д16*14=224��������
#define CF_Write_Bytes 114688 //ÿ��д224*512=114688�ֽ���
#define FAT_sectors_num 15125 //15125*4/55=1100���ļ�
unsigned char CF_Wsector_buffer[CF_SecByte_num];
unsigned char CF_Wsectors_buffer[CF_Write_Bytes];
unsigned char CF_Rsector_buffer[CF_SecByte_num];
unsigned char Dir_buffer[File_Dir_num];

/*MBR��DBR�ĵ�ַ��ʵ��һ��*/
unsigned int MBRAdd = 0xE0000000;
unsigned int MBRResrSec = 0;
unsigned int DBRAdd = 0;
unsigned short DBRResrSec = 0;     /*CF��������������*/
unsigned int SecPerPart = 0;       /*CF������������*/
unsigned int SecPerFAT = 0;        /*CF��ÿ��FAT�����������*/
unsigned int RootClus = 0;         /*CF����Ŀ¼DIR��ʼ�غ�*/
unsigned int FAT1Add = 0;
unsigned int FAT2Add = 0;
unsigned int DIRAdd = 0;
unsigned int DataAdd = 0;
unsigned char QB_Num = 0;
unsigned int QB_Num_LBAadd = 0xE1DD40AF;

int file_count=0;   // ��ǰ�ļ��ţ�

/****************************************************************************
*void Init_CF(void)                                                         *
*						                                                    *
*	���ã���ʼ��CF��,������ȡMBR��Ϣ���γ�FAT��� ��ʼ�����֮��Ϳ���д�ļ�*
*          			                                                        * 
*****************************************************************************/
void Init_CF(void)
{
	Read_MBR_sector();
	MBRResrSec = ConvertCharArrayToInt(&CF_Rsector_buffer[0x1C9]);
		
	if(CF_Rsector_buffer[0]==0xEB)//�ж�CF��MBR���Ƿ���
	{
		DBRAdd = MBRAdd;	
	}
	else
	{
		DBRAdd = MBRResrSec + MBRAdd;
	}

	CF_ReadSector(DBRAdd);//��ȡDBR����
	
	CF_Addr_information();//���CF����ַ��Ϣ

	//�¿�Ҫ����������̣���ʽ���ļ�Ŀ¼��-------------------------------------------------------------------------------
	//Clear_Dir();
//	Form_FAT1();//�γ�FAT1��
    Form_FAT();
	//�ɿ�֮ǰ��2���������ξͺ�--------------------------------------------------------------------------------------------

	File_num_test();  // ���Ŀ¼�Ƿ��о��ļ���

//  дǰ���,����һ���¿�ֻ��дһ��
//	Write_QB_num();  
//
	Read_QB_num();//��ȡǱ���
	
}

/****************************************************************************
*void Write_File_dir(void)                                                  *
*	���ã�д�ļ��ĸ�Ŀ¼��				                                    *
*  	˵���� ʵ��File_numΪ�γɵ��ļ���ţ���0��ʼ                            *
*****************************************************************************/
void Write_File_dir(unsigned int File_num)
{
	int i;
	int j;
	int k;
	unsigned int temp;
	unsigned int Dir_Sector_index;//��������
	unsigned int Dir_Block_index; //������

	Dir_Sector_index = File_num / Clu_Sec_num;
	Dir_Block_index = File_num % Clu_Sec_num;

	for(i=0;i<File_Dir_num;i++)
	{
		Dir_buffer[i]=0 ;
	}

	CF_ReadSector(DIRAdd + Dir_Sector_index);//��ȡDIR��������
	
	for(k=0;k<CF_SecByte_num;k++)
	{
		CF_Wsector_buffer[k] = CF_Rsector_buffer[k];
	}
	
	temp = File_num;

	/*д��Ŀ¼������*/
	Dir_buffer[0]='Q';//�ļ�����
	Dir_buffer[1]='B';
	Dir_buffer[2]=QB_Num;
	Dir_buffer[3]='_';
	
	Dir_buffer[4]=(temp / 1000) + 0x30;
	Dir_buffer[5]=(temp / 100) % 10 + 0x30;
	Dir_buffer[6]=(temp / 10) % 10 + 0x30;
	Dir_buffer[7]=(temp % 10) + 0x30;

	Dir_buffer[8]='D';//�ļ���չ��
	Dir_buffer[9]='A';
	Dir_buffer[10]='T';
	Dir_buffer[11]=0x20;//�ļ�����
	
//	temp = File_num * 0x1B80+0x04;//0x80*N ΪN MB���ļ�
    temp = File_num * 0x6E0+0x04;//64G���޸�
	Dir_buffer[20]=(temp>>16) & 0xff;//�ļ���ʼ�ر�ŵĸ�16λ
	Dir_buffer[21]=(temp>>24) & 0xff;
	Dir_buffer[26]= temp & 0xff;; //�ļ���ʼ�ر�ŵĵ�16λ
	Dir_buffer[27]=(temp>>8) & 0xff;
	
	Dir_buffer[28]=0x00;//ÿ���ļ��ֽڳ��� 1MB
	Dir_buffer[29]=0x00;//�ļ�����
	Dir_buffer[30]=0x70;//0x10*N ΪN MB���ļ�
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
*	���ã���ȡMBR��Ϣ                                                       *
*          			                                                        * 
*****************************************************************************/
void Read_MBR_sector(void)
{
	unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	unsigned short CF_SecRead_cnt = 0;
	int i;
	
 /*�趨���ݼĴ���λ��*/
	*(unsigned char *)(EMA_CE2+CF_FeaturesReg) = 0x01; //ʹ��8bit���ݴ���

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //PIOĬ��ģʽ

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = 0xE0; //write Drive Head Register and LBA27:24
	
	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0xEF; //write Command Register

	Detect_StatusReg_81();

 /*�趨PIOģʽ���ݴ���*/
	*(unsigned char *)(EMA_CE2+CF_FeaturesReg) = 0x03; //Set Transfer Mode based on value and Sector Count register.

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x0A; //PIO Flow Control Transfer Mode 2 

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = 0xE0; //write Drive Head Register and LBA27:24

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0xEF; //write Command Register

	Detect_StatusReg_81();	
 
 
 /*��ȡCF��MBR����*/
	ConvertIntToCharArray(MBRAdd, CF_LBAAddr_Buffers);

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //Ҫ��Ӳ�̵���������

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16��

	*(unsigned char *)(EMA_CE2+CF_CylindersLReg) = CF_LBAAddr_Buffers[1]; //write Cylinder Low Register or LBA15:8

	*(unsigned char *)(EMA_CE2+CF_SectorsNumReg) = CF_LBAAddr_Buffers[0]; //write Sector Number Register or LBA7:0

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0x21; //write Command Register

	Detect_StatusReg_58();

 /*��ȡcf����һ������*/ 
	CF_SecRead_cnt = CF_SecByte_num;		
   for(i=CF_SecRead_cnt;i>0;i--)
   {
   	
	CF_Rsector_buffer[CF_SecByte_num-i] = *(unsigned char *)(EMA_CE2+CF_DataReg);

   }
   Detect_StatusReg_50();
 
}

/****************************************************************************
*void CF_ReadSector(unsigned int sector_number)                             *
*						DSP��CF����������                                   *
*				                                                            *
*				                                                    		* 
*****************************************************************************/
void CF_ReadSector(unsigned int sector_number)
{
	int i = 0;
	unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	unsigned short CF_SecRead_cnt = 0; 
	
	ConvertIntToCharArray(sector_number, CF_LBAAddr_Buffers);	

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //Ҫ��Ӳ�̵���������

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16��

	*(unsigned char *)(EMA_CE2+CF_CylindersLReg) = CF_LBAAddr_Buffers[1]; //write Cylinder Low Register or LBA15:8

	*(unsigned char *)(EMA_CE2+CF_SectorsNumReg) = CF_LBAAddr_Buffers[0]; //write Sector Number Register or LBA7:0

	*(unsigned char *)(EMA_CE2+CF_CommandReg) = 0x21; //write Command Register

	Detect_StatusReg_58();

 /*��ȡcf����һ����*/ 
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
*	���ã�ͨ����ȡCF��DBR��������Ϣ�����CF����ַ�Ļ�����Ϣ                 *
*					                                                        * 
*****************************************************************************/
void CF_Addr_information(void)
{

 /*����CF������������*/
	DBRResrSec = CF_Rsector_buffer[0x0F];
	DBRResrSec = (DBRResrSec<<8) + CF_Rsector_buffer[0x0E];

 /*����CF����������������*/
	SecPerPart = ConvertCharArrayToInt(&CF_Rsector_buffer[0x23]);

/*����CFǱ���������*/
//	QB_Num_LBAadd = MBRAdd+SecPerPart-2;//ǰ�����CF����󼸸�����
    QB_Num_LBAadd = DBRAdd+SecPerPart-2;//ǰ�����CF����󼸸�����,��������������MBR��

 /*����CF��ÿ��FAT��������*/
	SecPerFAT = ConvertCharArrayToInt(&CF_Rsector_buffer[0x27]);

 /*����CF����Ŀ¼DIR��ʼ�غ�*/
	RootClus = ConvertCharArrayToInt(&CF_Rsector_buffer[0x2F]);

 /*����FAT1��ַ,FAT1��ʼ������ַ=��ʼ������ַ+����������*/
	FAT1Add = DBRAdd + DBRResrSec ;

 /*����FAT1��ַ,FAT1��ʼ������ַ=��ʼ������ַ+����������*/
	FAT2Add = FAT1Add + SecPerFAT ;

 /*����DIR��ַ;DIR��ʼ������ַ= FAT2��ʼ������ַ+FAT��������*/
	DIRAdd = FAT2Add + SecPerFAT ;

 /*������������ַ;Data��ʼ������ַ= DIR��ʼ������ַ+DIR������*/
	DataAdd = DIRAdd + CF_DirSec_num ;
}

/****************************************************************************
*void Read_QB_num(void)                                                     *
*					                                                        *
*					���ã���ȡϵͳ��ǰ���                                  *
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
*					���ã���CF����д��ϵͳ��ǰ���                          *
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
*						DSPдCF����������                                   *
*					���ã�DSP��CF����д������                               *
*					����1��char *pCF_temp ����CF����д�����ݵ����е�ַ      *
*					����2��sector_number ������CF����д�����ݵ�������		* 
*****************************************************************************/
void CF_WriteSector(unsigned int sector_number)
{
    unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	unsigned short CF_SecWrite_cnt = 0;
	int i;
	
	ConvertIntToCharArray(sector_number, CF_LBAAddr_Buffers);

	Detect_StatusReg_81();

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0x01; //Ҫд��CF������������

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16��

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
*						DSPдCF������������                                   *
*					���ã�DSP��CF����д��224������������                               *
*					����1��char *pCF_temp ����CF����д�����ݵ����е�ַ      *
*					����2��sector_number ������CF����д�����ݵ�������		* 
*****************************************************************************/
void CF_WriteSectors(unsigned int sector_number)
{
    unsigned char CF_LBAAddr_Buffers[4]={0,0,0,0};
	int i;
	int j;
	
	ConvertIntToCharArray(sector_number, CF_LBAAddr_Buffers);

	Detect_StatusReg_81();

	*(unsigned char *)(EMA_CE2+CF_SectorsCouReg) = 0xE0; //Ҫд��CF������������

	*(unsigned char *)(EMA_CE2+CF_HeadsReg) = CF_LBAAddr_Buffers[3]; //write Drive Head Register and LBA27:24	

	*(unsigned char *)(EMA_CE2+CF_CylindersHReg) = CF_LBAAddr_Buffers[2]; //write Cylinder High Register or LBA23:16��

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
*	���ã���ͬ��������֮���ת����charתint��                               *
*	˵����array	������ֽڵĵ�ַ			                                * 
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
*	���ã���ͬ��������֮���ת����intתchar��                               *
*	˵����array	������ֽڵĵ�ַ			                                * 
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
*	���ã����״̬�Ĵ������ж�CF���Ƿ��Ѿ�ִ���������͵�����                *
*	˵����			                                                        * 
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
*	���ã����״̬�Ĵ������ж�CF���Ƿ��Ѿ�ִ���������͵�����                *
*	˵����			                                                        * 
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
*	���ã����״̬�Ĵ������ж�CF���Ƿ��Ѿ�ִ���������͵�����                *
*	˵����			                                                        * 
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
*	���ã������Ŀ¼��                                                      *
*	˵������Ŀ¼����ռ16������			                                    * 
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
*	���ã�����FAT1��                                                      *
*	˵����ÿ���ļ�ռFAT1��128������			                                * 
*****************************************************************************/
void Form_FAT1(void)
{
	unsigned int FAT_Sector_num ;
	unsigned int FAT_Sector_index = 0 ;
	unsigned int FAT_FileEnd_num =0x0FFFFFFF ;//�ļ������غ�
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
    
    for(FAT_Sector_num=5;FAT_Sector_num<129;FAT_Sector_num++)//дFAT��ĵ�һ������
	{
		ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
		WBuffer_index = WBuffer_index+sizeof(unsigned int);
	}

	CF_WriteSector(FAT1Add+FAT_Sector_index);
	FAT_Sector_index++;

	for(i=0;i<CF_File_num;i++) //��270���ļ�
	{
		unsigned int j;
		for(j=0;j<File_FATSec_num;j++)//ÿ���ļ�55MB
		{
			WBuffer_index = 0;
			for(;FAT_Sector_num<(129+FAT_Sector_index*128);FAT_Sector_num++)//ÿ������512/4=128��FAT��
			{
				ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
				WBuffer_index = WBuffer_index+sizeof(unsigned int);

			}
			
			if(j==(File_FATSec_num-1))//ÿ���ļ������һ������д���ļ������غ�
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
	
	
	WBuffer_index = 0x10;//��д���һ��FAT������
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
*	���ã�����FAT1��                                                      *
*	˵����ÿ���ļ�ռFAT1��128������			                                * 
*****************************************************************************/
void Form_FAT(void)
{
	unsigned int FAT_Sector_num ;
	unsigned int FAT_Sector_index = 0 ;
	unsigned int FAT_FileEnd_num =0x0FFFFFFF ;//�ļ������غ�
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
    
    for(FAT_Sector_num=5;FAT_Sector_num<129;FAT_Sector_num++)//дFAT��ĵ�һ������
	{
		ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
		WBuffer_index = WBuffer_index+sizeof(unsigned int);
	}

	CF_WriteSector(FAT1Add+FAT_Sector_index);
	FAT_Sector_index++;

	for(;FAT_Sector_index<=FAT_sectors_num;FAT_Sector_index++)
	{
		WBuffer_index = 0;
        for(;FAT_Sector_num<(129+FAT_Sector_index*128);FAT_Sector_num++)//ÿ������512/4=128��FAT��
		{
			if((FAT_Sector_num%1760)==4)
               ConvertIntToCharArray(FAT_FileEnd_num, &CF_Wsector_buffer[WBuffer_index]);
			else  
			   ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
			WBuffer_index = WBuffer_index+sizeof(unsigned int);
			
		}
		if(FAT_Sector_index==FAT_sectors_num)
        {
		    WBuffer_index = 0x10;//��д���һ��FAT������
	        for(k=4;k<128;k++)
	        {
	        	ConvertIntToCharArray(0, &CF_Wsector_buffer[WBuffer_index]);
		        WBuffer_index = WBuffer_index+sizeof(unsigned int);
        	}
        }
		CF_WriteSector(FAT1Add+FAT_Sector_index);
	}
/*	for(i=0;i<CF_File_num;i++) //��270���ļ�
	{
		unsigned int j;
		for(j=0;j<File_FATSec_num;j++)//ÿ���ļ�7MB
		{
			WBuffer_index = 0;
			for(;FAT_Sector_num<(129+FAT_Sector_index*128);FAT_Sector_num++)//ÿ������512/4=128��FAT��
			{
				ConvertIntToCharArray(FAT_Sector_num, &CF_Wsector_buffer[WBuffer_index]);
				WBuffer_index = WBuffer_index+sizeof(unsigned int);

			}
			
			if(j==(File_FATSec_num-1))//ÿ���ļ������һ������д���ļ������غ�
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
	
	
/*	WBuffer_index = 0x10;//��д���һ��FAT������
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
  CF_ReadSector(DIRAdd);//��ȡDIR����
  while(i)
  {  
if(CF_Rsector_buffer[j]!=0x51)
{   i=0;    }
	   
else
{   file_count++;
    j+=0x20;
	  if(j==512) //����Ƿ�Ϊ��DIR�������һ�ļ���
	  {  j=0;
		  k++;
		  if(k<80)
          {  CF_ReadSector(DIRAdd+k);  }
          else 
		  {  file_count = 0;  //���DIR32�������Ѵ����������������ʼλ�ÿ�ʼ����
		     i=0;
		  }
	 }
}
  }  
}


