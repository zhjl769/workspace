/*
 * cross_page_set0.c
 *
 *  Created on: 2017-3-24
 *      Author: Administrator
 */
#include <string.h>

#include "function_declare.h"
/*---------------------------------int������-------------------------------------------*/
//void cross_page_set0_int(void *A , unsigned long int B)
//{
//	unsigned long int page_count;
//	unsigned long int page_count_rest;
//	int i;
//	int *C;//int��
//	C = (int*)A;
//
//	page_count 			= B/65536;//65536��int=128KB,������Ҫ�������ҳ��
//	page_count_rest 	= B%65536;//����
//	for(i=0;i<page_count;i++)
//	{
//		*C 	= 0;//��ǰҳ��һ����ַ��������
//		C 	= C+1;//��ַ��1,�ӵ�ǰҳ�ڶ�����ַ��ʼ��������
//		memset(C,0,65535*sizeof(int));//��ǰҳʣ�µĵ�ַȫ������
//		C 	= C+65535;//��ַ��65535��������һҳ��һ����ַ
//	}
//	switch(page_count_rest)//�ж�����
//	{
//	case 0://��0
//		break;
//	case 1://��1.˵�����һҳֻʣ�µ�һ����ַû������
//		*C 	= 0;//��ǰҳ��һ����ַ��������
//		break;
//	default://����>1������ʣ��һ������ҳ�Ŀռ���Ҫ����
//		*C 	= 0;//��ǰҳ��һ����ַ��������
//		C 	= C+1;//��ַ��1,�ӵ�ǰҳ�ڶ�����ַ��ʼ��������
//		memset(C,0,(page_count_rest-1)*sizeof(int));//��ǰҳʣ�µĵ�ַȫ�����㣬ע��˴�����Ҫ��1����Ϊ��ǰҳ�ĵ�һ����ַ�Ѿ������ˣ�
//		break;
//	}
//}

/*---------------------------------float������-------------------------------------------*/
void cross_page_set0_float(void *A , unsigned long int B)
{//ʹ��memset����ʱ�����ܲ���һҳ�е��׵�ַ�������ǰ��������ַ���ǵ������������ַ�е����ݣ�
	//���ô˺�����ע�⣺A������ĳһҳ����ʼ��ַ������
	long int page_count;
	long int page_count_rest;
	int i;
	float *C;//float��
	C = (float*)A;

	page_count 			= B/32768;//32768��float=128KB,������Ҫ�������ҳ��
	page_count_rest 	= B%32768;//����
	for(i=0;i<page_count;i++)
	{
		*C 	= 0;//��ǰҳ��һ����ַ��������
		C 	= C+1;//��ַ��1,�ӵ�ǰҳ�ڶ�����ַ��ʼ��������
		memset(C,0,32767*sizeof(float));//��ǰҳʣ�µĵ�ַȫ������
		C 	= C+32767;//��ַ��32768��������һҳ��һ����ַ
	}
	switch(page_count_rest)//�ж�����
	{
	case 0://��0
		break;
	case 1://��1.˵�����һҳֻʣ�µ�һ����ַû������
		*C 	= 0;//��ǰҳ��һ����ַ��������
		break;
	default://����>1������ʣ��һ������ҳ�Ŀռ���Ҫ����
		*C 	= 0;//��ǰҳ��һ����ַ��������
		C 	= C+1;//��ַ��1,�ӵ�ǰҳ�ڶ�����ַ��ʼ��������
		memset(C,0,(page_count_rest-1)*sizeof(float));//��ǰҳʣ�µĵ�ַȫ�����㣬ע��˴�����Ҫ��1����Ϊ��ǰҳ�ĵ�һ����ַ�Ѿ������ˣ�
		break;
	}
}
///*---------------------------------float������-------------------------------------------*/
//void cross_page_copy_float_src(float *dest , float *src,unsigned long int B)
//{
//	//B�������4����
//	//1��floatռ2����ַ����
//	//ʹ��ָ�븳ֵʱ������ָ��Ӽ�����ʹ��ָ���ҳ��ᷢ������������ת���ɳ����ͣ���ַ�Ӽ�����ת��ָ�롣Ҫע��˴�floatָ��+1����Ч�ڵ�ַ+2������
//	long unsigned int add_const;
//	long unsigned int add_tmp;
//	add_const = (((long unsigned int)src) & ((long unsigned int)0x000FFFFu));
//	//No.1
//	if(add_const == 0xFFFEu)//�����ַ�����ƿ�ʼ��ַ�����Ǳ�ҳ���һ����ַ
//	{
//		add_tmp	= (long unsigned int)src;//������0xFFFE��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//ָ���1��׼����������
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-2)*sizeof(float));
//	}
//	//No.2
//	else if(add_const == 0x0000u)//�����ַ�����ƿ�ʼ��ַ�����Ǳ�ҳ��һ����ַ
//	{
//		add_tmp	= (long unsigned int)src;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//ָ���1��׼����������
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-1)*sizeof(float));
//	}
//	//No.3
//	else if((add_const + (B-1)*2) == 0x000FFFEu)//���ø��Ƶ���ҳ�Ľ�β�����β��ַ������ͨ��������ֵ�ķ���ʵ�ָ���
//	{
//		memcpy(dest,src,(B-1)*sizeof(float));//�ȸ��ƣ��ٵ�����ֵ
//
//		add_tmp	= (long unsigned int)src + (B-1u)*2;
//		src 	= (float *)add_tmp;//������0xFFFE��ֵ
//
//		add_tmp	= (long unsigned int)dest + (B-1u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.4
//	else if(((add_const + (B-1)*2)&0x000FFFFu) == 0x0000u)//���ø��Ƶ���һҳ�ĵ�һ����ַ����ҳ��β��ַ����һҳ��ʼ��ַ������ͨ��������ֵ�ķ���ʵ�ָ���
//	{
//		memcpy(dest,src,(B-2)*sizeof(float));//�ȸ��ƣ��ٵ�����ֵ
//
//		add_tmp	= (long unsigned int)src + (B-2u)*2;//������0xFFFE��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (B-2u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.5
//	else if(((add_const + (B-1)*2)&0x001FFFFu) > 0x10000u)//����ʱ�����˿�ҳ��ֵ�����ڵ�ǰҳ���һ����ַ���ָ�ȸ��Ƶ���ǰҳ��β֮ǰ��һ�����ݣ�Ȼ��ҳ��β�Լ���һҳҳ�׵�����ֵ�����ֵ��һҳ����
//	{
//		memcpy(dest,src,(0xFFFEu - (unsigned short)add_const)/2*sizeof(float));//�ȸ��Ƶ�0xFFFE
//
//		add_tmp	= (long unsigned int)src + (0xFFFEu - (unsigned short)add_const);//������0xFFFE��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (0xFFFEu - (unsigned short)add_const);
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//ָ���1��׼����������
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(add_const+B*2-0x10002)/2*sizeof(float));
//	}
//	else
//	{
//		memcpy(dest,src,B*sizeof(float));
//	}
//}
//
///*---------------------------------float������-------------------------------------------*/
//void cross_page_copy_float_dest(float *dest , float *src,unsigned long int B)
//{
//	//B�������4����
//	//1��floatռ2����ַ����
//	//ʹ��ָ�븳ֵʱ������ָ��Ӽ�����ʹ��ָ���ҳ��ᷢ������������ת���ɳ����ͣ���ַ�Ӽ�����ת��ָ�롣Ҫע��˴�floatָ��+1����Ч�ڵ�ַ+2������
//	long unsigned int add_const;
//	long unsigned int add_tmp;
//	add_const = (((long unsigned int)dest) & ((long unsigned int)0x000FFFFu));
//	//No.1
//	if(add_const == 0xFFFEu)//�����ַ�����ƿ�ʼ��ַ�����Ǳ�ҳ���һ����ַ
//	{
//		add_tmp	= (long unsigned int)src;//������0xFFFE��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//ָ���1��׼����������
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-2)*sizeof(float));
//	}
//	//No.2
//	else if(add_const == 0x0000u)//�����ַ�����ƿ�ʼ��ַ�����Ǳ�ҳ��һ����ַ
//	{
//		add_tmp	= (long unsigned int)src;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//ָ���1��׼����������
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-1)*sizeof(float));
//	}
//	//No.3
//	else if((add_const + (B-1)*2) == 0x000FFFEu)//���ø��Ƶ���ҳ�Ľ�β�����β��ַ������ͨ��������ֵ�ķ���ʵ�ָ���
//	{
//		memcpy(dest,src,(B-1)*sizeof(float));//�ȸ��ƣ��ٵ�����ֵ
//
//		add_tmp	= (long unsigned int)src + (B-1u)*2;
//		src 	= (float *)add_tmp;//������0xFFFE��ֵ
//
//		add_tmp	= (long unsigned int)dest + (B-1u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.4
//	else if(((add_const + (B-1)*2)&0x000FFFFu) == 0x0000u)//���ø��Ƶ���һҳ�ĵ�һ����ַ����ҳ��β��ַ����һҳ��ʼ��ַ������ͨ��������ֵ�ķ���ʵ�ָ���
//	{
//		memcpy(dest,src,(B-2)*sizeof(float));//�ȸ��ƣ��ٵ�����ֵ
//
//		add_tmp	= (long unsigned int)src + (B-2u)*2;//������0xFFFE��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (B-2u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.5
//	else if(((add_const + (B-1)*2)&0x001FFFFu) > 0x10000u)//����ʱ�����˿�ҳ��ֵ�����ڵ�ǰҳ���һ����ַ���ָ�ȸ��Ƶ���ǰҳ��β֮ǰ��һ�����ݣ�Ȼ��ҳ��β�Լ���һҳҳ�׵�����ֵ�����ֵ��һҳ����
//	{
//		memcpy(dest,src,(0xFFFEu - (unsigned short)add_const)/2*sizeof(float));//�ȸ��Ƶ�0xFFFE
//
//		add_tmp	= (long unsigned int)src + (0xFFFEu - (unsigned short)add_const);//������0xFFFE��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (0xFFFEu - (unsigned short)add_const);
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//������0x0000��ֵ
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//ָ���1��׼����������
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(add_const+B*2-0x10002)/2*sizeof(float));
//	}
//	else
//	{
//		memcpy(dest,src,B*sizeof(float));
//	}
//}


/*---------------------------------float������-------------------------------------------*/
void cross_page_copy_float(float *dest , float *src,unsigned long int B)
{//B��ʾ�����Ƶ�float�ĸ�����BҪС��32768�������Ƴ��Ȳ��ܳ���128KB��Ҳ����һҳ��
	long unsigned int add_tail_1;
	long unsigned int add_tail_2;
	long unsigned int add_tail_3;
	long unsigned int add_tail_4;
	long unsigned int i;
	add_tail_1 = (((long unsigned int)src) & ((long unsigned int)0x000FFFFu));//ȡԴ��ַ��16λ
	add_tail_2 = (((long unsigned int)dest) & ((long unsigned int)0x000FFFFu));//ȡĿ�ĵ�ַ��16λ
	add_tail_3 = add_tail_1 + B*2;//ȡ��Դ��ַ+���Ƴ��ȣ���16λ
	add_tail_4 = add_tail_2 + B*2;//ȡ��Ŀ�ĵ�ַ+���Ƴ��ȣ���16λ
	if((add_tail_1 == 0xFFFEu)||(add_tail_1 == 0x0000u)||(add_tail_2 == 0xFFFEu)||(add_tail_2 == 0x0000u)||(add_tail_3 >= 0x10000u)||(add_tail_4 >= 0x10000u))//�����ַ
	//---------Դ��ַ��ҳβ---------------Դ��ַ��ҳ��---------------Ŀ�ĵ�ַ��ҳβ----------------Ŀ�ĵ�ַ��ҳ��------------Դ��ַ���ƿ�ҳ----------------Ŀ�ĵ�ַ���ƿ�ҳ-----//
	{//�������������������鷽ʽ��������
		for(i=0;i<B;i++)
		{
			dest[i] = src[i];
		}
	}
	else
	{
		memcpy(dest,src,B*sizeof(float));
	}
}

/*---------------------------------short������-------------------------------------------*/
void cross_page_copy_short(void *dest , void *src,unsigned long int B)
{//B��ʾ�����Ƶ�short�ĸ�����BҪС��65536�������Ƴ��Ȳ��ܳ���128KB��Ҳ����һҳ��
	short *dest_short;
	short *src_short;
	dest_short 	= dest;	//����ָ����ָ��ĵ�ַ��ת�Ƶ�������ָ����ָ��ĵ�ַ
	src_short 	= src;	//����ָ����ָ��ĵ�ַ��ת�Ƶ�������ָ����ָ��ĵ�ַ
	long unsigned int add_tail_1;
	long unsigned int add_tail_2;
	long unsigned int add_tail_3;
	long unsigned int add_tail_4;
	long unsigned int i;
	add_tail_1 = (((long unsigned int)src) & ((long unsigned int)0x000FFFFu));//ȡԴ��ַ��16λ
	add_tail_2 = (((long unsigned int)dest) & ((long unsigned int)0x000FFFFu));//ȡĿ�ĵ�ַ��16λ
	add_tail_3 = add_tail_1 + B;//ȡ��Դ��ַ+���Ƴ��ȣ���16λ
	add_tail_4 = add_tail_2 + B;//ȡ��Ŀ�ĵ�ַ+���Ƴ��ȣ���16λ
	if((add_tail_1 == 0xFFFFu)||(add_tail_1 == 0x0000u)||(add_tail_2 == 0xFFFFu)||(add_tail_2 == 0x0000u)||(add_tail_3 >= 0x10000u)||(add_tail_4 >= 0x10000u))//�����ַ
		//---------Դ��ַ��ҳβ---------------Դ��ַ��ҳ��---------------Ŀ�ĵ�ַ��ҳβ----------------Ŀ�ĵ�ַ��ҳ��------------Դ��ַ���ƿ�ҳ----------------Ŀ�ĵ�ַ���ƿ�ҳ-----//
	{//�������������������鷽ʽ��������

		for(i=0;i<B;i++)
		{
			dest_short[i] = src_short[i];
		}
	}
	else
	{
		memcpy(dest_short,src_short,B*sizeof(short));
	}
}


