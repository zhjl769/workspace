/* ===================================================================================
 * �ļ�����crc.c
 * ���ڣ�2014��08��24��
 * ���ߣ�LiBo
 *
 * ������crc.c��crc.h�������ļ���Ҫ��ɵĹ����ǽ��м������������������CRCУ��,CRC�������
 *      ���ɵĹ���ʵ�����Ƕ����ƶ���ʽշת�������Ľ��
 * �汾��2.0
 * -----------------------------------------------------------------------------------
 * �޸����ݣ����ʦ�ֵĳ�������Ͻ���ϸ΢�޸ģ�ɾ�����ñ���
 * �޸����ڣ�2017��3��8��
 * �޸��ˣ�xiaop
 * ===================================================================================
 */

/* =============================== ͷ�ļ����� ======================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declare.h"

/* ================================== crc ���� ======================================= */
void crc(char* input, int length_input, char* output, char* gen, int length_gen)
{
	volatile int i=0;
	volatile int k=0;
	int length_g=length_gen;
	int length_in=length_input;
	char* remainder=(char*)malloc(length_g*sizeof(char)); // ����
	char* dividor=(char*)malloc(length_g*sizeof(char)); // ����
	char* dividend=(char*)malloc(length_g*sizeof(char)); // ������

	/* ��ʼ���� */
	if(input[0]==0) // ��ʼ������
	{
		memset(dividor,0,length_gen);
	}
	else
	{
		memcpy(dividor,gen,length_gen);
	}
	memcpy(dividend,input,length_gen);  // ��ʼ��������
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

	/* ����������� */
	memcpy(output,input,length_in);
	memcpy(&output[length_in-length_g+1],&remainder[1],length_g-1);

	/* �ͷ��ڴ� */
	free(remainder);
	free(dividor);
	free(dividend);
}
