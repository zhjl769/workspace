#include "mmc.h"
#include <stdio.h>
#include "FAT16.h"

uint8 BUFFER[512];

uint16 BPB_BytesPerSec;
uint8 BPB_SecPerClus;
uint16 BPB_RsvdSecCnt;
uint8 BPB_NumFATs;
uint16 BPB_RootEntCnt;
uint16 BPB_TotSec16;
uint16 BPB_FATSz16;
uint32 BPB_HiddSec;

//********************************************************************************************
//读一个扇区
void ReadBlock(uint32 LBA)
//********************************************************************************************
{
    mmcReadSector(LBA, BUFFER);
    return;
}

//********************************************************************************************
//写一个扇区
void WriteBlock(uint32 LBA)
//********************************************************************************************
{
    mmcWriteSector(LBA, BUFFER);
    return;
}

//********************************************************************************************
void CopyBytes(void*S, void*D, uint16 size)
//********************************************************************************************
{
    uint8*s = S, *d = D;
    uint16 i;
    for (i = 0; i < size; i++)
        *d++ = *s++;
}

//********************************************************************************************
uint8 IsEqual(void*A, void*B, uint8 Size)
//********************************************************************************************
{
    uint8 i, *a = A, *b = B;
    for (i = 0; i < Size; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

//********************************************************************************************
void EmptyBytes(void*D, uint16 size)
//********************************************************************************************
{
    uint16 i;
    uint8*data = (uint8*) D;
    for (i = 0; i < size; i++)
    {
        *data++ = 0;
    }
}
//********************************************************************************************
//写BPB数据结构
void FATInit(void)
//********************************************************************************************
{
    FAT_BPB*BPB = (FAT_BPB*) BUFFER;
    //参数
    BPB->BPB_BytesPerSec = 0x0200;
    //每个扇区512字节
    BPB->BPB_SecPerClus = 0x08;
    //每个簇8个扇区
    BPB->BPB_RsvdSecCnt = 0x0004;
    //一个保留扇区
    BPB->BPB_NumFATs = 0x02;
    //2个FAT表
    BPB->BPB_RootEntCnt = 0x0200;
    //512目录项个数
    BPB->BPB_TotSec16 = 0x0000;
    //总扇区数
    BPB->BPB_FATSz16 = 0xF2;
    //一个FAT表所占的扇区数
    BPB->BPB_HiddSec = 0x00000000;
    //本FAT表前隐藏的扇区数

    WriteBlock(0);
    //写引导区
}
//********************************************************************************************
//读取BPB数据结构
void ReadBPB(void)
//********************************************************************************************
{
    FAT_BPB*BPB = (FAT_BPB*) BUFFER;
    ReadBlock(0);

    //获取参数
    BPB_BytesPerSec = BPB->BPB_BytesPerSec;
    BPB_SecPerClus = BPB->BPB_SecPerClus;
    BPB_RsvdSecCnt = BPB->BPB_RsvdSecCnt;
    BPB_NumFATs = BPB->BPB_NumFATs;
    BPB_RootEntCnt = BPB->BPB_RootEntCnt;
    BPB_TotSec16 = BPB->BPB_TotSec16;
    BPB_FATSz16 = BPB->BPB_FATSz16;
    BPB_HiddSec = BPB->BPB_HiddSec;

}
//********************************************************************************************
//获取FAT表开始扇区号
uint32 FATStartSec(void)
//********************************************************************************************
{
    return BPB_RsvdSecCnt;
}
//********************************************************************************************
//获取根目录开始扇区号
uint32 DirStartSec(void)
//********************************************************************************************
{
    return BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz16;
}

//********************************************************************************************
//获取数据区开始扇区号
uint32 DataStartSec(void)
//********************************************************************************************
{
    return (uint32) (DirStartSec() + BPB_RootEntCnt * 32 / BPB_BytesPerSec);
}

//********************************************************************************************
//目录项占用的扇区数
uint16 GetDirSecCount(void)
//********************************************************************************************
{
    return BPB_RootEntCnt * 32 / 512;
}

//********************************************************************************************
//获取一个簇的开始扇区
uint32 ClusConvLBA(uint16 ClusID)
//********************************************************************************************
{
    return DataStartSec() + BPB_SecPerClus * (ClusID - 2);
}

//********************************************************************************************
//读取文件分配表的指定项
uint16 ReadFAT(uint16 Index)
//********************************************************************************************
{
    uint16*RAM = (uint16*) BUFFER;
    ReadBlock(BPB_RsvdSecCnt + Index / 256);
    return RAM[Index % 256];
}

//********************************************************************************************
//写文件分配表的指定项
void WriteFAT(uint16 Index, uint16 Value)
//********************************************************************************************
{
    uint16*RAM = (uint16*) BUFFER;
    uint32 SecID;
    SecID = BPB_RsvdSecCnt + Index / 256;
    ReadBlock(SecID);
    RAM[Index % 256] = Value;
    WriteBlock(SecID);
}
//********************************************************************************************
//写FAT2
void WriteFAT2(uint16 Index, uint16 Value)
//********************************************************************************************
{
    uint16*RAM = (uint16*) BUFFER;
    uint32 SecID;
    SecID = BPB_RsvdSecCnt + BPB_FATSz16 + Index / 256;
    ReadBlock(SecID);
    RAM[Index % 256] = Value;
    WriteBlock(SecID);
}
//********************************************************************************************
//获取根目录中可以使用的一项
uint16 GetEmptyDIR()
//********************************************************************************************
{
    uint16 DirSecCut, DirStart, i, m, ID = 0;
    DirSecCut = GetDirSecCount();
    DirStart = DirStartSec();
    for (i = 0; i < DirSecCut; i++)
    {
        ReadBlock(DirStart + i);
        for (m = 0; m < 16; m++)
        {
            if ((BUFFER[m * 32] == 0) || (BUFFER[m * 32] == 0xe5))
                return ID;
            else
                ID++;
        }
    }
    return ID;
}

//********************************************************************************************
//获得和文件名对应的目录项以及该目录的位置
uint8 GetFileID(uint8 Name[11], myDIR*ID, uint16*Index)
//********************************************************************************************
{
    uint16 DirSecCut, DirStart, i, m;
    DirSecCut = GetDirSecCount();
    DirStart = DirStartSec();
    for (i = 0; i < DirSecCut; i++)
    {
        ReadBlock(DirStart + i);
        for (m = 0; m < 16; m++)
        {
            if (IsEqual(Name, &((myDIR*) &BUFFER[m * 32])->FileName, 11))
            {
                *ID = *((myDIR*) &BUFFER[m * 32]);
                *Index = i * 16 + m;
                return 1;
                //找到对应的目录项,返回目录的位置.
            }
        }
    }
    return 0;
    //没有找到对应的目录项,返回0.
}

//********************************************************************************************
//获取一个空的FAT项
uint16 GetNextFAT(void)
//********************************************************************************************
{
    uint16 FAT_Count, i;
    FAT_Count = BPB_FATSz16 * 256;
    //FAT表总项数
    for (i = 0; i < FAT_Count; i++)
    {
        if (ReadFAT(i) == 0)
            return i;
    }
    return 0;
}

//********************************************************************************************
//读取根目录的指定项
void ReadDIR(uint16 Index, myDIR*Value)
//********************************************************************************************
{
    uint32 DirStart = DirStartSec();
    ReadBlock(DirStart + Index / 16);
    CopyBytes(&BUFFER[(Index % 16) * 32], Value, 32);
}

//********************************************************************************************
//写根目录的指定项
void WriteDIR(uint16 Index, myDIR*Value)
//********************************************************************************************
{
    uint32 LBA = DirStartSec() + Index / 16;
    ReadBlock(LBA);
    CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
    WriteBlock(LBA);
}
//********************************************************************************************
//创建一个空文件
//说明:FileName     文件名必须是数字或者大写字母，扩展名也是如此
//文件大小事先不用设置,均为0;
void CreateFile(uint8 FileName[11])
//********************************************************************************************
{
    uint16 ClusID;
    myDIR FileDir;

    //ClusNum = Size / (BPB_SecPerClus * 512) + 1;
    EmptyBytes(&FileDir, sizeof(myDIR));
    CopyBytes(FileName, &FileDir.FileName, 11);

    FileDir.FileAttrib = 0x20;

    FileDir.FilePosit.Size = 0;
    FileDir.FilePosit.Start = GetNextFAT();

    ClusID = FileDir.FilePosit.Start;

    WriteFAT(ClusID, 0xffff);
    WriteFAT2(ClusID, 0xffff);
    WriteDIR(GetEmptyDIR(), &FileDir);
}
//********************************************************************************************
//删除文件
uint8 DelFile(uint8 Name[11])
{
    myDIR FileDir;
    uint16 ClusID, Index;
    if (!GetFileID(Name, &FileDir, &Index))
        return 0;
    FileDir.FileName.NAME[0] = 0xE5;
    ClusID = FileDir.FilePosit.Start;
    WriteFAT(ClusID, 0x0000);
    WriteFAT2(ClusID, 0x0000);
    WriteDIR(Index, &FileDir);
    return 1;
}
//********************************************************************************************
//********************************************************************************************
//复制文件分配表,使其和备份一致
void CopyFAT(void)
//********************************************************************************************
{
    uint16 FATSz16, RsvdSecCnt, i;
    FATSz16 = BPB_FATSz16;
    RsvdSecCnt = BPB_RsvdSecCnt;
    for (i = 0; i < FATSz16; i++)
    {
        ReadBlock(RsvdSecCnt + i);
        WriteBlock(RsvdSecCnt + FATSz16 + i);
    }
}

//********************************************************************************************
//操作文件的数据
//说明：Write       1时是写操作,0为读操作
//      Name        文件名支持数字和大写字母
//      Start       开始字节
//      Length      操作长度
//      Data        操作数据
uint8 OperateFile(uint8 Write, uint8 Name[11], uint32 Start, uint32 Length,
        void*Data)
//********************************************************************************************
{
    uint8*data = Data;
    uint16 BytePerClus, SecPerClus, ClusNum, ClusNext, ClusID, ClusIDValue, m,
            Index;
    uint32 LBA, i, length;
    myDIR FileDir;
    SecPerClus = BPB_SecPerClus;
    BytePerClus = BPB_SecPerClus * 512;
    // 每簇的字节数
    if (!GetFileID(Name, &FileDir, &Index))
        return 0;
    length = Length;

    //计算开始位置所在簇的簇号
    ClusNum = Start / BytePerClus;
    ClusID = FileDir.FilePosit.Start;

    for (i = 0; i < ClusNum; i++)
    {
        ClusIDValue = ReadFAT(ClusID);
        WriteFAT(ClusID, 0xffff);
        WriteFAT2(ClusID, 0xffff);
        if (ClusIDValue == 0xffff)
            ClusNext = GetNextFAT();
        else
            ClusNext = ClusIDValue;

        WriteFAT(ClusID, ClusNext);
        WriteFAT2(ClusID, ClusNext);
        ClusID = ClusNext;
    }
    WriteFAT(ClusID, 0xffff);
    WriteFAT2(ClusID, 0xffff);

    //计算开始位置所在扇区簇内偏移
    i = (Start % BytePerClus) / 512;

    //计算开始位置扇区内偏移
    m = (Start % BytePerClus) % 512;

    LBA = ClusConvLBA(ClusID) + i;

    for (i = 0; i < SecPerClus; i++)
    {
        if (Write)
            ReadBlock(LBA);
        else
            ReadBlock(LBA++);

        for (m = 0; m < 512; m++)
        {

            if (Write)
                BUFFER[m] = *data++;
            else
                *data++ = BUFFER[m];

            //如果读取完成就退出
            if (--Length == 0)
            {
                if (Write)
                {
                    //回写扇区
                    WriteBlock(LBA);
                    //回写目录
                    FileDir.FilePosit.Size = Start + length;
                    WriteDIR(Index, &FileDir);
                }
                return 1;
            }
        }
        if (Write)
            WriteBlock(LBA++);
        //回写扇区,指针下移
    }
    return 1;
}
//}
//********************************************************************************************
//格式化
//不是真正意义上的格式化，只是清除根目录以及FAT表
void FormatCard(void)
//********************************************************************************************
{
    uint16 i, DirStart, DirSecNum, FatStart, FatSecNum;

    ReadBPB();
    //清除根目录
    for (i = 0; i <= 512; i++)
        BUFFER[i] = 0;
    DirSecNum = GetDirSecCount();
    DirStart = DirStartSec();
    for (i = 0; i < DirSecNum; i++)
    {
        WriteBlock(DirStart + i);
    }

    //清除FAT表
    FatStart = BPB_RsvdSecCnt;
    FatSecNum = BPB_FATSz16;
    for (i = 0; i < FatSecNum; i++)
    {
        WriteBlock(FatStart + i);
    }
    WriteFAT(0, 0xf8ff);
    WriteFAT(1, 0xffff);

    //备份FAT表
    CopyFAT();
}

//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------
