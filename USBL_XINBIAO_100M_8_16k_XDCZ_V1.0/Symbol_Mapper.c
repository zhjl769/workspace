/* ===================================================================================
 * 文件名：Symbol_Mapper.c
 * 日期：2016年10月12日
 * 作者：xiaop
 *
 * 描述：Symbol_Mapper.c和Symbol_Mapper.h这两个文件主要完成的功能是符号映射
 * 版本：1.0
 * -----------------------------------------------------------------------------------
 * 修改内容: 调整星座图顺序
 * 修改日期: 2017年3月7日
 * 修改人: xiaop
 * ===================================================================================
 */

/* ================================== 头文件声明 ====================================== */
/* 系统模块声明 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* 符号映射模块声明 */
#include "function_declare.h"

/* ================================== 全局变量声明 ==================================== */
float constellation_bpsk[2][2] = {{-1,0},{1,0}};
float constellation_qpsk[4][2] = {{0.7071, 0.7071},{-0.7071, 0.7071},{0.7071,-0.7071},{-0.7071,-0.7071}};

/* ================================ Symbol_Mapper 函数 ================================ */
void Symbol_Mapper(char * Input_bits, int input_length, float *symbol_map, int constellation)
{
	volatile int i =0;
	int k = log2(constellation);
	char * Index_symbol = (char *)calloc(input_length/k, sizeof(char));
    
    if (k == 1)
    {
    	memcpy(Index_symbol, Input_bits, input_length*sizeof(char));
    }
    else
    {
    	for(i = 0; i < input_length/k; i++)
    	{
    		Index_symbol[i] = Input_bits[2*i]+Input_bits[2*i+1]*2;
    	}
    }

    switch(k)
    {
    	case 1:
    		for(i = 0; i < input_length; i++)
    		{
    			symbol_map[2*i] = constellation_bpsk[Index_symbol[i]][0];
    			symbol_map[2*i+1] = constellation_bpsk[Index_symbol[i]][1];
    		}
    		break;
    	case 2:
    		for(i = 0; i < input_length/k; i++)
    	    {
    	    	symbol_map[2*i] = constellation_qpsk[Index_symbol[i]][0];
    	    	symbol_map[2*i+1] = constellation_qpsk[Index_symbol[i]][1];
    	    }
    		break;
    }

    free(Index_symbol);
}
