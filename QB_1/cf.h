
/*******************************************************************************
* FILENAME
*   cf.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/
#ifndef CF_H 
#define CF_H 
#include "c6x.h"
#include "c6xdsp.h"

void Read_QB_num(void);
void Write_QB_num(void);
void CF_ReadSector(unsigned int sector_number);
void CF_Addr_information(void);
void CF_WriteSector(unsigned int sector_number);
void CF_WriteSectors(unsigned int sector_number);
unsigned int ConvertCharArrayToInt(unsigned char *array);
void ConvertIntToCharArray(unsigned int value,unsigned char *array);
void Detect_StatusReg_81(void);
void Detect_StatusReg_58(void);
void Detect_StatusReg_50(void);
void Clear_Dir(void);
void Form_FAT(void);
void Form_FAT1(void);
void Init_CF(void);
void Write_File_dir(unsigned int File_num);
void Read_MBR_sector(void);

#endif

void File_num_test();
