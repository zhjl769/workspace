/* ===================================================================================
 * �ļ�����Symbol_combination.c
 * ���ڣ�2016��10��12��
 * ���ߣ�xiaop
 *
 * ������Symbol_combination.c��Symbol_combination.h�������ļ���Ҫ��ɵĹ����Ƿ������ӣ���
 * 		ѵ�����б��غ���Ϣ���ط���ͬһ������
 * �汾��1.0
 * ===================================================================================
 */

/* =================================== ͷ�ļ����� =================================== */
#include <stdio.h>
#include <string.h>
#include "function_declare.h"

/* ============================ Symbol_combination ���� ============================= */
/* Trains - ѵ�����У� length_Trains - ѵ�����еĳ��ȣ� Transmits - ��Ϣ���г��ȣ�
 * length_Transmits - ��Ϣ���еĳ��ȣ� Send_symbol - ���ͷ���
 */
void Symbol_combination(char * Trains, int length_Trains, char * Transmits, int length_Transmits, char * Send_symbol)
{
	volatile int i = 0;

	memset(Send_symbol, 0, (length_Trains+length_Transmits)*sizeof(char));
	for(i = 0; i < length_Trains; i++)
	{
		Send_symbol[i] = Trains[i];
	}
	for(i = length_Trains; i < length_Trains+length_Transmits; i++)
	{
		Send_symbol[i] = Transmits[i-length_Trains];
	}
}
