/*
 * cross_page_set0.c
 *
 *  Created on: 2017-3-24
 *      Author: Administrator
 */
#include <string.h>

#include "function_declare.h"
/*---------------------------------int型数组-------------------------------------------*/
//void cross_page_set0_int(void *A , unsigned long int B)
//{
//	unsigned long int page_count;
//	unsigned long int page_count_rest;
//	int i;
//	int *C;//int型
//	C = (int*)A;
//
//	page_count 			= B/65536;//65536个int=128KB,计算需要清零的整页数
//	page_count_rest 	= B%65536;//余数
//	for(i=0;i<page_count;i++)
//	{
//		*C 	= 0;//当前页第一个地址单独清零
//		C 	= C+1;//地址加1,从当前页第二个地址开始批量清零
//		memset(C,0,65535*sizeof(int));//当前页剩下的地址全部清零
//		C 	= C+65535;//地址加65535，跳至下一页第一个地址
//	}
//	switch(page_count_rest)//判断余数
//	{
//	case 0://余0
//		break;
//	case 1://余1.说明最后一页只剩下第一个地址没有清零
//		*C 	= 0;//当前页第一个地址单独清零
//		break;
//	default://余数>1，即还剩下一个非整页的空间需要清零
//		*C 	= 0;//当前页第一个地址单独清零
//		C 	= C+1;//地址加1,从当前页第二个地址开始批量清零
//		memset(C,0,(page_count_rest-1)*sizeof(int));//当前页剩下的地址全部清零，注意此处余数要减1，因为当前页的第一个地址已经清完了！
//		break;
//	}
//}

/*---------------------------------float型数组-------------------------------------------*/
void cross_page_set0_float(void *A , unsigned long int B)
{//使用memset函数时，不能操作一页中的首地址（无论是包含这个地址还是单独清零这个地址中的数据）
	//调用此函数需注意：A必须是某一页的起始地址！！！
	long int page_count;
	long int page_count_rest;
	int i;
	float *C;//float型
	C = (float*)A;

	page_count 			= B/32768;//32768个float=128KB,计算需要清零的整页数
	page_count_rest 	= B%32768;//余数
	for(i=0;i<page_count;i++)
	{
		*C 	= 0;//当前页第一个地址单独清零
		C 	= C+1;//地址加1,从当前页第二个地址开始批量清零
		memset(C,0,32767*sizeof(float));//当前页剩下的地址全部清零
		C 	= C+32767;//地址加32768，跳至下一页第一个地址
	}
	switch(page_count_rest)//判断余数
	{
	case 0://余0
		break;
	case 1://余1.说明最后一页只剩下第一个地址没有清零
		*C 	= 0;//当前页第一个地址单独清零
		break;
	default://余数>1，即还剩下一个非整页的空间需要清零
		*C 	= 0;//当前页第一个地址单独清零
		C 	= C+1;//地址加1,从当前页第二个地址开始批量清零
		memset(C,0,(page_count_rest-1)*sizeof(float));//当前页剩下的地址全部清零，注意此处余数要减1，因为当前页的第一个地址已经清完了！
		break;
	}
}
///*---------------------------------float型数组-------------------------------------------*/
//void cross_page_copy_float_src(float *dest , float *src,unsigned long int B)
//{
//	//B必须大于4！！
//	//1个float占2个地址！！
//	//使用指针赋值时，若对指针加减运算使得指针跨页则会发生错误，所以先转换成长整型，地址加减后再转成指针。要注意此处float指针+1，等效于地址+2！！！
//	long unsigned int add_const;
//	long unsigned int add_tmp;
//	add_const = (((long unsigned int)src) & ((long unsigned int)0x000FFFFu));
//	//No.1
//	if(add_const == 0xFFFEu)//特殊地址：复制开始地址正好是本页最后一个地址
//	{
//		add_tmp	= (long unsigned int)src;//单独对0xFFFE赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//指针加1，准备批量复制
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-2)*sizeof(float));
//	}
//	//No.2
//	else if(add_const == 0x0000u)//特殊地址：复制开始地址正好是本页第一个地址
//	{
//		add_tmp	= (long unsigned int)src;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//指针加1，准备批量复制
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-1)*sizeof(float));
//	}
//	//No.3
//	else if((add_const + (B-1)*2) == 0x000FFFEu)//正好复制到本页的结尾，则结尾地址的数据通过单独赋值的方法实现复制
//	{
//		memcpy(dest,src,(B-1)*sizeof(float));//先复制，再单独赋值
//
//		add_tmp	= (long unsigned int)src + (B-1u)*2;
//		src 	= (float *)add_tmp;//单独对0xFFFE赋值
//
//		add_tmp	= (long unsigned int)dest + (B-1u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.4
//	else if(((add_const + (B-1)*2)&0x000FFFFu) == 0x0000u)//正好复制到下一页的第一个地址，则本页结尾地址与下一页开始地址的数据通过单独赋值的方法实现复制
//	{
//		memcpy(dest,src,(B-2)*sizeof(float));//先复制，再单独赋值
//
//		add_tmp	= (long unsigned int)src + (B-2u)*2;//单独对0xFFFE赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (B-2u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.5
//	else if(((add_const + (B-1)*2)&0x001FFFFu) > 0x10000u)//复制时发生了跨页赋值，则在当前页最后一个地址处分割，先复制到当前页结尾之前的一个数据，然后页结尾以及下一页页首单独赋值，最后赋值下一页数据
//	{
//		memcpy(dest,src,(0xFFFEu - (unsigned short)add_const)/2*sizeof(float));//先复制到0xFFFE
//
//		add_tmp	= (long unsigned int)src + (0xFFFEu - (unsigned short)add_const);//单独对0xFFFE赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (0xFFFEu - (unsigned short)add_const);
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//指针加1，准备批量复制
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
///*---------------------------------float型数组-------------------------------------------*/
//void cross_page_copy_float_dest(float *dest , float *src,unsigned long int B)
//{
//	//B必须大于4！！
//	//1个float占2个地址！！
//	//使用指针赋值时，若对指针加减运算使得指针跨页则会发生错误，所以先转换成长整型，地址加减后再转成指针。要注意此处float指针+1，等效于地址+2！！！
//	long unsigned int add_const;
//	long unsigned int add_tmp;
//	add_const = (((long unsigned int)dest) & ((long unsigned int)0x000FFFFu));
//	//No.1
//	if(add_const == 0xFFFEu)//特殊地址：复制开始地址正好是本页最后一个地址
//	{
//		add_tmp	= (long unsigned int)src;//单独对0xFFFE赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//指针加1，准备批量复制
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-2)*sizeof(float));
//	}
//	//No.2
//	else if(add_const == 0x0000u)//特殊地址：复制开始地址正好是本页第一个地址
//	{
//		add_tmp	= (long unsigned int)src;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//指针加1，准备批量复制
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//		memcpy(dest,src,(B-1)*sizeof(float));
//	}
//	//No.3
//	else if((add_const + (B-1)*2) == 0x000FFFEu)//正好复制到本页的结尾，则结尾地址的数据通过单独赋值的方法实现复制
//	{
//		memcpy(dest,src,(B-1)*sizeof(float));//先复制，再单独赋值
//
//		add_tmp	= (long unsigned int)src + (B-1u)*2;
//		src 	= (float *)add_tmp;//单独对0xFFFE赋值
//
//		add_tmp	= (long unsigned int)dest + (B-1u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.4
//	else if(((add_const + (B-1)*2)&0x000FFFFu) == 0x0000u)//正好复制到下一页的第一个地址，则本页结尾地址与下一页开始地址的数据通过单独赋值的方法实现复制
//	{
//		memcpy(dest,src,(B-2)*sizeof(float));//先复制，再单独赋值
//
//		add_tmp	= (long unsigned int)src + (B-2u)*2;//单独对0xFFFE赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (B-2u)*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//	}
//	//No.5
//	else if(((add_const + (B-1)*2)&0x001FFFFu) > 0x10000u)//复制时发生了跨页赋值，则在当前页最后一个地址处分割，先复制到当前页结尾之前的一个数据，然后页结尾以及下一页页首单独赋值，最后赋值下一页数据
//	{
//		memcpy(dest,src,(0xFFFEu - (unsigned short)add_const)/2*sizeof(float));//先复制到0xFFFE
//
//		add_tmp	= (long unsigned int)src + (0xFFFEu - (unsigned short)add_const);//单独对0xFFFE赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + (0xFFFEu - (unsigned short)add_const);
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//单独对0x0000赋值
//		src 	= (float *)add_tmp;
//
//		add_tmp	= (long unsigned int)dest + 1u*2;
//		dest 	= (float *)add_tmp;
//
//		*dest 	= *src;
//
//		add_tmp	= (long unsigned int)src + 1u*2;//指针加1，准备批量复制
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


/*---------------------------------float型数组-------------------------------------------*/
void cross_page_copy_float(float *dest , float *src,unsigned long int B)
{//B表示待复制的float的个数，B要小于32768，即复制长度不能超过128KB（也就是一页）
	long unsigned int add_tail_1;
	long unsigned int add_tail_2;
	long unsigned int add_tail_3;
	long unsigned int add_tail_4;
	long unsigned int i;
	add_tail_1 = (((long unsigned int)src) & ((long unsigned int)0x000FFFFu));//取源地址低16位
	add_tail_2 = (((long unsigned int)dest) & ((long unsigned int)0x000FFFFu));//取目的地址低16位
	add_tail_3 = add_tail_1 + B*2;//取（源地址+复制长度）低16位
	add_tail_4 = add_tail_2 + B*2;//取（目的地址+复制长度）低16位
	if((add_tail_1 == 0xFFFEu)||(add_tail_1 == 0x0000u)||(add_tail_2 == 0xFFFEu)||(add_tail_2 == 0x0000u)||(add_tail_3 >= 0x10000u)||(add_tail_4 >= 0x10000u))//特殊地址
	//---------源地址在页尾---------------源地址在页首---------------目的地址在页尾----------------目的地址在页首------------源地址复制跨页----------------目的地址复制跨页-----//
	{//上述情况发生则采用数组方式访问数据
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

/*---------------------------------short型数组-------------------------------------------*/
void cross_page_copy_short(void *dest , void *src,unsigned long int B)
{//B表示待复制的short的个数，B要小于65536，即复制长度不能超过128KB（也就是一页）
	short *dest_short;
	short *src_short;
	dest_short 	= dest;	//将空指针所指向的地址，转移到短整型指针所指向的地址
	src_short 	= src;	//将空指针所指向的地址，转移到短整型指针所指向的地址
	long unsigned int add_tail_1;
	long unsigned int add_tail_2;
	long unsigned int add_tail_3;
	long unsigned int add_tail_4;
	long unsigned int i;
	add_tail_1 = (((long unsigned int)src) & ((long unsigned int)0x000FFFFu));//取源地址低16位
	add_tail_2 = (((long unsigned int)dest) & ((long unsigned int)0x000FFFFu));//取目的地址低16位
	add_tail_3 = add_tail_1 + B;//取（源地址+复制长度）低16位
	add_tail_4 = add_tail_2 + B;//取（目的地址+复制长度）低16位
	if((add_tail_1 == 0xFFFFu)||(add_tail_1 == 0x0000u)||(add_tail_2 == 0xFFFFu)||(add_tail_2 == 0x0000u)||(add_tail_3 >= 0x10000u)||(add_tail_4 >= 0x10000u))//特殊地址
		//---------源地址在页尾---------------源地址在页首---------------目的地址在页尾----------------目的地址在页首------------源地址复制跨页----------------目的地址复制跨页-----//
	{//上述情况发生则采用数组方式访问数据

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


