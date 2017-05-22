/*
 * Create_signal.h
 *
 *  Created on: 2017-5-10
 *      Author: Administrator
 */

#ifndef CREATE_SIGNAL_H_
#define CREATE_SIGNAL_H_



/* ϵͳģ������ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "function_declare.h"

/* �źŴ���ģ������ */
//#include "crc.h" /* CRCУ�� */
//#include "Rsc_encoder.h" /* ϵͳ�ݹ����뺯�� */
//#include "Interleaver.h" /* ��֯���� */
//#include "Symbol_combination.h" /* �������Ӻ��� */
//#include "Pwm_modulation.h" /* ���PWM���ƺ��� */

/* Ԥ��ʼ���������� */

#include "var_creat_signal.h" /* Ԥ������� */

/* ================================ �궨������ ======================================= */
#define K				(7) /* ������������Ƴ���+1 */
#define len_train    	(500) /* ѵ�����г��� */
#define CONSTELLATION  (2) /*  ����ͼѡ��: BPSK = 2, QPSK = 4 */

#define LEN_CRC			(7) /* CRCУ�����ʽ���� */
#define LEN_TAIL		(8) /* ��Ϣ���ؽ�β������ */

/* ============================= ȫ�ֱ������� ======================================== */

/* ================================== crc ���� ======================================= */
void crc(char* input, int length_input, char* output, char* gen, int length_gen)
{
	int i=0;
	int k=0;
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

/* ============================== parity_counter ���� ================================== */
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

/* ============================== rsc_enc_bit ���� ================================== */
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

/* ============================== rsc_enc_bit ���� ================================== */
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

/* ================================ rsc_tail ���� ==================================== */
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



/* ================================== itob ���� ====================================== */
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


/* ================================ conv_encode ���� ================================== */
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



void Rsc_encoder(char *input, int len_input, char *output, short *gen)
{
	int   DataLength, CodeLength, i, j;
	int   *g_encoder;
	int	  nn, KK, mm, max_states;
	int	  *input_int, *output_int;
	int   *out0, *out1, *state0, *state1, *tail;

	/* �������ݳ��� */
	DataLength = len_input;

	/* Ϊ�������ݽ��ж�̬�ڴ���� */
	input_int = calloc(DataLength, sizeof(int) );
	for (i=0;i<DataLength;i++)
	{
		/* ǿ������ת�� */
		input_int[i] = (int) input[i];
	}

	nn = 2; /* ���ɾ��������(genά��),�˲���������Ϊ1/2 */
	mm = 0; /* ��λ�Ĵ������ȳ�ʼ�� */
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
	KK = mm; /* ���ɶ���ʽ�ĳ���(��2������ʽ��ʾ�� */
	mm = KK - 1; /* ��λ�Ĵ������� */

	/* Ϊ������н����ռ� */
	max_states = 1 << mm; /* ���״ֵ̬ */

	/* ����������� */
	CodeLength = nn*(DataLength+mm);

	/* ת�����ɶ���ʽΪʮ�������� */
	g_encoder = calloc(nn, sizeof(int) );

	for (i=0; i<nn; i++)
	{
		/* ǿ������ת�� */
		g_encoder[i] = (int)gen[i];
	}

	/* Ϊ�м�����������������ڴ� */
	output_int = calloc(CodeLength, sizeof(int));
	out0 = calloc(max_states, sizeof(int));
	out1 = calloc(max_states, sizeof(int) );
	state0 = calloc(max_states, sizeof(int) );
	state1 = calloc(max_states, sizeof(int) );
	tail = calloc(max_states, sizeof(int) );

	/* ����ת�ƾ��� */
	rsc_transit(out0, state0, 0, g_encoder, KK, nn);
	rsc_transit(out1, state1, 1, g_encoder, KK, nn);
	rsc_tail(tail, g_encoder, max_states, mm);

	/* ����������� */
	conv_encode(output_int, input_int, out0, state0, out1, state1, tail, KK, DataLength, nn);

	/* ת�������� */
	for (i=0;i<CodeLength;i++)
	{
		output[i] = (char)output_int[i];
	}

	/* �ͷ��ڴ� */
	free(output_int);
	free(input_int);
	free(g_encoder);
	free(out0);
	free(out1);
	free(state0);
	free(state1);
	free(tail);
}

/* ============================= Interleaver���� ===================================== */
void Interleaver(char * Input_code_bits,int Input_code_bits_length,
		char * Output_interleave_bits,
		short * Interleaver_index)
{
	int i = 0;
	for(i =0; i < Input_code_bits_length; i++)
	{
		Output_interleave_bits[i] = Input_code_bits[Interleaver_index[i]];
	}

}

/* ============================ Symbol_combination ���� ============================= */
/* Trains - ѵ�����У� length_Trains - ѵ�����еĳ��ȣ� Transmits - ��Ϣ���г��ȣ�
 * length_Transmits - ��Ϣ���еĳ��ȣ� Send_symbol - ���ͷ���
 */
void Symbol_combination(char * Trains, int length_Trains, char * Transmits, int length_Transmits, char * Send_symbol)
{
	int i = 0;

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

/* ================================ Pwm_modulate ���� ================================ */
/* Send_symbol - ���ͷ���; len_Send_symbol - ���ͷ��ų���; Pwm_mod - PWM��������;
 * Pwm_List - Pwm���Ʋ����б�; len_Pwm_List - ����PWM���γ���
 */
void Pwm_modulation(char *Send_symbol, int len_Send_symbol, short *Pwm_mod,
		char *Pwm_List, int len_Pwm_List)
{
	volatile int i, j, index;
	char trans_info[7] = {0};
	long unsigned int temp_dest;
	long unsigned int temp_src;
	short *temp_short_dest;
	short *temp_short_src;

	memcpy(trans_info+4, Send_symbol, 3*sizeof(char)); /* ������س�ʼ�� */
	for(i=0; i<len_Send_symbol; i++)
	{
		memcpy(trans_info, trans_info+1, 6*sizeof(char)); /* ������λ */
		if (i < (len_Send_symbol-3))
		{
			trans_info[6] = Send_symbol[3+i];
		}
		else
		{
			trans_info[6] = 0;
		}

		index = 0; /* ���������� */
		for(j=0; j<7; j++)
		{
			index += (trans_info[j]<<(6-j));
		}

		/* ��ȡPWM�������� */
		//////////////////////////////////////////////////////////////////////////////////////////////
		//memcpy(Pwm_mod+i*len_Pwm_List, Pwm_List+index*len_Pwm_List, len_Pwm_List*sizeof(char));
		//////////////////////////////////////////////////////////////////////////////////////////////
		/*������������ݵ�Դ��ַ��Ŀ�ĵ�ַ*/
		temp_dest 	= (long unsigned int)Pwm_mod	+(long unsigned int)i		*(long unsigned int)len_Pwm_List;
		temp_src 	= (long unsigned int)Pwm_List	+(long unsigned int)index	*(long unsigned int)len_Pwm_List;
		temp_short_dest = (short *)temp_dest;
		temp_short_src	= (short *)temp_src;

		cross_page_copy_short( temp_short_dest , temp_short_src , len_Pwm_List*sizeof(char));
	}
}

/* ========================== Create_Pwm ���� ===================================== */
/*�������ܣ���PWM�γ̱���ת��ΪPWM�ź�*/
void Translate_Pwm(short *RLE , short *Pwm , int len_Send_symbol , int len_Pwm_List)
{
	long int i,j,k;//ѭ����
	unsigned int z = 0;//��ż���ж�
	short count;//���ڼ�¼����+1��-1�ĸ���
	unsigned long int length = 0;//���ڼ�¼����PWM_result���������

	for(i=0;i<len_Send_symbol;i++)
	{
		z=0;//���¶�z���㣬��Ϊÿ������Ϊһ��ѭ�������γ̱����У�һ��������97������ɣ���һ������-1�ĸ�������ô���һλҲӦ����-1�ĸ���������ζ��ÿ�����Ŷ�����-1��Ϊ��ͷ�ͽ�β�ģ�
		for(j=0;j<len_Pwm_List;j++)
		{
			count = RLE[i*len_Pwm_List + j];
			z++;//ÿ��RLE�ж���һ�����󣬶���z�Լӣ����ʹ��z����ż�ı仯�ȼ����γ̱�����+1/-1�ı仯��
			if(z%2 == 0)//zΪż��
			{
				for(k=0;k<count;k++)
				{
					//Pwm[length] = 1;//�����ã���MATLAB�Ա�
					Pwm[length] = 0x03;
					length++;
				}
			}
			else//zΪ����
			{
				for(k=0;k<count;k++)
				{
					//Pwm[length] = -1;//�����ã���MATLAB�Ա�
					Pwm[length] = 0x05;
					length++;
				}
			}//end if
		}//end for(j)
	}//end for(i)
}

/* ========================== Create_signal ���� ===================================== */
void Create_signal(char *Input_bits, int len_input, short *Pwm_mod,long int *len_Pwm_Mod)
{
	/* �ֲ�������ʼ�� */
	int CONSTELLATION_LOG2 = log2(CONSTELLATION);
	int len_crc = (len_input+LEN_CRC+LEN_TAIL-1);
	int len_encode = (len_crc+K-1)*2;
	int len_Send_symbol = len_encode+len_train;

	/* ���ɾ����ʼ�� */
	short gen[2] = {91, 121};

	/* ��̬�����ڴ� */
	char  *Crc_bits          = (char *)calloc(len_crc,          sizeof(char));
	char  *Encode_symbol     = (char  *)calloc(len_encode,      sizeof(char));
	char  *Interleave_symbol = (char  *)calloc(len_encode,      sizeof(char));
	char  *Send_symbol       = (char  *)calloc(len_Send_symbol, sizeof(char));

	/* BPSK���ƹ��� */
	*len_Pwm_Mod = (long int)len_Send_symbol*97; /* PWM�����źų��� */

	/* CRCУ�� */
	memcpy(Crc_bits, Input_bits, sizeof(char)*len_input); /* ��ֵ��Ϣ���� */
	memcpy(Crc_bits+len_input, tail_bit, sizeof(char)*LEN_TAIL); /* �����β�� */
	crc(Crc_bits, len_crc, Crc_bits, crc_check_bit, LEN_CRC); /* CRCУ�� */

	/* �ݹ�ϵͳ�������� */
	Rsc_encoder(Crc_bits, len_crc, Encode_symbol, gen);

	/* �����֯ */
	Interleaver(Encode_symbol, len_encode, Interleave_symbol, index_2740);

	/* �������� */
	Symbol_combination(Trainsymbol, len_train, Interleave_symbol, len_encode, Send_symbol);

	cross_page_set0_float(Pwm_mod , 157140);//������������㸡���͵ģ��������short��n�ó���2

	/* ���PWM���� */
	Pwm_modulation(Send_symbol, len_Send_symbol, Pwm_mod, Pwm_List, 97);

	cross_page_set0_float(Pwm_result , 777600);//������������㸡���͵ģ��������short��n�ó���2

	Translate_Pwm(Pwm_mod , Pwm_result , len_Send_symbol , 97);

	/* �ͷ��ڴ� */
	free(Crc_bits);
	free(Encode_symbol);
	free(Interleave_symbol);
	free(Send_symbol);
}










#endif /* CREATE_SIGNAL_H_ */
