#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Variable_init.h"
#include "AD7762_init.h"
#include "Math_Function.h"

interrupt void XINT1_isr(void);
void Gpio_select(void);

void main(void)
{
	InitSysCtrl();
	DINT;    // Initialize the PIE control registers to their default state.
	InitPieCtrl();  // Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();  // Initialize the PIE vector table with pointers to the shell Interrupt
	Xcorr_refdata();
	Gpio_select();
	InitEPwm2Gpio();
	GPIO3_PWM_Init(1152); //GPIO3的PWM初始化.1.152MHz采样率为18kHz
	//	GPIO3_PWM_Init(2304); //GPIO3的PWM初始化.2.304MHz采样率为36kHz
	GPIO4_XINT_Init(); //XINt1中断初始化
	AD7762_Write(0x0002, 0x0003);        //AD7662控制寄存器2写入0x0003值；
	memset(DATABuf1, 0, BUF_SIZE);
	memset(DATABuf2, 0.0, BUF_SIZE);

	EALLOW;
	PieVectTable.XINT1 = &XINT1_isr;
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;    //使能XINT1的PIE级中断开关
	EDIS;
	IER |= M_INT1; // Enable CPU 第一组中断
	EINT; // Enable Global interrupt INTM
	ERTM; // Enable Global realtime interrupt DBGM

	while(1)
	{
		if(FLAG != 0)
		{
			int16 i;
			FLAG = 0;
			ad_signal_num = 0;
			AD7762_Write(0x0002, 0x000f);        //AD7662控制寄存器2写入0x0000值,停止AD采集
			DATA_Mean(DATABuf1, BUF_SIZE);
			memset(DATABuf2, 0.0, BUF_SIZE);
			for(i = 0; i < BUF_SIZE; i++)
				DATABuf2[i] =(float32)((float32)(DATABuf1[i])/8388608); //8388608为2^23,整型转化为浮点
			//			DATA_Gate(0x00500000, 16, DATABuf1, DATABuf2);
			memset(DATABuf4, 0.0, BUF_SIZE);
			xcorr(DATABuf3, Block_xcorr, DATABuf2, BUF_SIZE, DATABuf4);
			Piont1 = DATA_Max(DATABuf4, BUF_SIZE);
			//			Piont2 = DATA_Max(DATABuf5, BUF_SIZE);
			AD7762_Write(0x0002, 0x0003);        //AD7662控制寄存器2写入0x0003值；
		}
		else
			asm("  NOP");
	}
}


interrupt void XINT1_isr(void)
{
	DINT;
	DRTM;
	PieCtrlRegs.PIEIER1.bit.INTx4 = 0;    //关闭XINT1的PIE级中断开关
	PieCtrlRegs.PIEIFR1.bit.INTx4 = 0;    //XINT1的PIE级中断标志位清零

	if(ad_signal_num > 1023)
	{
		//		AD7762_Write(0x0002, 0x000f);        //AD7662控制寄存器2写入0x0000值,停止AD采集
		FLAG = 10;
		ad_signal_num = 0;
	}
	DATABuf1[ad_signal_num] = AD7762_Read();
	ad_signal_num++;

	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;    //XINT1的PIE级中断开关
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //PIEACK清0
	ERTM; // Enable Global realtime interrupt DBGM
	EINT;

}

