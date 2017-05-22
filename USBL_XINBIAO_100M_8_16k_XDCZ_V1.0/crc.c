/* ===================================================================================
 * 文件名：crc.c
 * 日期：2014年08月24日
 * 作者：LiBo
 *
 * 描述：crc.c和crc.h这两个文件主要完成的功能是进行加入冗余比特译码数据CRC校验,CRC冗余比特
 *      生成的过程实质上是二进制多项式辗转相除计算的结果
 * 版本：2.0
 * -----------------------------------------------------------------------------------
 * 修改内容：在李波师兄的程序基础上进行细微修改，删除无用变量
 * 修改日期：2017年3月8日
 * 修改人：xiaop
 * ===================================================================================
 */

/* =============================== 头文件声明 ======================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declare.h"

/* ================================== crc 函数 ======================================= */
void crc(char* input, int length_input, char* output, char* gen, int length_gen)
{
	volatile int i=0;
	volatile int k=0;
	int length_g=length_gen;
	int length_in=length_input;
	char* remainder=(char*)malloc(length_g*sizeof(char)); // 余数
	char* dividor=(char*)malloc(length_g*sizeof(char)); // 除数
	char* dividend=(char*)malloc(length_g*sizeof(char)); // 被除数

	/* 初始化商 */
	if(input[0]==0) // 初始化除数
	{
		memset(dividor,0,length_gen);
	}
	else
	{
		memcpy(dividor,gen,length_gen);
	}
	memcpy(dividend,input,length_gen);  // 初始化被除数
	for(i=0;i<(length_in-length_g+1);i++)
	{


		for(k=0;k<length_g;k++)
		{
			remainder[k]=(dividor[k])^(dividend[k]);
		}
		if(i==(length_in-length_g))
		{
			break;
		}
		for(k=0;k<length_g-1;k++)
		{
			remainder[k]=remainder[k+1];
		}
		remainder[length_g-1]=input[i+length_g];

		memcpy(dividend,remainder,length_gen);

		if(dividend[0]==0)
		{
			memset(dividor,0,length_gen);
		}
		else
		{
			memcpy(dividor,gen,length_gen);
		}

	}

	/* 加入冗余比特 */
	memcpy(output,input,length_in);
	memcpy(&output[length_in-length_g+1],&remainder[1],length_g-1);

	/* 释放内存 */
	free(remainder);
	free(dividor);
	free(dividend);
}
