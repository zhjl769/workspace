/* ===================================================================================
 * �ļ�����Symbol_Mapper.c
 * ���ڣ�2016��10��12��
 * ���ߣ�xiaop
 *
 * ������Symbol_Mapper.c��Symbol_Mapper.h�������ļ���Ҫ��ɵĹ����Ƿ���ӳ��
 * �汾��1.0
 * -----------------------------------------------------------------------------------
 * �޸�����: ��������ͼ˳��
 * �޸�����: 2017��3��7��
 * �޸���: xiaop
 * ===================================================================================
 */

/* ================================== ͷ�ļ����� ====================================== */
/* ϵͳģ������ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* ����ӳ��ģ������ */
#include "function_declare.h"

/* ================================== ȫ�ֱ������� ==================================== */
float constellation_bpsk[2][2] = {{-1,0},{1,0}};
float constellation_qpsk[4][2] = {{0.7071, 0.7071},{-0.7071, 0.7071},{0.7071,-0.7071},{-0.7071,-0.7071}};

/* ================================ Symbol_Mapper ���� ================================ */
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
