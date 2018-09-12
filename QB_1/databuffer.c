/*******************************************************************************
* FILENAME
*  databuffer.c
*
* DESCRIPTION
*   为程序开辟内存数据存储空间
*
*******************************************************************************/

#include "databuffer.h"

//数据缓存初始化
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
//数据缓存写索引加1
void DataBuffer_WriteIndexInc(DATA_BUFFER *pdataBuffer)
{
	pdataBuffer->WriteIndex++;
	if(pdataBuffer->WriteIndex >= pdataBuffer->MaxCellNums)
		pdataBuffer->WriteIndex = 0;
}
//数据缓存读索引加1
void DataBuffer_ReadIndexInc(DATA_BUFFER *pdataBuffer)
{
	pdataBuffer->ReadIndex++;
	if(pdataBuffer->ReadIndex >= pdataBuffer->MaxCellNums)
		pdataBuffer->ReadIndex = 0;
}
//设置数据缓存中缓存单元的使用的数目
void DataBuffer_SetUsedCellNum(DATA_BUFFER *pdataBuffer,unsigned int in_int)
{
	pdataBuffer->WritedNums = in_int;
}
//获得写指针
char* DataBuffer_GetWritePointer(DATA_BUFFER *pdataBuffer)
{
	return pdataBuffer->BufferPointer+pdataBuffer->WriteIndex*pdataBuffer->BufferCellSize;	
}
//获得读指针
char* DataBuffer_GetReadPointer(DATA_BUFFER *pdataBuffer)
{
	return pdataBuffer->BufferPointer+pdataBuffer->ReadIndex*pdataBuffer->BufferCellSize;
}
