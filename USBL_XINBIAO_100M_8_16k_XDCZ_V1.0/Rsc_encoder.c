/* ===================================================================================
 * 文件名：Rsc_encoder.h
 * 日期：2017年03月09日
 * 作者：xiaop
 *
 * 描述：Rsc_encode.c和Rsc_encoder.h这两个文件主要完成的功能是完成1/2码率的递归系统卷积码
 * 版本：1.0
 * ===================================================================================
 */

/* =============================== 头文件声明 ======================================== */
/* 系统模块声明 */
#include <stdio.h>
#include <stdlib.h>

/* 编码模块声明 */
#include "function_declare.h"

/* ============================== Rsc_encoder 函数 ================================== */
void Rsc_encoder(char *input, int len_input, char *output, short *gen)
{
	volatile int   DataLength, CodeLength, i, j;
	int   *g_encoder;
	int	  nn, KK, mm, max_states;
	int	  *input_int, *output_int;
	int   *out0, *out1, *state0, *state1, *tail;

	/* 编码数据长度 */
	DataLength = len_input;

	/* 为输入数据进行动态内存分配 */
	input_int = calloc(DataLength, sizeof(int) );
	for (i=0;i<DataLength;i++)
	{
		/* 强制类型转换 */
		input_int[i] = (int) input[i];
	}

	nn = 2; /* 生成矩阵的列数(gen维数),此步限制码率为1/2 */
	mm = 0; /* 移位寄存器长度初始化 */
	for( i= 0; i < nn; i++)
	{
		j = gen[i];
		KK = 0;
		while(j > 0)
		{
			j = j>>1;
			KK+=1;
		}
		if(mm<KK)
			mm = KK;
	}
	KK = mm; /* 生成多项式的长度(以2进制形式表示） */
	mm = KK - 1; /* 移位寄存器长度 */

	/* 为输出序列建立空间 */
	max_states = 1 << mm; /* 最大状态值 */

	/* 计算输出长度 */
	CodeLength = nn*(DataLength+mm);

	/* 转换生成多项式为十进制数据 */
	g_encoder = calloc(nn, sizeof(int) );

	for (i=0; i<nn; i++)
	{
		/* 强制类型转换 */
		g_encoder[i] = (int)gen[i];
	}

	/* 为中间变量及输出结果分配内存 */
	output_int = calloc(CodeLength, sizeof(int));
	out0 = calloc(max_states, sizeof(int));
	out1 = calloc(max_states, sizeof(int) );
	state0 = calloc(max_states, sizeof(int) );
	state1 = calloc(max_states, sizeof(int) );
	tail = calloc(max_states, sizeof(int) );

	/* 构造转移矩阵 */
	rsc_transit(out0, state0, 0, g_encoder, KK, nn);
	rsc_transit(out1, state1, 1, g_encoder, KK, nn);
	rsc_tail(tail, g_encoder, max_states, mm);

	/* 卷积码编码过程 */
	conv_encode(output_int, input_int, out0, state0, out1, state1, tail, KK, DataLength, nn);

	/* 转换输出结果 */
	for (i=0;i<CodeLength;i++)
	{
		output[i] = (char)output_int[i];
	}

	/* 释放内存 */
	free(output_int);
	free(input_int);
	free(g_encoder);
	free(out0);
	free(out1);
	free(state0);
	free(state1);
	free(tail);
}

/* ============================== rsc_enc_bit 函数 ================================== */
static void rsc_transit(int output_p[], int trans_p[], int input, int g[], int KK, int nn)
{
	int nextstate[1];
	int state, states;

	states = 1 << (KK-1); /* The number of states: 2^mm */

	/* Determine the output and next state for each possible starting state */
	for(state=0;state<states;state++) {
		output_p[state] = rsc_enc_bit( nextstate, input, state, g, KK, nn );
		trans_p[state]  = nextstate[0];
	}
	return;
}

/* ============================== rsc_enc_bit 函数 ================================== */
static int rsc_enc_bit(int state_out_p[], int input, int state_in, int g[], int KK, int nn)
{
	/* declare variables */
	int state, i, out, a_k;

	/* systematic output */
	out = input;

	/* determine feedback bit */
	a_k = input^parity_counter( g[0]&state_in, KK );

	/* create a word made up of state and feedback bit */
	state = (a_k<<(KK-1))^state_in;

	/* AND the word with the generators */
	for (i=1;i<nn;i++)
	{
		/* update output symbol */
		out = (out<<1) + parity_counter( state&g[i], KK );
	}

	/* shift the state to make the new state */
	state_out_p[0] = state>>1;
	return(out);
}

/* ================================ rsc_tail 函数 ==================================== */
static void rsc_tail(int	tail_p[], int g[], int max_states, int mm)
{
	int state;

	/* Determine the tail for each state */
	for(state=0;state<max_states;state++) {
		/* determine feedback word */
		tail_p[state] = parity_counter( g[0]&state, mm );
	}
	return;
}

/* ============================== parity_counter 函数 ================================== */
int parity_counter(int symbol, int length)
{
	int counter;
	int temp_parity = 0;

	for (counter=0;counter<length;counter++) {
		temp_parity = temp_parity^(symbol&1);
		symbol = symbol>>1;
	}

	return( temp_parity );
}

/* ================================ conv_encode 函数 ================================== */
static void conv_encode(int output_p[], int input[], int out0[], int state0[], int out1[],
		int state1[], int tail[],int KK, int LL, int nn)
{
	int i, j, outsym;
	int *bin_vec;
	int state = 0;

	/* Negative value in "tail" is a flag that this is
	  a tail-biting NSC code.  Determine initial state */

	if ( tail[0] < 0 ) {
		for (i=LL-KK+1;i<LL;i++) {
			if (input[i]) {
				/* Determine next state */
				state = state1[state];
			} else {
				/* Determine next state */
				state = state0[state];
			}
		}
	}

	bin_vec = calloc( nn, sizeof(int) );

	/* encode data bits one bit at a time */
	for (i=0;i<LL;i++) {
		if (input[i]) {
			/* Input is a one */
			outsym = out1[state];  /* The output symbol */

			/* Determine next state */
			state = state1[state];
		} else {
			/* Input is a zero */
			outsym = out0[state];  /* The output symbol */

			/* Determine next state */
			state = state0[state];
		}

		/* Convert symbol to a binary vector	*/
		itob( bin_vec, outsym, nn );

		/* Assign to output */
		for (j=0;j<nn;j++)
			output_p[nn*i+j] = bin_vec[j];
	}

	/* encode tail if needed */
	if (tail[0] >= 0) {
		for (i=LL;i<LL+KK-1;i++) {
			if (tail[state]) {
				/* Input is a one */
				outsym = out1[state];  /* The output symbol */

				/* Determine next state */
				state = state1[state];
			} else {
				/* Input is a zero */
				outsym = out0[state];  /* The output symbol */

				/* Determine next state */
				state = state0[state];
			}

			/* Convert symbol to a binary vector	*/
			itob( bin_vec, outsym, nn );

			/* Assign to output */
			for (j=0;j<nn;j++)
				output_p[nn*i+j] = bin_vec[j];
		}
	}

	free(bin_vec);

	return;
}

/* ================================== itob 函数 ====================================== */
void itob(int	binvec_p[], int symbol, int length)
{
	int counter;

	/* Go through each bit in the vector */
	for (counter=0;counter<length;counter++) {
		binvec_p[length-counter-1] = (symbol&1);
		symbol = symbol>>1;
	}

	return;
}
