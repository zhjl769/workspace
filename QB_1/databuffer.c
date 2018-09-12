/*******************************************************************************
* FILENAME
*  databuffer.c
*
* DESCRIPTION
*   Ϊ���򿪱��ڴ����ݴ洢�ռ�
*
*******************************************************************************/

#include "databuffer.h"

//���ݻ����ʼ��
void DataBuffer_Initial(DATA_BUFFER *pdataBuffer,char* bufferPointer,unsigned int bufferCellSize,unsigned int maxCellNums)
{
	pdataBuffer->ReadIndex = 0;
	pdataBuffer->WriteIndex = 0;
	pdataBuffer->WritedNums = 0;
	pdataBuffer->BufferCellSize = bufferCellSize;
	pdataBuffer->MaxCellNums = maxCellNums;
	pdataBuffer->BufferPointer = bufferPointer;
	memset(pdataBuffer->BufferPointer,0,pdataBuffer->MaxCellNums*pdataBuffer->BufferCellSize);
}
//���ݻ���д������1
void DataBuffer_WriteIndexInc(DATA_BUFFER *pdataBuffer)
{
	pdataBuffer->WriteIndex++;
	if(pdataBuffer->WriteIndex >= pdataBuffer->MaxCellNums)
		pdataBuffer->WriteIndex = 0;
}
//���ݻ����������1
void DataBuffer_ReadIndexInc(DATA_BUFFER *pdataBuffer)
{
	pdataBuffer->ReadIndex++;
	if(pdataBuffer->ReadIndex >= pdataBuffer->MaxCellNums)
		pdataBuffer->ReadIndex = 0;
}
//�������ݻ����л��浥Ԫ��ʹ�õ���Ŀ
void DataBuffer_SetUsedCellNum(DATA_BUFFER *pdataBuffer,unsigned int in_int)
{
	pdataBuffer->WritedNums = in_int;
}
//���дָ��
char* DataBuffer_GetWritePointer(DATA_BUFFER *pdataBuffer)
{
	return pdataBuffer->BufferPointer+pdataBuffer->WriteIndex*pdataBuffer->BufferCellSize;	
}
//��ö�ָ��
char* DataBuffer_GetReadPointer(DATA_BUFFER *pdataBuffer)
{
	return pdataBuffer->BufferPointer+pdataBuffer->ReadIndex*pdataBuffer->BufferCellSize;
}
