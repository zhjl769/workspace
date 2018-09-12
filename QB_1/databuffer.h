/*****************************************************************************/
/*  databuffer.H v6.0.8                                                       */
/*                                                                            */
/******************************************************************************/

#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#define ACQ_BUFFER_START_POINTER 0x000A0400//���ݻ������ʼ��ַ
#define ACQ_BUFFER_CELL_SIZE 4000 //���浥Ԫ��С
#define ACQ_BUFFER_NUMS 20	//���10�����浥Ԫ

#define GPS_BUFFER_START_POINTER 0x000B3C80
#define GPS_BUFFER_CELL_SIZE	8000
#define GPS_BUFFER_NUMS 2
#define HEADER_OFFSET 16
#define CENTER_POINTER 3992

#define PRESSURE_BUFFER_START_POINTER 0x000B7C80
#define PRESSURE_BUFFER_CELL_SIZE 1050
#define PRESSURE_BUFFER_NUMS 2

typedef struct _DATA_BUFFER
{
	unsigned int ReadIndex;		 //���浥Ԫ������
	unsigned int WriteIndex;	 //���浥Ԫд����
	unsigned int WritedNums;//���浥Ԫռ����Ŀ
	unsigned int BufferCellSize;
	unsigned int MaxCellNums;
	char *BufferPointer;	//������ʼ��ַ
}DATA_BUFFER;

//���ݻ����ʼ��
void DataBuffer_Initial(DATA_BUFFER *pdataBuffer,char* bufferPointer,unsigned int bufferCellSize,unsigned int maxCellNums);
//���ݻ���д������1
void DataBuffer_WriteIndexInc(DATA_BUFFER *pdataBuffer);
//���ݻ����������1
void DataBuffer_ReadIndexInc(DATA_BUFFER *pdataBuffer);  

//���ݻ�����,���û��浥Ԫ����Ŀ��1
#define DataBuffer_WritedNumsInc(pdataBuffer) (pdataBuffer)->WritedNums++
//�ǶȻ�����,���û��浥Ԫ����Ŀ��1
#define DataBuffer_WritedNumsDec(pdataBuffer) (pdataBuffer)->WritedNums--
//�������ݻ����л��浥Ԫ��ʹ�õ���Ŀ
void DataBuffer_SetUsedCellNum(DATA_BUFFER *pdataBuffer,unsigned int in_int); 
//������ݻ����л��浥Ԫ��ʹ�õ���Ŀ
#define DataBuffer_GetWritedNums(pdataBuffer) (pdataBuffer)->WritedNums

char* DataBuffer_GetWritePointer(DATA_BUFFER *pdataBuffer);

char* DataBuffer_GetReadPointer(DATA_BUFFER *pdataBuffer);

#endif
