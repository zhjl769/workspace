#ifndef _FAT16_H_
#define _FAT16_H_

#include "..\predef.h"
#pragma pack(1)

typedef struct
{
    uint8 BS_jmpBoot[3];
    uint8 BS_OEMName[8];
    uint16 BPB_BytesPerSec;
    uint8 BPB_SecPerClus;
    uint16 BPB_RsvdSecCnt;
    uint8 BPB_NumFATs;
    uint16 BPB_RootEntCnt;
    uint16 BPB_TotSec16;
    uint8 BPB_Media;
    uint16 BPB_FATSz16;
    uint16 BPB_SecPerTrk;
    uint16 BPB_NumHeads;
    uint32 BPB_HiddSec;
    uint32 BPB_TotSec32;
    uint8 BS_DrvNum;
    uint8 BS_Reservedl;
    uint8 BS_BootSig;
    uint32 BS_VolID;
    uint8 BS_VolLab[11];
    uint8 BS_FilSysType[8];
    uint8 ExecutableCode[448];
    uint8 ExecutableMarker[2];
} FAT_BPB;
typedef struct
{
    uint8 NAME[8];
    uint8 TYPE[3];
} FILE_NAME;
typedef struct
{
    uint16 Start;
    uint32 Size;
} FILE_POSIT;
typedef struct
{
    FILE_NAME FileName;
    uint8 FileAttrib;
    uint8 UnUsed[10];
    uint8 FileUpdateTime[2];
    uint8 FileUpdateData[2];
    FILE_POSIT FilePosit;
} myDIR;
typedef struct
{
    uint16 ClusID;
    uint16 SecOfClus;
    uint16 ByteOfSec;
} DATA_POSIT;

#pragma pack()

//********************************************************************************************
//读一个扇区
void ReadBlock(uint32 LBA);
//********************************************************************************************
//写一个扇区
void WriteBlock(uint32 LBA);
//********************************************************************************************
void CopyBytes(void* S, void* D, uint16 size);
//********************************************************************************************
uint8 IsEqual(void* A, void* B, uint8 Size);
//********************************************************************************************
void EmptyBytes(void* D, uint16 size);
//********************************************************************************************
//写BPB数据结构
void FATInit(void);
//********************************************************************************************
//读取BPB数据结构
void ReadBPB(void);
//********************************************************************************************
//获取FAT表开始扇区号
uint32 FATStartSec(void);
//********************************************************************************************
//获取根目录开始扇区号
uint32 DirStartSec(void);
//********************************************************************************************
//获取数据区开始扇区号
uint32 DataStartSec(void);
//********************************************************************************************
//目录项占用的扇区数
uint16 GetDirSecCount(void);
//********************************************************************************************
//获取一个簇的开始扇区
uint32 ClusConvLBA(uint16 ClusID);
//********************************************************************************************
//读取文件分配表的指定项
uint16 ReadFAT(uint16 Index);
//********************************************************************************************
//写文件分配表的指定项
void WriteFAT(uint16 Index, uint16 Value);
//********************************************************************************************
//写备份文件分配表
void WriteFAT2(uint16 Index, uint16 Value);
//********************************************************************************************
//获取根目录中可以使用的一项
uint16 GetEmptyDIR(void);
//********************************************************************************************
//获得和文件名对应的目录项
uint8 GetFileID(uint8 Name[11], myDIR* ID, uint16* Index);
//********************************************************************************************
//获取一个空的FAT项
uint16 GetNextFAT(void);
//********************************************************************************************
//读取根目录的指定项
void ReadDIR(uint16 Index, myDIR* Value);
//********************************************************************************************
//写根目录的指定项
void WriteDIR(uint16 Index, myDIR* Value);
//********************************************************************************************
//创建一个空文件
void CreateFile(uint8 FileName[11]);
//********************************************************************************************
//复制文件分配表,使其和备份一致
void CopyFAT(void);
//********************************************************************************************
//操作文件的数据
uint8 OperateFile(uint8 Write, uint8 Name[11], uint32 Start, uint32 Length,
        void* Data);
//********************************************************************************************
//删除文件
uint8 DelFile(uint8 Name[11]);
//********************************************************************************************
//格式化
void FormatCard(void);
//********************************************************************************************

#endif
//---------------------------------------------------------------------------------------------------------------------------------
