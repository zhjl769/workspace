/* ===================================================================================
 * �ļ�����Interleaver.c
 * ���ڣ�2016��10��12��
 * ���ߣ�xiaop
 *
 * ������Interleaver.c��Interleaver.h�������ļ���Ҫ��ɵĹ����������֯
 * �汾��1.0
 * ===================================================================================
 */

/* ================================ ͷ�ļ����� ======================================= */
#include <stdio.h>
#include "function_declare.h"

/* ============================= Interleaver���� ===================================== */
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
