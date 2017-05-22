/* ===================================================================================
 * 文件名：Symbol_combination.c
 * 日期：2016年10月12日
 * 作者：xiaop
 *
 * 描述：Symbol_combination.c和Symbol_combination.h这两个文件主要完成的功能是符号连接，将
 * 		训练序列比特和信息比特放置同一数组内
 * 版本：1.0
 * ===================================================================================
 */

/* =================================== 头文件声明 =================================== */
#include <stdio.h>
#include <string.h>
#include "function_declare.h"

/* ============================ Symbol_combination 函数 ============================= */
/* Trains - 训练序列； length_Trains - 训练序列的长度； Transmits - 信息序列长度；
 * length_Transmits - 信息序列的长度； Send_symbol - 发送符号
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
