/*
 * Init_CLK.c
 *
 *  Created on: 2015-10-17
 *      Author: zhjl769
 */

#include <msp430f1611.h>

void Init_CLK(void)
{
    unsigned int i;
	//���Ĵ�������������
	//XT2��������
	//LFTX1�����ڵ�Ƶģʽ
	//ACLK�ķ�Ƶ����Ϊ1
    BCSCTL1 &= ~XT2OFF;
   // BCSCTL1 = 0X00;
    do
    {
	IFG1 &= ~OFIFG;                       // ���OSCFault��־
	for (i = 0x20; i > 0; i--);
    }
    while ((IFG1 & OFIFG) == OFIFG);      // ���OSCFault =1

    BCSCTL2 = 0X00;			//���Ĵ�������������
    BCSCTL2 += SELM1;		//MCLK��ʱ��ԴΪTX2CLK����Ƶ����Ϊ1
    BCSCTL2 |= SELS;		//SMCLK��ʱ��ԴΪTX2CLK����Ƶ����Ϊ1
}


