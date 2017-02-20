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
//��һ������
void ReadBlock(uint32 LBA)
//********************************************************************************************
{
    mmcReadSector(LBA, BUFFER);
    return;
}

//********************************************************************************************
//дһ������
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
//дBPB���ݽṹ
void FATInit(void)
//********************************************************************************************
{
    FAT_BPB*BPB = (FAT_BPB*) BUFFER;
    //����
    BPB->BPB_BytesPerSec = 0x0200;
    //ÿ������512�ֽ�
    BPB->BPB_SecPerClus = 0x08;
    //ÿ����8������
    BPB->BPB_RsvdSecCnt = 0x0004;
    //һ����������
    BPB->BPB_NumFATs = 0x02;
    //2��FAT��
    BPB->BPB_RootEntCnt = 0x0200;
    //512Ŀ¼�����
    BPB->BPB_TotSec16 = 0x0000;
    //��������
    BPB->BPB_FATSz16 = 0xF2;
    //һ��FAT����ռ��������
    BPB->BPB_HiddSec = 0x00000000;
    //��FAT��ǰ���ص�������

    WriteBlock(0);
    //д������
}
//********************************************************************************************
//��ȡBPB���ݽṹ
void ReadBPB(void)
//********************************************************************************************
{
    FAT_BPB*BPB = (FAT_BPB*) BUFFER;
    ReadBlock(0);

    //��ȡ����
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
//��ȡFAT��ʼ������
uint32 FATStartSec(void)
//********************************************************************************************
{
    return BPB_RsvdSecCnt;
}
//********************************************************************************************
//��ȡ��Ŀ¼��ʼ������
uint32 DirStartSec(void)
//********************************************************************************************
{
    return BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz16;
}

//********************************************************************************************
//��ȡ��������ʼ������
uint32 DataStartSec(void)
//********************************************************************************************
{
    return (uint32) (DirStartSec() + BPB_RootEntCnt * 32 / BPB_BytesPerSec);
}

//********************************************************************************************
//Ŀ¼��ռ�õ�������
uint16 GetDirSecCount(void)
//********************************************************************************************
{
    return BPB_RootEntCnt * 32 / 512;
}

//********************************************************************************************
//��ȡһ���صĿ�ʼ����
uint32 ClusConvLBA(uint16 ClusID)
//********************************************************************************************
{
    return DataStartSec() + BPB_SecPerClus * (ClusID - 2);
}

//********************************************************************************************
//��ȡ�ļ�������ָ����
uint16 ReadFAT(uint16 Index)
//********************************************************************************************
{
    uint16*RAM = (uint16*) BUFFER;
    ReadBlock(BPB_RsvdSecCnt + Index / 256);
    return RAM[Index % 256];
}

//********************************************************************************************
//д�ļ�������ָ����
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
//дFAT2
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
//��ȡ��Ŀ¼�п���ʹ�õ�һ��
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
//��ú��ļ�����Ӧ��Ŀ¼���Լ���Ŀ¼��λ��
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
                //�ҵ���Ӧ��Ŀ¼��,����Ŀ¼��λ��.
            }
        }
    }
    return 0;
    //û���ҵ���Ӧ��Ŀ¼��,����0.
}

//********************************************************************************************
//��ȡһ���յ�FAT��
uint16 GetNextFAT(void)
//********************************************************************************************
{
    uint16 FAT_Count, i;
    FAT_Count = BPB_FATSz16 * 256;
    //FAT��������
    for (i = 0; i < FAT_Count; i++)
    {
        if (ReadFAT(i) == 0)
            return i;
    }
    return 0;
}

//********************************************************************************************
//��ȡ��Ŀ¼��ָ����
void ReadDIR(uint16 Index, myDIR*Value)
//********************************************************************************************
{
    uint32 DirStart = DirStartSec();
    ReadBlock(DirStart + Index / 16);
    CopyBytes(&BUFFER[(Index % 16) * 32], Value, 32);
}

//********************************************************************************************
//д��Ŀ¼��ָ����
void WriteDIR(uint16 Index, myDIR*Value)
//********************************************************************************************
{
    uint32 LBA = DirStartSec() + Index / 16;
    ReadBlock(LBA);
    CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
    WriteBlock(LBA);
}
//********************************************************************************************
//����һ�����ļ�
//˵��:FileName     �ļ������������ֻ��ߴ�д��ĸ����չ��Ҳ�����
//�ļ���С���Ȳ�������,��Ϊ0;
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
//ɾ���ļ�
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
//�����ļ������,ʹ��ͱ���һ��
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
//�����ļ�������
//˵����Write       1ʱ��д����,0Ϊ������
//      Name        �ļ���֧�����ֺʹ�д��ĸ
//      Start       ��ʼ�ֽ�
//      Length      ��������
//      Data        ��������
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
    // ÿ�ص��ֽ���
    if (!GetFileID(Name, &FileDir, &Index))
        return 0;
    length = Length;

    //���㿪ʼλ�����ڴصĴغ�
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

    //���㿪ʼλ��������������ƫ��
    i = (Start % BytePerClus) / 512;

    //���㿪ʼλ��������ƫ��
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

            //�����ȡ��ɾ��˳�
            if (--Length == 0)
            {
                if (Write)
                {
                    //��д����
                    WriteBlock(LBA);
                    //��дĿ¼
                    FileDir.FilePosit.Size = Start + length;
                    WriteDIR(Index, &FileDir);
                }
                return 1;
            }
        }
        if (Write)
            WriteBlock(LBA++);
        //��д����,ָ������
    }
    return 1;
}
//}
//********************************************************************************************
//��ʽ��
//�������������ϵĸ�ʽ����ֻ�������Ŀ¼�Լ�FAT��
void FormatCard(void)
//********************************************************************************************
{
    uint16 i, DirStart, DirSecNum, FatStart, FatSecNum;

    ReadBPB();
    //�����Ŀ¼
    for (i = 0; i <= 512; i++)
        BUFFER[i] = 0;
    DirSecNum = GetDirSecCount();
    DirStart = DirStartSec();
    for (i = 0; i < DirSecNum; i++)
    {
        WriteBlock(DirStart + i);
    }

    //���FAT��
    FatStart = BPB_RsvdSecCnt;
    FatSecNum = BPB_FATSz16;
    for (i = 0; i < FatSecNum; i++)
    {
        WriteBlock(FatStart + i);
    }
    WriteFAT(0, 0xf8ff);
    WriteFAT(1, 0xffff);

    //����FAT��
    CopyFAT();
}

//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------
