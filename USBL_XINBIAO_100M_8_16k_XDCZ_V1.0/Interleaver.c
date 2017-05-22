/* ===================================================================================
 * 文件名：Interleaver.c
 * 日期：2016年10月12日
 * 作者：xiaop
 *
 * 描述：Interleaver.c和Interleaver.h这两个文件主要完成的功能是随机交织
 * 版本：1.0
 * ===================================================================================
 */

/* ================================ 头文件声明 ======================================= */
#include <stdio.h>
#include "function_declare.h"

/* ============================= Interleaver函数 ===================================== */
void Interleaver(char * Input_code_bits,int Input_code_bits_length,
				 char * Output_interleave_bits,
				 short * Interleaver_index)
{
	volatile int i = 0;
	for(i =0; i < Input_code_bits_length; i++)
	{
		Output_interleave_bits[i] = Input_code_bits[Interleaver_index[i]];
	}

}
