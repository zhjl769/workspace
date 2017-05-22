/*DSP的时钟配置*/

PLL_Config PLLconfig = {
		0,    //PLL锁定，与进入IDLE之前的状态保持一致， Initialize After Idle
		1,    // 时钟发生器切换到旁路模式，并重新开始PLL的锁相过程。Initialize On Break
		25,   //输入时钟乘以该值并除以PLLDIV就是时钟频率(范围2～31)。 PLL Multiply value
		1,   //0=不分频，1=2分频，2=三分频，3=四分频。 PLL Divide value
};

/*DSP的时钟配置(通信信号发送专用时钟192MHz，精确产生960kHz定时器中断)*/
PLL_Config PLLconfig_Tx = {
		0,    //PLL锁定，与进入IDLE之前的状态保持一致， Initialize After Idle
		1,    // 时钟发生器切换到旁路模式，并重新开始PLL的锁相过程。Initialize On Break
		24,   //输入时钟乘以该值并除以PLLDIV就是时钟频率(范围2～31)。 PLL Multiply value
		0,   //0=不分频，1=2分频，2=三分频，3=四分频。 PLL Divide value
};
/*DSP时钟*/

PLL_Config PLLconfig_I2C = {
		0,    //PLL锁定，与进入IDLE之前的状态保持一致， Initialize After Idle
		1,    // 时钟发生器切换到旁路模式，并重新开始PLL的锁相过程。Initialize On Break
		3,   //输入时钟乘以该值并除以PLLDIV就是时钟频率(范围2～31)。 PLL Multiply value48MHz
		1,   //0，不分频，1,2分频，2三分频，3四分频。 PLL Divide value
};
/*I2C配置*/
I2C_Config I2Cconfig = {
		0x0000, /* I2COAR */
		0xFFFF, /* I2CIER */
		0xFFFF, /* I2CSTR */
		20, /* I2CCLKL 6 */
		20, /* I2CCLKH 4 */
		0x00FF, /* I2CCNT */
		0x0050, /* I2CSAR */
		0x0020, /* I2CMDR */
		0xFFFF, /* I2CISRC */
		0x0009, /* I2CPSC  100/(I2CPSC + 1) = 5MHz*/
};
/*SDRAM初始化*/
EMIF_Config EMIFconfig = {
		0x0220, /* egcr */
		0xFFFF, /* emirst */
		0x112F, /* ce01 */
		0x00BB, /* ce02 */
		0x00FF, /* ce03 */
		0x112F, /* ce11 */
		0x00BB, /* ce12 */
		0x00FF, /* ce13 */
		0x3000, /* ce21 */
		0xFFFF, /* ce22 */ /*使用同步存储器时忽略这些bit*/
		0x00FF, /* ce23 */ /*使用同步存储器时忽略这些bit*/
		0x3000, /* ce31 */
		0xFFFF, /* ce32 */ /*使用同步存储器时忽略这些bit*/
		0x00FF, /* ce33 */ /*使用同步存储器时忽略这些bit*/
		0x3922, /* sdc1 */
		0x003D, /* sdper */
		0x0000, /* init */
		0x0255 /* sdc2 */
};

void EMIF_init(void)
{
	ioport unsigned int *sdc3  = (unsigned int *)0x0814;
	ioport unsigned int *ebsr  = (unsigned int *)0x6c00;//外部总线选择寄存器（控制HPI与EMIF）
	*ebsr = 0x0211;
	EMIF_reset();
	EMIF_config(&EMIFconfig);
	*sdc3 = 0x0003;/*若SDRAM时钟等于CPU时钟，sdc3=0x0007；若SDRAM时钟等于CPU时钟的分频，sdc3=0x0003*/
}

/* Create DMA Receive Side Configuration */
DMA_Config  myConfig = {
		DMA_DMACSDP_RMK(
				DMA_DMACSDP_DSTBEN_NOBURST,//地址不打包
				DMA_DMACSDP_DSTPACK_OFF,
				DMA_DMACSDP_DST_DARAM,//目的地址为DARAM
				DMA_DMACSDP_SRCBEN_NOBURST,
				DMA_DMACSDP_SRCPACK_OFF,
				DMA_DMACSDP_SRC_PERIPH,//源地址为外设
				DMA_DMACSDP_DATATYPE_16BIT//16bit数据
		),                                         /* DMACSDP  */
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_POSTINC,//目的地址自加
				DMA_DMACCR_SRCAMODE_CONST,//源地址为常数
				DMA_DMACCR_ENDPROG_OFF,
				DMA_DMACCR_REPEAT_OFF,
				DMA_DMACCR_AUTOINIT_ON,//传输结束后等待ENDPROG变为1进行下次传输
				DMA_DMACCR_EN_STOP,//禁止DMA传输
				DMA_DMACCR_PRIO_LOW,//优先级为低
				DMA_DMACCR_FS_DISABLE,//数据单元传输等待同步事件
				DMA_DMACCR_SYNC_REVT0//同步事件为McBSP0接收中断
		),                                       /* DMACCR   */
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,
				DMA_DMACICR_LASTIE_OFF,
				DMA_DMACICR_FRAMEIE_ON,//帧中断使能
				DMA_DMACICR_FIRSTHALFIE_OFF,
				DMA_DMACICR_DROPIE_OFF,
				DMA_DMACICR_TIMEOUTIE_OFF
		),
		(DMA_AdrPtr)(MCBSP_ADDR(DRR11)),  /* DMACSSAL *///McBSP接收数据寄存器
		0,                                /* DMACSSAU */
		(DMA_AdrPtr)&Rev1,                /* DMACDSAL *///接收数组
		0,                                /* DMACDSAU */
		BUF_LEN_MAX,                      /* DMACEN   *///接收数据长度
		1,                                /* DMACFN   */
		0,                                /* DMACFI   */
		0                                 /* DMACEI   */
};
DMA_Config  myconfigdma0 = { //发送配置
		DMA_DMACSDP_RMK(
				DMA_DMACSDP_DSTBEN_NOBURST,    /*目标地址突发模式：不使用*/
				DMA_DMACSDP_DSTPACK_OFF,       /*目标地址打包：不使用*/
				DMA_DMACSDP_DST_PERIPH,         /*选择目标类型：外设(GPIO)*/
				DMA_DMACSDP_SRCBEN_NOBURST,    /*源地址突发模式：不使用*/
				DMA_DMACSDP_SRCPACK_OFF,       /*源地址打包：不使用*/
				DMA_DMACSDP_SRC_EMIF,         /*选择源类型：EMIF（SDRAM）*/
				DMA_DMACSDP_DATATYPE_16BIT     /*通道传输的数据类型：16BIT*/
		),                                       /* DMACSDP寄存器*/
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_CONST,   /*目标地址寻址模式：常数地址*/
				DMA_DMACCR_SRCAMODE_POSTINC,   /*源地址寻址模式：地址自加*/
				DMA_DMACCR_ENDPROG_OFF,         /*编程结束标志位*/
				DMA_DMACCR_REPEAT_OFF,         /*循环条件：等待ENPROG=1*/
				DMA_DMACCR_AUTOINIT_ON,        /*自动初始化：开启*/
				DMA_DMACCR_EN_STOP,            /*通道使能控制位*/
				DMA_DMACCR_PRIO_HI,            /*通道优先级：高*/
				DMA_DMACCR_FS_ELEMENT,         /*同步类型：单元同步*/
				DMA_DMACCR_SYNC_TIM2INT           /*同步源：计数器2*/
		),                                       /* DMACCR寄存器*/
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,       /*所有块中断使能位*/
				DMA_DMACICR_LASTIE_OFF,        /*最后帧中断使能位*/
				DMA_DMACICR_FRAMEIE_ON,        /*所有帧中断使能位*/
				DMA_DMACICR_FIRSTHALFIE_OFF,   /*半帧中断使能位*/
				DMA_DMACICR_DROPIE_OFF,        /*同步事件旁落中断使能位*/
				DMA_DMACICR_TIMEOUTIE_OFF      /*超时中断使能位*/
		),                                      /* DMACICR寄存器*/
		(DMA_AdrPtr)&Send_sig,                	/* DMACSSAL源起始地址寄存器（低位） *///CE2空间
		0,                     				 	/* DMACSSAU源起始地址寄存器（高位）  */
		(DMA_AdrPtr)GPIO_ADDR(IODATA),          /* DMACDSAL目标起始地址寄存器（低位）  */
		0,                                    	/* DMACDSAU目标起始地址寄存器（高位）  */
		10000,		                          	/* DMACEN单元数寄存器   */
		1,                                     	/* DMACFN帧数寄存器   */
		0,                                     	/* DMACSFI源地址帧索引寄存器   */
		0,										/* DMACSEI源地址单元索引寄存器    */
		0,										/* DMACDFI目标地址帧索引寄存器 */
		0										/* DMACDEI目标地址单元索引寄存器       */
};

/*通信信号发送DMA配置*/
DMA_Config  dmaConfigForTxSend = { //发送配置
		DMA_DMACSDP_RMK(
				DMA_DMACSDP_DSTBEN_NOBURST,    /*目标地址突发模式：不使用*/
				DMA_DMACSDP_DSTPACK_OFF,       /*目标地址打包：不使用*/
				DMA_DMACSDP_DST_PERIPH,         /*选择目标类型：外设(GPIO)*/
				DMA_DMACSDP_SRCBEN_NOBURST,    /*源地址突发模式：不使用*/
				DMA_DMACSDP_SRCPACK_OFF,       /*源地址打包：不使用*/
				DMA_DMACSDP_SRC_DARAM,         /*选择源类型：EMIF（SDRAM）*/
				DMA_DMACSDP_DATATYPE_16BIT     /*通道传输的数据类型：16BIT*/
		),                                       /* DMACSDP寄存器*/
		DMA_DMACCR_RMK(
				DMA_DMACCR_DSTAMODE_CONST,   /*目标地址寻址模式：常数地址*/
				DMA_DMACCR_SRCAMODE_POSTINC,   /*源地址寻址模式：地址自加*/
				DMA_DMACCR_ENDPROG_OFF,         /*编程结束标志位*/
				DMA_DMACCR_REPEAT_OFF,         /*循环条件：等待ENPROG=1*/
				DMA_DMACCR_AUTOINIT_ON,        /*自动初始化：开启*/
				DMA_DMACCR_EN_STOP,            /*通道使能控制位*/
				DMA_DMACCR_PRIO_HI,            /*通道优先级：高*/
				DMA_DMACCR_FS_ELEMENT,         /*同步类型：单元同步*/
				DMA_DMACCR_SYNC_TIM2INT           /*同步源：计数器2*/
		),                                       /* DMACCR寄存器*/
		DMA_DMACICR_RMK(
				DMA_DMACICR_BLOCKIE_OFF,       /*所有块中断使能位*/
				DMA_DMACICR_LASTIE_OFF,        /*最后帧中断使能位*/
				DMA_DMACICR_FRAMEIE_ON,        /*所有帧中断使能位*/
				DMA_DMACICR_FIRSTHALFIE_OFF,   /*半帧中断使能位*/
				DMA_DMACICR_DROPIE_OFF,        /*同步事件旁落中断使能位*/
				DMA_DMACICR_TIMEOUTIE_OFF      /*超时中断使能位*/
		),                                      /* DMACICR寄存器*/
		(DMA_AdrPtr)&Sig_Tx_temp_1,                /* DMACSSAL源起始地址寄存器（低位） *///CE2空间
		0,                     				 /* DMACSSAU源起始地址寄存器（高位）  */
		(DMA_AdrPtr)GPIO_ADDR(IODATA),                   /* DMACDSAL目标起始地址寄存器（低位）  */
		0,                                     /* DMACDSAU目标起始地址寄存器（高位）  */
		480,		                           /* DMACEN单元数寄存器   */
		1,                                     /* DMACFN帧数寄存器   */
		0,                                     /* DMACSFI源地址帧索引寄存器   */
		0,										/* DMACSEI源地址单元索引寄存器    */
		0,										/* DMACDFI目标地址帧索引寄存器 */
		0										/* DMACDEI目标地址单元索引寄存器       */
};


#define TIMER_CTRL0    TIMER_TCR_RMK(\
		TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
		TIMER_TCR_FUNC_OF(01),       /* FUNC   == 01外部时钟源 */ \
		TIMER_TCR_TLB_RESET,         /* TLB    == 1 */ \
		TIMER_TCR_SOFT_BRKPTNOW,     /* SOFT   == 0 */ \
		TIMER_TCR_FREE_WITHSOFT,     /* FREE   == 0 */ \
		TIMER_TCR_PWID_OF(0),        /* PWID   == 01*/ \
		TIMER_TCR_ARB_RESET,         /* ARB    == 1 */ \
		TIMER_TCR_TSS_STOP,          /* TSS    == 1 */ \
		TIMER_TCR_CP_CLOCK,          /* CP     == 0 */ \
		TIMER_TCR_POLAR_HIGH,        /* POLAR  == 1 */ \
		TIMER_TCR_DATOUT_0           /* DATOUT == 0 */ \
		)

#define TIMER_CTRL1    TIMER_TCR_RMK(\
		TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
		TIMER_TCR_FUNC_OF(01),       /* FUNC   == 01CPU时钟源 */ \
		TIMER_TCR_TLB_RESET,         /* TLB    == 1 */ \
		TIMER_TCR_SOFT_BRKPTNOW,     /* SOFT   == 0 */ \
		TIMER_TCR_FREE_WITHSOFT,     /* FREE   == 0 */ \
		TIMER_TCR_PWID_OF(0),        /* PWID   == 01*/ \
		TIMER_TCR_ARB_RESET,         /* ARB    == 1 */ \
		TIMER_TCR_TSS_STOP,          /* TSS    == 1 */ \
		TIMER_TCR_CP_PULSE,          /* CP     == 0 */ \
		TIMER_TCR_POLAR_HIGH,        /* POLAR  == 1 */ \
		TIMER_TCR_DATOUT_0           /* DATOUT == 0 */ \
		)

TIMER_Config timCfg0 = {	//Timer0为100ms  外部时钟频率=10MHz
		TIMER_CTRL0,               /* TCR0 */
		0x1869u,                   /* PRD0=62499  10ms=0xf423 1ms=1869 */
		0x000fu                    /* PRSC=15 */
};//定时100ms

//change by wyh 
TIMER_Config timCfg1 = {	//Timer1为1uS
		TIMER_CTRL1,              /* TCR0 */
		0x0063u,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;100MHz->0x0063*/
		0x0000                    /* PRSC */
};

//MMC控制器的初始化参数，用户可以更改
//change by wyh
MMC_SetupNative Init = {
		0,   /* 禁止MMC的DMA申请 */
		0,   /* Set level of edge detection for DAT3 pin        */
		0,   /* Determines if MMC goes IDLE during IDLE instr   */
		1,   /* Memory clk reflected on CLK Pin                 */
		11,  /* 23。MMC控制器的频率分频因子：Fmmc=Fsystem/(a+1),a为该处初始值，范围：1~255   91*/
		18,  /* 17。MMC控制器给SD卡的通信频率：Fsd=Fsystem/(2(a+1)(b+1)),a为上个值，b为该处初始值。范围：0~255 17*/
		0,   /* No. memory clks to wait before response timeout */
		0,   /* No. memory clks to wait before data timeout     */
		512, /* 每个扇区为512字节（必须和SD的CSD中的数据相同）  */
};

void DMA_init(void)
{
	RevsrcAddrHi = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) >> 15) & 0xFFFFu;//McBSP接收寄存器高字节地址
	RevsrcAddrLo = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) << 1) & 0xFFFFu;//McBSP接收寄存器低字节地址
	Revdst1AddrHi = (Uint16)(((Uint32)(&Rev1)) >> 15) & 0xFFFFu;//重叠保留数组1高字节地址
	Revdst1AddrLo = (Uint16)(((Uint32)(&Rev1)) << 1) & 0xFFFFu;//重叠保留数组1低字节地址
	Revdst2AddrHi = (Uint16)(((Uint32)(&Rev2)) >> 15) & 0xFFFFu;//重叠保留数组2高字节地址
	Revdst2AddrLo = (Uint16)(((Uint32)(&Rev2)) << 1) & 0xFFFFu;//重叠保留数组2低字节地址

	DMA_DMAGSCR |= DMA_DMAGSCR_COMPMODE_OFF;

	myConfig.dmacssal = (DMA_AdrPtr)RevsrcAddrLo;//DMA通道源地址低字节
	myConfig.dmacssau = RevsrcAddrHi;//DMA通道源地址高字节
	myConfig.dmacdsal = (DMA_AdrPtr)Revdst1AddrLo;//DMA通道目的地址低字节
	myConfig.dmacdsau = Revdst1AddrHi;//DMA通道目的地址高字节

	sendsrc0AddrHi = (Uint16)(((Uint32)(myconfigdma0.dmacssal)) >> 15) & 0xFFFFu;//DMA0发射源地址高字节
	sendsrc0AddrLo = (Uint16)(((Uint32)(myconfigdma0.dmacssal)) << 1) & 0xFFFFu;//DMA0发射源地址低字节
	senddst0AddrHi = (Uint16)(((Uint32)(myconfigdma0.dmacdsal)) >> 15) & 0xFFFFu;//DMA0发射目的地址高字节
	senddst0AddrLo = (Uint16)(((Uint32)(myconfigdma0.dmacdsal)) << 1) & 0xFFFFu;//DMA0发射目的地址低字节

	myconfigdma0.dmacssal = (DMA_AdrPtr)sendsrc0AddrLo;//发射源地址EMIF
	myconfigdma0.dmacssau = sendsrc0AddrHi;
	myconfigdma0.dmacdsal = (DMA_AdrPtr)senddst0AddrLo;//发射目的地址GPIO
	myconfigdma0.dmacdsau = senddst0AddrHi;
	/*通信信号DMA1*/
	sendsrc1AddrHi = (Uint16)(((Uint32)(dmaConfigForTxSend.dmacssal)) >> 15) & 0xFFFFu;//DMA1发射源地址高字节
	sendsrc1AddrLo = (Uint16)(((Uint32)(dmaConfigForTxSend.dmacssal)) << 1) & 0xFFFFu;//DMA1发射源地址低字节
	senddst1AddrHi = (Uint16)(((Uint32)(dmaConfigForTxSend.dmacdsal)) >> 15) & 0xFFFFu;//DMA1发射目的地址高字节
	senddst1AddrLo = (Uint16)(((Uint32)(dmaConfigForTxSend.dmacdsal)) << 1) & 0xFFFFu;//DMA1发射目的地址低字节

	dmaConfigForTxSend.dmacssal = (DMA_AdrPtr)sendsrc1AddrLo;//发射源地址EMIF
	dmaConfigForTxSend.dmacssau = sendsrc1AddrHi;
	dmaConfigForTxSend.dmacdsal = (DMA_AdrPtr)senddst1AddrLo;//发射目的地址GPIO
	dmaConfigForTxSend.dmacdsau = senddst1AddrHi;

	hDmaSend0 	= DMA_open(DMA_CHA0,DMA_OPEN_RESET);	//DMA0用于GPIO发射
	hDmaSend1 	= DMA_open(DMA_CHA1,DMA_OPEN_RESET);	//DMA1用于通信信号发射
	myhDma 		= DMA_open(DMA_CHA2,DMA_OPEN_RESET);  	//DMA2用于McBSP采集数据
	hDmaSend3 	= DMA_open(DMA_CHA3,DMA_OPEN_RESET);	//DMA3用于CW信号发射

	mhTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);//定时器0作为同步时钟
	mhTimer1 = TIMER_open(TIMER_DEV1, TIMER_OPEN_RESET);//定时器1作为发射信号同步事件

	DMA_config(hDmaSend0,&myconfigdma0);		//加载DMA0的配置信息
	DMA_config(hDmaSend1,&dmaConfigForTxSend);	//加载DMA1的配置信息
	DMA_config(myhDma,&myConfig);				//加载DMA2的配置信息
	DMA_config(hDmaSend3,&myconfigdma0);		//加载DMA3的配置信息

	TIMER_config(mhTimer0, &timCfg0);
	TIMER_config(mhTimer1, &timCfg1);
}


/*//change by wyh 
  函数功能：初始化内部ADC时钟为2MHz	 采样率为10kHz
ADC Sample and Hold Period =  (ADC Clock Period) *(2 *(CONVRATEDIV + 1 + SAMPTIMEDIV))
 */
void InitADC()
{
	ADCCLKCTL = 0x17; // 4MHz ADCLK   ADC Clock = (CPU Clock)/(CPUCLKDIV + 1)
	ADCCLKDIV = 0x0ba00; //10kHz采样率
}

/*McBSP初始化配置  函数功能：初始化配置AD采样率为40kHz  */
//change by wyh，只需要改 ：SRGR1_0=0x0223
void McBSP_init()
{
	SPCR2_0 = 0x0000;
	SPCR1_0 = 0x0000;
	XCR2_0  = 0x0000;
	XCR1_0  = 0x0040;//XWDLEN1=16bit
	SRGR2_0 = 0x3063;//CLKSM=1，McBSP internal input clock；    帧同步信号周期为20;采样率为40kHz;FPER=99;CLKin=144M,CLKG=
	SRGR1_0 = 0x0218;//帧同步脉冲4个CLKG宽度;时钟分频次数为100//决定FWID,CLKGDV,  fs=40k,clk=200MHz,0231h
	PCR0    = 0x0b0d;//FSXM=1，帧同步由mcbsp产生;FSRM=0，接收同步由外部产生；CLKXM=CLKRM=1;FSXP=1，发射帧同步低有效；FSRP=1;CLKRP=CLKXP接收发射同源；
	RCR2_0  = 0x0005;//RWDLEN2=000；RCOMPAND=01；无压缩LSB先接收。RFIG=0；Frame-sync detect.
	RCR1_0  = 0x0040;//接收字长为16位
}  

/*串口接收数据初始化
  函数功能：串口初始化配置，波特率为38400
 */

void receive_752_init()
{
	UART_IER232 = 0x00;//清中断
	asm(" nop ");
	UART_LCR232 = 0x80;//DLAB=1,下步进行波特率设置
	asm(" nop ");
	UART_DLL232 = 0x18;//波特率设置0x60->9600
	asm(" nop ");    //波特率设置0x18->38400
	UART_DLH232 = 0x00;
	asm(" nop ");
	UART_LCR232 = 0x0bf;//设置其他寄存器前必须将LCR设置为0x0bf（参见芯片文档）
	asm(" nop ");
	UART_EFR232 = 0x10;//使能IER：bit4~7;FCR:bit4~5;MCR:bit5~7
	asm(" nop ");
	UART_LCR232 = 0x00;//还原LCR
	asm(" nop ");
	UART_FCR232 = 0x0ff;//初始化设置FIFO，使能，清零
	asm(" nop ");
	UART_MCR232 = 0x48;//
	asm(" nop ");
	UART_TCR232 = 0x03;//RCVFIFO设置为28字节停止发送
	asm(" nop ");
	UART_TLR232 = 0x05f;//RCVFIFO中断触发级别0x03*4=12，12个字节触发一次中断
	asm(" nop ");
	UART_LCR232 = 0x03;//设置数据格式：无奇偶校验，1比特停止位，8比特数据位
	asm(" nop ");
	UART_IER232 = 0x01;//使能FIFO接收中断
	asm(" nop ");
	//通道2初始化用于压力传感器传输通信，映射CE1空间
	UART_IER485 = 0x00;//清中断
	asm(" nop ");
	UART_LCR485 = 0x80;//DLAB=1,下步进行波特率设置
	asm(" nop ");
	UART_DLL485 = 0x60;//波特率设置0x18->38400
	asm(" nop ");    //0x60->9600
	UART_DLH485 = 0x00;//0x30->19200
	asm(" nop ");
	UART_LCR485 = 0x0bf;//设置其他寄存器前必须将LCR设置为0x0bf（参见芯片文档）
	asm(" nop ");
	UART_EFR485 = 0x10;//使能IER：bit4~7;FCR:bit4~5;MCR:bit5~7
	asm(" nop ");
	UART_LCR485 = 0x00;//还原LCR
	asm(" nop ");
	UART_FCR485 = 0x0ff;//初始化设置FIFO，使能，清零
	asm(" nop ");
	UART_MCR485 = 0x48;//
	asm(" nop ");
	UART_TCR485 = 0x06;//RCVFIFO中断级别触发停止发送
	asm(" nop ");
	UART_TLR485 = 0x03f;//RCVFIFO中断触发级别0x06*4=24（可能需要修改下）
	asm(" nop ");
	UART_LCR485 = 0x03;//设置数据格式：无奇偶校验，1比特停止位，8比特数据位
	asm(" nop ");
	UART_IER485 = 0x01;//使能FIFO接收中断
	asm(" nop ");
}
/*系统初始化配置
  函数功能：初始化时钟为200MHz; 串口配置在CE0、CE1空间; SDRAM配置在CE2、CE3空间
			中断向量表映射在0x000e;	初始化GPIO为输出，输出为低
 */
void System_initial()
{
	//CLKMD = 0x0000;
	//while(CLKMD & 0x0001){}
	//CLKMD = 0x2CF2;//clk = 8M; mult=50;div=1;50/2*8M=200M.CLKMD = 0x2c92 ;改144M CLKMD = 0x2912 bywyh,100M CLKMD = 0xCB2 bywyh
	//8MHz晶振，0x2CF2=50MHz
	//EBSR=0x0211;//设置为全部EMIF模式，无保持信号应答
	//EMI_RST=0x0000;
	//CE2和CE3为SDRAM
	/*
	EGCR = EGCR & 0x0ffdf;//SDRAM初始化
    EGCR = 0x020f;//CLKMEM=CPU时钟的一半；不响应外部总线征用请求
	CE2_1 = 0x3000;//16bit宽度SDRAM;
	CE3_1 = 0x3000;//16bit宽度SDRAM;
	SDC1 = 0x5958;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF对SDRAM进行刷新;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
//	SDC1 = 0x5858;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF对SDRAM进行刷新;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
	SDC2 = 0x028f;//SDACC=0,EMIF总线宽度为16bit;TMRD=2,tMRD=30ns;TRAS=8,tRAS=90ns;TACTV2ACTV=15,tRRD=160ns
    SDPER = 0x061a;//PERIOD=15620ns
    EGCR = EGCR|0x0020;//CLKMEM引脚输出时钟使能
    INIT = 0x0000;//初始化SDRAM
	 */
	//CE0和CE1初始化为异步串口 yuanshi
	/*
	CE0_1=0x103f;//16bit宽度异步存储器;读建立时间5ns;读选通时间75ns;读保持时间15ns
	CE0_2=0x00ff;//读延长保持时间5ns;写延长保持时间5ns;写建立时间5ns;写选通时间315ns;写保持时间15ns
	CE0_3=0x00ff;//超时时间为256个CPU时钟

	CE1_1=0x103f;//16bit宽度异步存储器;读建立时间5ns;读选通时间75ns;读保持时间15ns
	CE1_2=0x00ff;//读延长保持时间5ns;写延长保持时间5ns;写建立时间5ns;写选通时间315ns;写保持时间15ns
	CE1_3=0x00ff;//超时时间为256个CPU时钟
	 *///bywyh EMIF pdf clk=144MHz
	//CE0_1=0x112F;//16bit宽度异步存储器;读建立时间5ns;读选通时间75ns;读保持时间15ns
	//CE0_2=0x00BB;//读延长保持时间5ns;写延长保持时间5ns;写建立时间5ns;写选通时间315ns;写保持时间15ns
	//CE0_3=0x00ff;//超时时间为256个CPU时钟
	//CE1_1=0x112F;//16bit宽度异步存储器;读建立时间5ns;读选通时间75ns;读保持时间15ns
	//CE1_2=0x00BB;//读延长保持时间5ns;写延长保持时间5ns;写建立时间5ns;写选通时间315ns;写保持时间15ns
	//CE1_3=0x00ff;//超时时间为256个CPU时钟
	IVPH=0x000e;//中断向量表映射地址
	IVPD=0x000e;
	receive_752_init();//串口接收数据初始化
}
/******************************************************************
函数：void SD_WriteData(unsigned int SectorNumber,Uint16 *SendData)
功能：向指定的扇区中写入512字节的数据
输入参数：扇区号（0~根据卡的容量计算），写入数据的首地址
返回：无
 *******************************************************************/
void SD_WriteData(Uint32 SectorNumber,Uint16 *SendData)
{
	volatile Uint16 i,j;
	//asm(" BCLR XF ");//测试写入时间,第一个引号后必须有至少一个空格

	MMC_write(mmc1,512*SectorNumber,SendData,512);
	// busy_flag_0=MMCST0_1;
	//长延时，等待SD卡自编程结束(根据系统时钟频率和MMC控制器与SD卡通信时钟频率来决定延时）
	//下面的延时对于Fsd=666.7KHz ，Fsystem=192MHz的正常调试结果。（经验公式：当Fsd=500KHz ，Fsystem=24MHz时，以下延时至少9个）
	//经笔者调试情况看，此处延时很重要  272
	for(i=0;i<84;i++)
	{
		for (j = 0; j <= 25000; j++)
			asm("	NOP ");
	}

	//asm(" BSET XF");
}

/******************************************************************
函数：SD_ReadData(unsigned int SectorNumber,Uint16 *ReceiveData)
功能：向指定的扇区中读出512字节的数据
输入参数：扇区号（0~根据卡的容量计算），存储读出数据的首地址
返回：无
 *******************************************************************/
void SD_ReadData(Uint32 SectorNumber,Uint16 *ReceiveData)
{
	MMC_read(mmc1,512*SectorNumber,ReceiveData,512);
}

/*************************************************************************
函数：Uint16 ReadSectorNumber_Usable(void)
功能：从第2012159扇区中读出可用的扇区号(对应1G卡的倒数第二个扇区）
输入参数：无
返回：可用的扇区号
 ***************************************************************************/
Uint32 ReadSectorNumber_Usable(void)
{
	SD_ReadData(10000000,SecCounter);

	return((Uint32)((((Uint32)SecCounter[1])<<16)|(Uint32)SecCounter[0]));
}
/*************************************************************************
函数：void SaveSectorCounter(void)
功能：保存最后下一个空扇区号，保存在第2012159扇区中?以备下次接着该扇区写入数据
输入参数：无
返回：无
 ***************************************************************************/
void SaveSectorCounter(void)
{
	SecCounter[0]=(Uint16)SectorCounter;
	SecCounter[1]=(Uint16)(SectorCounter>>16);
	SD_WriteData(10000000,SecCounter);
}
/****************************************************
函数：void MMC_Init(void)
功能：初始化MMC控制器
输入参数：无
返回：无
 ****************************************************/
void MMC_Init(void)
{
	volatile short SD_jishu;
	//注：对于5509a的MMC控制器，只支持MMC卡和SD卡
	mmc1 = MMC_open(MMC_DEV1);//使能MMC0控制器
	MMC_setupNative(mmc1,&Init);//按照初始参数来初始化MMC0控制器

	MMC_sendGoIdle(mmc1);//复位总线上的说有存储卡
	for (count=0;count<4016;count++)
	{
		asm(" NOP ");
	}
	SD_jishu = 0;
	do
	{
		cardtype = MMC_sendOpCond(mmc1,0x00100000);//工作电压门限：3.2~3.3V，不同的卡对该命令的应答值不同，MMC控制器根据应答值来判断该存储卡类型
		SD_jishu++;
		if(SD_jishu == 10)
		{
			SD_status = 0;
			break;
		}
	}while(cardtype == 65535);

	if(cardtype == 0xffff)
	{
		SD_status = 0;
	}
	else
	{
		//检测到为SD卡
		cid = &cardid;
		SD_sendAllCID(mmc1,cid); // get the CID structure for all cards.
		card = &cardalloc;
		rca = SD_sendRca(mmc1,card);
		SD_status = 1;


		retVal = MMC_selectCard(mmc1,card);//选择MMC0来和SD卡进行通信

		SD_setWidth(mmc1, 0x4);//MMC控制器和SD卡的通信数据宽度为4bits
	}
}   

void uart_config()
{
	memset(SD_write,0,sizeof(short)*256);
	SD_write[0]  = 0x0ee; //校验位
	SD_write[1]  = RAMSES_ref_CIF;//公共询问
	SD_write[2]  = RAMSES_ref_IIF;//独立询问
	SD_write[3]  = (ID1) & 0x0ff; //信标标志频率1
	SD_write[4]  = (ID2) & 0x0ff; //信标标志频率2
	SD_write[5]  = (SNR) & 0x0ff; //检测SNR(待定)
	SD_write[6]  = (Maikuan_l) & 0x0ff; //相关峰检测下限(待定)
	SD_write[7]  = (Maikuan_h) & 0x0ff; //相关峰检测上限(待定)

	SD_write[8]  = 0x0dd; //校验位
	SD_write[9]  = 0x00;            //最小检测门限(待定)
	SD_write[10] = 0x0ff;           //命令码(待定)
	SD_write[11] = transponder_mode;//水下模式/电触发模式
	SD_write[12] = main_node;   	//相对测阵
	SD_write[13] = XDCZ_num;		//相对次数
	SD_write[14] = XDCZ_max_DMA_N;
	SD_write[15] = CIF_enable;

	SD_write[16] = 0x0cc; //校验位
	SD_write[17] = IIF_enable;
	SD_write[18] = ((Time_Delay/1000)) & 0x0ff;   //转发时延低8bit
	SD_write[19] = ((Time_Delay/1000)>>8) & 0x0ff;//转发时延高8bit
	SD_write[20] = responder_period/100;          //主站发射信号周期
	SD_write[21] = ((responder_Delay/1000)) & 0x0ff;       //电触发转发时延低8bit
	SD_write[22] = ((responder_Delay/1000)>>8) & 0x0ff;    //电触发转发时延高8bit
	SD_write[23] = ((Uint16)(temperature/0.5)) & 0xff;     //温度量化值低8bit

	SD_write[24] = 0x0bb; //校验位
	SD_write[25] = (((Uint16)(temperature/0.5))>>8) & 0xff;//温度量化值高8bit
	SD_write[26] = SD_status;//SD卡状态，1为正常，0为不正常
	SD_write[27] = (voltage_AD) & 0x0ff;          //电压量化值低8bit
	SD_write[28] = (voltage_AD>>8) & 0x0ff;       //电压量化值高8bit
	SD_write[29] = (RS485_DATA[5]) & 0xff;        //
	SD_write[30] = ((RS485_DATA[4])) & 0xff;      //
	SD_write[31] = ((RS485_DATA[3])) & 0xff;      //

	SD_write[32] = 0x0aa; //校验位
	SD_write[33] = ((RS485_DATA[2])) & 0xff;      //
	SD_write[34] = (Xcorr_max_val) & 0x0ff;       //相关峰最大值低8位
	SD_write[35] = (Xcorr_max_val>>8) & 0x0ff;    //相关峰最大值8位
	SD_write[36] = (width_save) & 0x0ff;          //相关峰宽度低8位
	SD_write[37] = (width_save>>8) & 0x0ff;       //相关峰宽度高8位
	SD_write[38] = (NOISE_TH_huadong) & 0x0ff;    //相关峰的滑动绝对门限
	SD_write[39] = (NOISE_TH_huadong>>8) & 0x0ff; //相关峰的滑动绝对门限
	SD_write[40] = (NOISE_TH_NEW) & 0x0ff;        //相关峰绝对门限
	SD_write[41] = (NOISE_TH_NEW>>8) & 0x0ff;     //相关峰绝对门限
	//	SD_write[42] = 0;
	//	SD_write[43] = 0;

}
