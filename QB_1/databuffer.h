/*****************************************************************************/
/*  databuffer.H v6.0.8                                                       */
/*                                                                            */
/******************************************************************************/

#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#define ACQ_BUFFER_START_POINTER 0x000A0400//数据缓存的起始地址
#define ACQ_BUFFER_CELL_SIZE 4000 //缓存单元大小
#define ACQ_BUFFER_NUMS 20	//最大10个缓存单元

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
	unsigned int ReadIndex;		 //缓存单元读索引
	unsigned int WriteIndex;	 //缓存单元写索引
	unsigned int WritedNums;//缓存单元占用数目
	unsigned int BufferCellSize;
	unsigned int MaxCellNums;
	char *BufferPointer;	//缓存起始地址
}DATA_BUFFER;

//数据缓存初始化
void DataBuffer_Initial(DATA_BUFFER *pdataBuffer,char* bufferPointer,unsigned int bufferCellSize,unsigned int maxCellNums);
//数据缓存写索引加1
void DataBuffer_WriteIndexInc(DATA_BUFFER *pdataBuffer);
//数据缓存读索引加1
void DataBuffer_ReadIndexInc(DATA_BUFFER *pdataBuffer);  

//数据缓存中,已用缓存单元的数目增1
#define DataBuffer_WritedNumsInc(pdataBuffer) (pdataBuffer)->WritedNums++
//角度缓存中,已用缓存单元的数目减1
#define DataBuffer_WritedNumsDec(pdataBuffer) (pdataBuffer)->WritedNums--
//设置数据缓存中缓存单元的使用的数目
void DataBuffer_SetUsedCellNum(DATA_BUFFER *pdataBuffer,unsigned int in_int); 
//获得数据缓存中缓存单元的使用的数目
#define DataBuffer_GetWritedNums(pdataBuffer) (pdataBuffer)->WritedNums

char* DataBuffer_GetWritePointer(DATA_BUFFER *pdataBuffer);

char* DataBuffer_GetReadPointer(DATA_BUFFER *pdataBuffer);

#endif
