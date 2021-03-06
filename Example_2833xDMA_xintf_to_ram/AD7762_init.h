/*
 * AD7762_init.h
 *
 *  Created on: 2016-11-25
 *      Author: zhjl769
 */

#ifndef AD7762_INIT_H_
#define AD7762_INIT_H_



void delay_N_Iclk(unsigned long Num)
{
	unsigned long i;
	for (i = 0; i < Num; i++)
	{
		asm("  NOP");
	}
}


/*--------------------------初始化GPIO---------------------------------*/
void Gpio_select(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO    GPIO0~GPIO15
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO    GPIO16~GPIO31
	GpioCtrlRegs.GPAMUX1.bit.GPIO3=0X0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO4=0X0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO5=0X0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO6=0X0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO7=0X0;
	GpioCtrlRegs.GPBMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPBMUX2.all = 0x00000000; // B ALL GPIO    GPIO48~GPIO63
	GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 0x0;//DSPIN    IN
	GpioDataRegs.GPADAT.bit.GPIO5 =0x1;    //RESET  UP
	GpioDataRegs.GPADAT.bit.GPIO6 =0x1;    //SYNS  UP
	GpioCtrlRegs.GPBDIR.bit.GPIO36 = 0x1;    //CS    OUT
	GpioCtrlRegs.GPADIR.bit.GPIO3  =0X1;     //MCLK     OUT
	GpioCtrlRegs.GPADIR.bit.GPIO4  =0x0;      //DRDY    IN
	//    GpioCtrlRegs.GPADIR.bit.GPIO10 =0X0;    //I/O    (DS1302)    IN
	GpioCtrlRegs.GPADIR.bit.GPIO5 =0x1;     //RESET    OUT
	GpioCtrlRegs.GPADIR.bit.GPIO31 =0x1;     //RD/WR  OUT
	//    GpioCtrlRegs.GPADIR.bit.GPIO12 =0X1;    //CE    (DS1302)    OUT
	GpioCtrlRegs.GPADIR.bit.GPIO6 =0x1;     //SYNC   OUT
	GpioCtrlRegs.GPCMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPCMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPCDIR.all = 0x00000000;    //GPIOC   ALL INPUT
	//    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 0x1;    //CS1    OUT
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 0x1;     //MCLK     OUT
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0x1;     //MCLK  PWM
	EDIS;
}


void GPIO4_XINT_Init(void)
{

	EALLOW;
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    //使能PIE中断开关
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;    //使能XINT1的PIE级中断开关
	IER |= M_INT1; // Enable CPU 第一组中断
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0; //选择GPIO4是GPIO口
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;//选择GPIO4都是输入口
	GpioCtrlRegs.GPAQSEL1.bit.GPIO4= 0;//GPIO4时钟和系统时钟一样且支持GPIO
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 4;//外部中断1选择GPIO4
	XIntruptRegs.XINT1CR.bit.POLARITY= 0;//触发模式为下降沿触发
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;//使能中断
	EDIS;
}

void GPIO3_PWM_Init(Uint16 Freq)    //Freq为需要输出的频率值，单位为kHz
{
	Uint16 Tem = 60000/Freq;  //60000为系统的输入ePWM模块的时钟，如果更改HSPCLKDIV与CLKDIV的值后需要更改此处的值

	EALLOW;
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;    //设置GPIO2输出方向
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   //ePWM2A
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;    //设置GPIO3输出方向
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   //ePWM2B

	//时间基准寄存器
	EPwm2Regs.TBPRD = (Tem - 1);             // Period = 7499+1 个TB时钟周期
	EPwm2Regs.TBPHS.all = 0;            // 相位为零
	EPwm2Regs.TBCTR = 0;                  //计数器初始值为零

	// Set Compare values
	EPwm2Regs.CMPA.half.CMPA = Tem/2;
	EPwm2Regs.CMPB = Tem/2;

	//设置TBCLK=120M/2=60M
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;  //高速时间基准时钟预分频
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;     //时间基准时钟预分频

	//TBCTL为定时器控制寄存器 
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; //设置计数模式位为连续增计数模式，产生对称方波 
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE; // 将定时器相位使能位关闭 
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW; //映射寄存器SHADOW使能并配置映射寄存器为自动读写 
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //定时器时钟源选择，一共有四种时钟源

	//设定动作
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR; //增模式：计数器等于CPMA时ePWMA输出变低
	EPwm2Regs.AQCTLA.bit.PRD = AQ_SET; //增模式：计数器等于周期值时wPWMA输出变高
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR; //增模式：计数器等于CPMB时ePWMB输出变低
	EPwm2Regs.AQCTLB.bit.PRD = AQ_SET; //增模式：计数器等于周期值时ePWMB输出变高
	EDIS;

}



void AD7762_Write(int16 reg_addr, int16 reg_number)
{
	int16 tmp1 = 0x0000,tmp2 = 0x0000;
	tmp1 =
			((reg_addr&0x0001)<<15)|((reg_addr&0x0002)<<13)|((reg_addr&0x0004)<<11)
			|((reg_addr&0x0008)<<9)|((reg_addr&0x0010)<<7)|((reg_addr&0x0020)<<5)
			|((reg_addr&0x0040)<<3)|((reg_addr&0x0080)<<1)|((reg_addr&0x0100)>>1)
			|((reg_addr&0x0200)>>3)|((reg_addr&0x0400)>>5)|((reg_addr&0x0800)>>7)
			|((reg_addr&0x1000)>>9)|((reg_addr&0x2000)>>11)|((reg_addr&0x4000)>>13)|((reg_addr&0x8000)>>15);
	tmp2 =
			((reg_number&0x0001)<<15)|((reg_number&0x0002)<<13)|((reg_number&0x0004)<<11)
			|((reg_number&0x0008)<<9)|((reg_number&0x0010)<<7)|((reg_number&0x0020)<<5)
			|((reg_number&0x0040)<<3)|((reg_number&0x0080)<<1)|((reg_number&0x0100)>>1)
			|((reg_number&0x0200)>>3)|((reg_number&0x0400)>>5)|((reg_number&0x0800)>>7)
			|((reg_number&0x1000)>>9)|((reg_number&0x2000)>>11)|((reg_number&0x4000)>>13)|((reg_number&0x8000)>>15);
	EALLOW;
	GpioDataRegs.GPADAT.bit.GPIO5=0x1;    //拉高RESET
	delay_N_Iclk(100);
	GpioDataRegs.GPADAT.bit.GPIO5=0x0;    //延时8.3us后拉低RESET(从那看出延迟了6.6us,延迟1000隔指令周期？)
	delay_N_Iclk(100);
	GpioDataRegs.GPADAT.bit.GPIO5=0x1;    //保持8.3us后拉高RESET    中间延时的时间间隔可否再短一些，提高执行速度
	delay_N_Iclk(500);
	GpioCtrlRegs.GPCMUX1.all=0x0000;   //GPIO79-64作为GPIO口
	GpioCtrlRegs.GPCDIR.all = 0xffff;   // GPIO79-64作为 outputs
	GpioDataRegs.GPCDAT.all =0x0000;    //GPIO79-64作为0
	GpioCtrlRegs.GPCPUD.all = 0xffff;   //上拉电阻使能
	delay_N_Iclk(500);
	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	GpioDataRegs.GPADAT.bit.GPIO31=0x1;    //RD/WD 拉高
	delay_N_Iclk(3500);
	GpioDataRegs.GPCDAT.all |= tmp1;    //reg_addr寄存器地址赋予GPIO79-64；
	delay_N_Iclk(500);
	GpioDataRegs.GPBDAT.bit.GPIO36=0x0;    //CS 拉低
	delay_N_Iclk(3500);
	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	GpioDataRegs.GPCDAT.all =0x0000;    //GPIO79-64作为0
	delay_N_Iclk(3500);
	GpioDataRegs.GPCDAT.all |= tmp2;    //reg_number寄存器值赋予GPIO79-64；
	delay_N_Iclk(3500);
	GpioDataRegs.GPBDAT.bit.GPIO36=0x0;    //CS 拉低
	delay_N_Iclk(3500);
	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	GpioDataRegs.GPCDAT.all =0x0000;    //GPIO79-64作为0
	delay_N_Iclk(3500);
	GpioCtrlRegs.GPCDIR.all = 0x0000;   // GPIO79-64作为 intputs
	GpioCtrlRegs.GPCMUX1.all = 0x0000;   //GPIO79-64作为GPIO口
	GpioCtrlRegs.GPCDIR.all = 0x0000;   // GPIO79-64作为inputs
	GpioDataRegs.GPCDAT.all = 0x0000;    //GPIO79-64作为0
	GpioCtrlRegs.GPCPUD.all = 0xffff;   //上拉电阻使能
	GpioDataRegs.GPADAT.bit.GPIO31=0x1;    //RD/WD 拉高
	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	delay_N_Iclk(3500);
	EDIS;
}

int32 AD7762_Read(void)
{
	int32 tmp = 0x00000000;
	int16 tmp1 = 0x0000,tmp2 = 0x0000;
	EALLOW;
	//	GpioCtrlRegs.GPCMUX1.all = 0x0000;   //GPIO79-64作为GPIO口
	//	GpioCtrlRegs.GPCDIR.all = 0x0000;   // GPIO79-64作为inputs
	//	GpioDataRegs.GPCDAT.all = 0x0000;    //GPIO79-64作为0
	//	GpioCtrlRegs.GPCPUD.all = 0xffff;   //上拉电阻使能
	//	GpioDataRegs.GPADAT.bit.GPIO31=0x1;    //RD/WD 拉高
	//	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	GpioDataRegs.GPADAT.bit.GPIO31=0x0;    //RD/WD 拉低
	GpioDataRegs.GPBDAT.bit.GPIO36=0x0;    //CS 拉低
	delay_N_Iclk(5);
	tmp1 |= GpioDataRegs.GPCDAT.all;    //GPIO79-64的值赋予tmp1；
	delay_N_Iclk(5);
	GpioDataRegs.GPADAT.bit.GPIO31=0x1;    //RD/WD 拉高
	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	delay_N_Iclk(5);
	GpioDataRegs.GPADAT.bit.GPIO31=0x0;    //RD/WD 拉低
	GpioDataRegs.GPBDAT.bit.GPIO36=0x0;    //CS 拉低
	delay_N_Iclk(5);
	tmp2 |= GpioDataRegs.GPCDAT.all;    //GPIO79-64的值赋予tmp1；
	delay_N_Iclk(5);
	GpioDataRegs.GPADAT.bit.GPIO31=0x1;    //RD/WD 拉高
	GpioDataRegs.GPBDAT.bit.GPIO36=0x1;    //CS 拉高
	EDIS;

	tmp1 =
			((tmp1&0x0001)<<15)|((tmp1&0x0002)<<13)|((tmp1&0x0004)<<11)
			|((tmp1&0x0008)<<9)|((tmp1&0x0010)<<7)|((tmp1&0x0020)<<5)
			|((tmp1&0x0040)<<3)|((tmp1&0x0080)<<1)|((tmp1&0x0100)>>1)
			|((tmp1&0x0200)>>3)|((tmp1&0x0400)>>5)|((tmp1&0x0800)>>7)
			|((tmp1&0x1000)>>9)|((tmp1&0x2000)>>11)|((tmp1&0x4000)>>13)|((tmp1&0x8000)>>15);

	tmp2 =
			((tmp2&0x0001)<<15)|((tmp2&0x0002)<<13)|((tmp2&0x0004)<<11)
			|((tmp2&0x0008)<<9)|((tmp2&0x0010)<<7)|((tmp2&0x0020)<<5)
			|((tmp2&0x0040)<<3)|((tmp2&0x0080)<<1)|((tmp2&0x0100)>>1)
			|((tmp2&0x0200)>>3)|((tmp2&0x0400)>>5)|((tmp2&0x0800)>>7)
			|((tmp2&0x1000)>>9)|((tmp2&0x2000)>>11)|((tmp2&0x4000)>>13)|((tmp2&0x8000)>>15);

	tmp |= tmp1;
	tmp &= 0x0000ffff;
	tmp <<= 16;
	tmp |= (tmp2 & 0xff00);
	tmp >>= 8;
	return tmp;
}

#endif /* AD7762_INIT_H_ */
