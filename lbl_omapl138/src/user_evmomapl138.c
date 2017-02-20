#include "user_evmomapl138_header.h"

/*-----------------------------------------**
** 	用户使用的硬件初始化函数定义           **
** 	连接库文件：	evmomapl138_bsl.lib	   **
**-----------------------------------------*/

//用户定义延迟函数：单位11us
void USFUNC_delay(uint32_t num)
{
	int i;
	for(i = 300*num; i > 0; i--)
	{
		;
	}
}

void init_psc_user()
{
   //-----------------------------------------
   // PSC0, domain 0 - all modules, always on.
   //-----------------------------------------

   // configure the next state for psc0 modules.
   
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_EMIFA, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_SPI0, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_MMCSD0, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_AINTC, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_UART0, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_SCR0, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_SCR1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_SCR2, PSC_ENABLE);

   //-----------------------------------------
   // PSC1, domain 0 - all modules, always on.
   //-----------------------------------------

   // configure the next state for psc1 modules.
   
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_USB0, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_USB1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_GPIO, PSC_ENABLE);
//   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_HPI, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_EMAC, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MCASP0, PSC_ENABLE);
//    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_SATA, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_VPIF, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_SPI1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_I2C1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_UART1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_UART2, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MCBSP0, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MCBSP1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_LCDC, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_PWM, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MMCSD1, PSC_ENABLE);
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_RPI, PSC_ENABLE);
//    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_ECAP, PSC_ENABLE);
//    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_SCR8, PSC_ENABLE);
//    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_SCR7, PSC_ENABLE);
//    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_SCR12, PSC_ENABLE);
    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_SHRAM, PSC_ENABLE);
}

void open_EDMA_E4(CSL_Edma3ccRegs *edma3ccRegs)
{
	// Enable Channel 4 Event Register
	CSL_FINST(edma3ccRegs->EESR, EDMA3CC_EESR_E4, SET);
	// Enable Interrupts for Channel 4
	CSL_FINST(edma3ccRegs->IESR, EDMA3CC_IESR_I4, SET);
}

void close_EDMA_E4(CSL_Edma3ccRegs *edma3ccRegs)
{
	// Disable Channel 4 Event Register
	CSL_FINST(edma3ccRegs->IECR, EDMA3CC_IECR_I4, CLEAR);
	// Disable Interrupts for Channel 4
	CSL_FINST(edma3ccRegs->EECR, EDMA3CC_EECR_E4, CLEAR);
	
}

void init_clocks_user()
{
	uint32_t rtn;
	// unlock the system config registers.
	SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
	SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
   
	rtn = config_pll0(0,24,1,0,1,11,5);
	rtn |= config_pll1(24,1,0,1,2);
	//lock the system config registers.
	SYSCONFIG->KICKR[0] = KICK0R_LOCK;
	SYSCONFIG->KICKR[1] = KICK1R_LOCK;
}
									   
void evm_init_omapl138()			//硬件全局初始化，类似于没有GEL文件情况下
{
	// configure power, sysconifg, and clocks.
	init_psc_user();
	init_clocks_user();
}

void evm_init_omapl138_DDR2()		//DDR2使用初始化，类似于没有GEL文件情况下
{
   // unlock the system config registers and set the ddr 2x clock source.
   SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
   SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
   CLRBIT(SYSCONFIG->CFGCHIP[3], CLK2XSRC);
   //以上操作是完成步骤1,似乎PLL1已经打开了输出1

   // enable emif3a clock.实际就是DDR2的时钟源打开，完成步骤2
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_EMIF3A, PSC_ENABLE);
   
   // check if vtp calibration is enabled.
   //如果POWERDN位被置位，表示没有进入节能模式，VTP没有被初始化，需要进行一次初始化。
   if (CHKBIT(VTPIO_CTL, 0x00000040))
   {
      // vtp cal disabled, begin cal.

      // enable input buffer and vtp.
	  //设置IOPWRDN位
      SETBIT(VTPIO_CTL, 0x00004000);
	  //清除POWERDN位
      CLRBIT(VTPIO_CTL, 0x00000040);
      
      // pulse clrz to init vtp cal.配置CLKRZ标志位
      SETBIT(VTPIO_CTL, 0x00002000);
      CLRBIT(VTPIO_CTL, 0x00002000);
      SETBIT(VTPIO_CTL, 0x00002000);
      
      // poll ready bit to wait for cal to complete.等待READY位被置位
      while (!CHKBIT(VTPIO_CTL, 0x00008000)) {}
      
      // set lock and power save bits.
      SETBIT(VTPIO_CTL, 0x00000180);
   }

   // config ddr timing.
   DDR->DDRPHYCTL1 = 0x000000C4;//外部选通门模式、接收器关电、RL=4
   
   //编程SDCR位期望的值，在此时有以下特性
   //DDR2TERM1			=1(复位值，没更改)
   //BOOTUNLOCK			=1(更改，表示解锁)写某些位时必须将这一位置位
   //DDR2EN 			=1(复位值，没更改)
   //DDREN				=1(复位值，没更改)
   //SDRAMEN			=1(复位值，没更改)
   //TIMUNLOCK			=1(更改，表示解锁)写CL时必须将这一位置位
   //NM					=1(复位值，没更改)
   //CL					=3(期望值，与外部存储器芯片有关)
   //IBANK				=3(8bank，与外部存储器芯片有关)
   //PAGESIZE			=2(10bit列地址，与外部存储器芯片有关)
   DDR->SDCR = 0x0893C632;
   //修改DDR2EN为0值，此时BOOTUNLOCK为1表示解锁
   DDR->SDCR &= ((DDR->SDCR & 0xFF0FFFFF) | 0x00800000);
   //修改完DDR2EN为1值,此时BOOTUNLOCK为0表示修改成功，锁上
   DDR->SDCR = ((DDR->SDCR & 0xFF0FFFFF) | 0x00100000);
   //修改完DDR2控制器的CL值，此时TIMUNLOCK为0表示修改成功，锁上
   DDR->SDCR &= (~0x00008000);
   DDR->SDTIMR1 = 0x26492A09;
   DDR->SDTIMR2 = 0x7D14C722;
   DDR->SDCR2 = 0x0;
   DDR->SDRCR = 0x00000492;
   
   // set ddr2 to sync reset.
   SETBIT(DDR->SDRCR, 0xC0000000);
   
   // sync reset the ddr clock.
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_EMIF3A, PSC_SYNCRESET);
   
   // enable the clock.
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_EMIF3A, PSC_ENABLE);
   
   // disable self refresh.
   CLRBIT(DDR->SDRCR, 0xC0000000);
}


//timerP
void evm_init_timer(timer_regs_t *timer_x, uint32_t user_timer)
{
	uint32_t prd_timer;
	timer_x->TGCR = 0x00000000;             //初始化
	timer_x->TCR = 0x00000000;               //内部时钟,与PLL输入时钟相同，24MHz

	// interrupts and set emulation to free run.
	timer_x->INTCTLSTAT = 0;
	SETBIT(timer_x->EMUMGT, SOFT | FREE);

	// config timer0 in 32-bit unchained mode.      两个独立的定时器  timer 3:4 和timer 1:2
	// remove timer0 - 3:4 from reset.                   保持当前值
	SETBIT(timer_x->TGCR, TIMMODE_32BIT_UNCHAINED | TIM12RS );	
		
	// Reset the Counter for Timer0:12
	SETBIT(timer_x->TIM12, CSL_TMR_TIM12_TIM12_RESETVAL);

	// Select Internal Clock for Timer0:12 (24 MHz)
	//timer_x->TCR = 0x00000100;                      //External clock on TM64P_IN12
	// Set Timer0:12 Period :输入的时间为Xus，定时周期为1/24000000s，则写入值N = （X*10e-6）/（1/24000000） = X*24
	prd_timer = user_timer*24 - 1;
	SETBIT(timer_x->PRD12, prd_timer);

//	timer_x->INTCTLSTAT = 0x00000001;
	// Disable the New Timer Features 既不需要timer_x->INTCTLSTAT来设置中断
	SETBIT(timer_x->TGCR, (CSL_TMR_TGCR_PLUSEN_DISABLE<<CSL_TMR_TGCR_PLUSEN_SHIFT));
	//The timer is enabled one time. The timer stops after the counter reaches the period.
	SETBIT(timer_x->TCR, ENAMODE12_ONETIME);
}

//timerP2_12
void evm_init_timer_send(timer_regs_t *timer_x, uint32_t user_timer)
{
	uint32_t prd_timer;
	timer_x->TGCR = 0x00000000;
	timer_x->TCR = 0x00000000;

	// interrupts and set emulation to free run.
	timer_x->INTCTLSTAT = 0;
	SETBIT(timer_x->EMUMGT, SOFT | FREE);

	// config timer0 in 32-bit unchained mode.
	// remove timer0 - 1:2 from reset.
	SETBIT(timer_x->TGCR, TIMMODE_32BIT_UNCHAINED | TIM12RS );	
		
	// Reset the Counter for Timer0:12          没有prescaler
	SETBIT(timer_x->TIM12, CSL_TMR_TIM12_TIM12_RESETVAL);

	// Select 外部时钟 Clock for Timer0:12 (10MHz)
	timer_x->TCR = 0x00000100;
	// Set Timer0:12 Period :输入的时间为Xus，定时周期为1/10000000s，则写入值N = （X*10e-6）/（1/10000000） = X*10
	prd_timer = user_timer*10 - 1; //10->24是否正确？？？
	SETBIT(timer_x->PRD12, prd_timer);

//	timer_x->INTCTLSTAT = 0x00000001; //开启定时中断
	// Disable the New Timer Features
	SETBIT(timer_x->TGCR, (CSL_TMR_TGCR_PLUSEN_DISABLE<<CSL_TMR_TGCR_PLUSEN_SHIFT));
	
//	SETBIT(timer_x->TCR, ENAMODE12_CONT);  在发射程序里面打开定时器12
}

void evm_init_mcbsp()
{
	// enable the psc and config pinmux for mcasp1.
	EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MCBSP1, PSC_ENABLE);
	EVMOMAPL138_pinmuxConfig(PINMUX_MCBSP1_REG, PINMUX_MCBSP1_MASK, PINMUX_MCBSP1_VAL);

	// configure receive registers (32 bit, single phase, no delay).
	//配置数据格式：单段帧RPHASE=0，段2位bit30~21=0全配置为0
	//无数据格式处理MSB先传RCOMPAND=0，忽略不期望的帧同步信号RFIG=0
	//段1配置为主：数据延迟1bit接收，1帧接收一个字，一个字长16bit
 	//在第一个脉冲重新启动发送器后开始接收帧同步脉冲
	MCBSP1->RCR = 0x00010040;
	//数据的长度为16bit
	MCBSP1->XCR = 0x00000040;
	// 帧发送信号FSX采用内部产生FSXM=1，驱动AD按照采样率转换;  帧接收信号FSR采用外部BUSY信号驱动FSRM=0
	//发送接收时钟由内部控制CLKXM=1，CLKRM=1;
	//采用MCBSP外部输入时钟作为MCBSP采样率时钟发生器源SCLKME=0 mcbsp clksm=0 SCLKME=0 采用外部时钟源
	//根据AD时序，发送帧同步信号低有效FSXP=1；接收帧同步信号低有效FSRP=1；
	//因此设置发送数据在CLKX下降沿采样CLKXP=0
	//因此设置接收数据在CLKR下降沿采样CLKRP=0
	MCBSP1->PCR = 0x00000B0C;
}

void AD_stop()
{
	MCBSP1->SPCR = 0x00000000;
}

void AD_start()
{
	int i;
	// reset mcbsp.
	//MCBSP复位操作，所有禁止
	MCBSP1->SPCR = 0x00000000;

	// configure receive registers (32 bit, single phase, no delay).
	//配置数据格式：单段帧RPHASE=0，段2位bit30~21=0全配置为0
	//无数据格式处理MSB先传RCOMPAND=0，忽略不期望的帧同步信号RFIG=0
	//段1配置为主：数据延迟1bit接收，1帧接收一个字，一个字长16bit
	//在第一个脉冲重新启动发送器后开始接收帧同步脉冲
//	MCBSP1->RCR = 0x00010040;  //***//

	// configure transmit registers (32 bit, single phase, no delay).
	//配置数据格式：不使用，暂不配置
//	MCBSP1->XCR = 0x00000000;  //***//

	// Disable int frame generation and enable slave w/ext frame signals on FSX
	// Frame sync is active high, data clocked on rising edge of clkx
	// 帧发送信号FSX采用内部产生FSXM=1，驱动AD按照采样率转换;  帧接收信号FSR采用外部BUSY信号驱动FSRM=0
	//发送接收时钟由内部控制CLKXM=1，CLKRM=1;
	//采用MCBSP内部输入时钟作为MCBSP采样率时钟发生器源SCLKME=0
	//根据AD时序，发送帧同步信号低有效FSXP=1；接收帧同步信号低有效FSRP=1；
	//因此设置发送数据在CLKX下降沿采样CLKXP=0
	//因此设置接收数据在CLKR下降沿采样CLKRP=0
//	MCBSP1->PCR = 0x00000B0C;  //***//
	
   //mcbsp clksm=0 SCLKME=0 采用外部时钟源 CLKGDV=9 FWID=2  FPER=24
   //CLKG=CLKS/(CLKGDV+1)=1M  FPER=1M/(24+1)=40K  帧脉冲宽度=(FWID+1)*（1/40khz）=75us  
	MCBSP1->SRGR = 0x10180209;  //3    1M 40K
//	MCBSP1->SRGR = 0x3031021D;         // 5M   100kHz
//	MCBSP1->SRGR = 0x10090218;	//3		
//	for(i=0;i<20;i++){}			//4		600clock
	MCBSP1->SPCR = 0x00400000;	//5		
//	for(i=0;i<20;i++){};					//600clock

	SETBIT(MCBSP1->SPCR, 0x00010000);	//6             Serial port transmitter is enabled
	//必须等待一定的时间，先使能，再禁止，防止出现第一次误触发
	for(i=0;i<210;i++){}					//1200clock
	CLRBIT(MCBSP1->SPCR, 0x00010000);
	//完成这一步骤之后必须建立EDMA的使用
	open_EDMA_E4(Edma30cc_0_RegsOvly);		//7

//	MCBSP1->SPCR = 0x00410001;
	MCBSP1->SPCR = 0x00C10001;                               //The serial port receiver is enabled，Serial port transmitter is enabled

//	SETBIT(MCBSP1->SPCR, 0x00010001);	//8
//	SETBIT(MCBSP1->SPCR, 0x00800000);	//11
}

void init_PaRAM_event4 (CSL_Edma3ccRegs *edma3ccRegs)
{
	// Reset EDMA PaRAM OPT Register
	//复位EDMA的参数表4，让其值为0x00000000
	//所有的EDMA_EVENT4，换成EDMA_EVENT10
	edma3ccRegs->PARAMSET[EDMA_EVENT4].OPT = CSL_EDMA3CC_OPT_RESETVAL;
	
	// Config PaRAM OPT (Enable TC Interrupt; Set TCC)
	//然后配置参数表4的OPT寄存器：
	//配置传输完成中断使能，同时设置传输完成代码为TCC=4；
	//默认采用A同步模式，目的地址自增模式
	edma3ccRegs->PARAMSET[EDMA_EVENT4].OPT = 0x00104000;
	edma3ccRegs->PARAMSET[EDMA_EVENT4].SRC = 0x01D11000;//(uint32_t)&(MCBSP1->DDR)
	//0xC0080000;//设置固定的起始地址DDR2地址;
	// Set EDMA Event PaRAM A,B,C CNT
	//设置传输事件的传输特征，包括帧大小，传输格式
	//此次传输的格式如下：传输ACNT为2（2个字节长度），传输BCNT为512，传输CCNT为1
	//格式：    	(BCNT)|(ACNT)
	//A表示一次事件的单元个数，B表示阵列个数，C表示帧个数
//	edma3ccRegs->PARAMSET[EDMA_EVENT4].A_B_CNT = 0x03700002;  //880
	edma3ccRegs->PARAMSET[EDMA_EVENT4].A_B_CNT = 0x02000002;
	// Initialize EDMA Event Src and Dst Addresses
	//设置通道传输的源地址信息和目的地址信息
	edma3ccRegs->PARAMSET[EDMA_EVENT4].DST = (uint32_t)&buffer1;
	// Set EDMA Event PaRAM SRC/DST BIDX
	//设置源地址/目的地址的单元自加索引为2/0个字节:  (DSTBIDX)|(SRCBIDX)
	edma3ccRegs->PARAMSET[EDMA_EVENT4].SRC_DST_BIDX = 0x00020000;
	// Set EDMA Event PaRAM LINK and BCNTRLD
	//设置EDMA的参数表连接地址和BCNTRLD值
	//设置无连接，同时设置BCNTRLD为0
	edma3ccRegs->PARAMSET[EDMA_EVENT4].LINK_BCNTRLD = (1024<<16)|PaRAM_pang_74;
	// Set EDMA Event PaRAM SRC/DST CIDX
	//设置帧索引值为0
	edma3ccRegs->PARAMSET[EDMA_EVENT4].SRC_DST_CIDX = 0x00000000;
	edma3ccRegs->PARAMSET[EDMA_EVENT4].CCNT = 0x0001;	
}

void init_PaRAM_event74 (CSL_Edma3ccRegs *edma3ccRegs)
{
	// Reset EDMA PaRAM OPT Register
	//复位EDMA的参数表4，让其值为0x00000000
	edma3ccRegs->PARAMSET[EDMA_EVENT74].OPT = CSL_EDMA3CC_OPT_RESETVAL;
	
	// Config PaRAM OPT (Enable TC Interrupt; Set TCC)
	//然后配置参数表4的OPT寄存器：
	//配置传输完成中断使能，同时设置传输完成代码为TCC=4；
	//默认采用A同步模式，目的地址自增模式
	edma3ccRegs->PARAMSET[EDMA_EVENT74].OPT = 0x00104000;
	edma3ccRegs->PARAMSET[EDMA_EVENT74].SRC = 0x01D11000;//(uint32_t)&(MCBSP1->DDR);
	// Set EDMA Event PaRAM A,B,C CNT
	//设置传输事件的传输特征，包括帧大小，传输格式
	//此次传输的格式如下：传输ACNT为2，传输BCNT为1024，传输CCNT为1
	//格式：    	(BCNT)|(ACNT)
	//A表示一次事件的单元个数，B表示阵列个数，C表示帧个数
//	edma3ccRegs->PARAMSET[EDMA_EVENT74].A_B_CNT = 0x03700002;
	edma3ccRegs->PARAMSET[EDMA_EVENT74].A_B_CNT = 0x02000002;
	// Initialize EDMA Event Src and Dst Addresses
	//设置通道传输的源地址信息和目的地址信息
	edma3ccRegs->PARAMSET[EDMA_EVENT74].DST = (uint32_t)&buffer2;
	// Set EDMA Event PaRAM SRC/DST BIDX
	//设置源地址/目的地址的单元自加索引为2/0个字节:  (DSTBIDX)|(SRCBIDX)
	edma3ccRegs->PARAMSET[EDMA_EVENT74].SRC_DST_BIDX = 0x00020000;
	// Set EDMA Event PaRAM LINK and BCNTRLD
	//设置EDMA的参数表连接地址和BCNTRLD值
	//设置无连接，同时设置BCNTRLD为0
	edma3ccRegs->PARAMSET[EDMA_EVENT74].LINK_BCNTRLD = (1024<<16)|PaRAM_ping_75;
	// Set EDMA Event PaRAM SRC/DST CIDX
	//设置帧索引值为0
	edma3ccRegs->PARAMSET[EDMA_EVENT74].SRC_DST_CIDX = 0x00000000;
	edma3ccRegs->PARAMSET[EDMA_EVENT74].CCNT = 0x0001;
}

void init_PaRAM_event75 (CSL_Edma3ccRegs *edma3ccRegs)
{
	// Reset EDMA PaRAM OPT Register
	//复位EDMA的参数表4，让其值为0x00000000
	edma3ccRegs->PARAMSET[EDMA_EVENT75].OPT = CSL_EDMA3CC_OPT_RESETVAL;
	
	// Config PaRAM OPT (Enable TC Interrupt; Set TCC)
	//然后配置参数表4的OPT寄存器：
	//配置传输完成中断使能，同时设置传输完成代码为TCC=4；
	//默认采用A同步模式，目的地址自增模式
	edma3ccRegs->PARAMSET[EDMA_EVENT75].OPT = 0x00104000;
	edma3ccRegs->PARAMSET[EDMA_EVENT75].SRC = 0x01D11000;//(uint32_t)&(MCBSP1->DDR);
	// Set EDMA Event PaRAM A,B,C CNT
	//设置传输事件的传输特征，包括帧大小，传输格式
	//此次传输的格式如下：传输ACNT为2，传输BCNT为1024，传输CCNT为1
	//格式：    	(BCNT)|(ACNT)
	//A表示一次事件的单元个数，B表示阵列个数，C表示帧个数
//	edma3ccRegs->PARAMSET[EDMA_EVENT75].A_B_CNT = 0x03700002;
	edma3ccRegs->PARAMSET[EDMA_EVENT75].A_B_CNT = 0x02000002;
	// Initialize EDMA Event Src and Dst Addresses
	//设置通道传输的源地址信息和目的地址信息
	edma3ccRegs->PARAMSET[EDMA_EVENT75].DST = (uint32_t)&buffer1;
	// Set EDMA Event PaRAM SRC/DST BIDX
	//设置目的地址/源地址的单元自加索引为2/0个字节:  (DSTBIDX)|(SRCBIDX)
	edma3ccRegs->PARAMSET[EDMA_EVENT75].SRC_DST_BIDX = 0x00020000;
	// Set EDMA Event PaRAM LINK and BCNTRLD
	//设置EDMA的参数表连接地址和BCNTRLD值
	//设置无连接，设置BCNTRLD为0
	edma3ccRegs->PARAMSET[EDMA_EVENT75].LINK_BCNTRLD = (1024<<16)|PaRAM_pang_74;
	// Set EDMA Event PaRAM SRC/DST CIDX
	//设置帧索引值为0
	edma3ccRegs->PARAMSET[EDMA_EVENT75].SRC_DST_CIDX = 0x00000000;
	edma3ccRegs->PARAMSET[EDMA_EVENT75].CCNT = 0x0001;
}

void evm_init_edma_setup(CSL_Edma3ccRegs *edma3ccRegs)
{
	// Clear Event Registers

	//建立EDMA时间过程：
	//向第一ECR寄存器写0xffffffff，表示清除所有的DMA事件
	CSL_FINST(edma3ccRegs->ECR, EDMA3CC_ECR_REG, MASK);                 //->表示使用指针访问结构体成员
//	edma3ccRegs->ECR = 0x00000010;//清除通道4
	//向第二SECR寄存器写0xffffffff，表示清除所有的EDMA事件
	CSL_FINST(edma3ccRegs->SECR, EDMA3CC_SECR_REG, MASK);
	//因为有两个事件寄存器，因此需要写两个寄存器
//	edma3ccRegs->SECR = 0x00000010;		
	// Enable Channel 4 to DSP (Region 1)
	//使能EDMA3CC0的通道4用于DSP传输数据，采用影域1（shadow1域）
	//选择同步传输事件为MCBSP1接收事件E4，详细请参考OMAP文档关于EDMA事件描述部分
	edma3ccRegs->DRA[CSL_EDMA3_REGION_1].DRAE = 0x00000010;
	//edma3ccRegs->DRA[CSL_EDMA3_REGION_1].DRAE = 0x00000400;
	// Assign Channel 4 to Queue 0
	//使能通道队列0寄存器（DMAQNUM0）的E4通道队列事，且将Q0中
	edma3ccRegs->DMAQNUM[0] = 0x00000000;
	//edma3ccRegs->DMAQNUM[1] = 0x00000000;

	// Initialize PaRAM Transfer Context for Event 4

	//初始化参数表用于事件4传输。
	init_PaRAM_event4(edma3ccRegs);
	init_PaRAM_event74(edma3ccRegs);
	init_PaRAM_event75(edma3ccRegs);
		
	// Enable Channel 4 Event Register
//	CSL_FINST(edma3ccRegs->EESR, EDMA3CC_EESR_E4, SET);
	// Enable Interrupts for Channel 4
//	CSL_FINST(edma3ccRegs->IESR, EDMA3CC_IESR_I4, SET);
}

void evm_init_gpio_extern()
{
	//第一步：设置要的引脚功能
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO0H_REG, PINMUX_GPIO0H_MASK, PINMUX_GPIO0H_VAL);	//继电器 模拟增益
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO3H_REG, PINMUX_GPIO3H_MASK, PINMUX_GPIO3H_VAL);	//CPLD
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO6L_REG, PINMUX_GPIO6L_MASK, PINMUX_GPIO6L_VAL);	//CPLD 触发DSP  GPIO6[6] 输入
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO6H_REG, PINMUX_GPIO6H_MASK, PINMUX_GPIO6H_VAL);	//CPLD 
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO5L_REG, PINMUX_GPIO5L_MASK, PINMUX_GPIO5L_VAL);	//CPLD PWM
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO5H_REG, PINMUX_GPIO5H_MASK, PINMUX_GPIO5H_VAL);	//CPLD PWM


	//第二步：编程PSC模块使能GPIO功能
	EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_GPIO, PSC_ENABLE);

	//第三步：编程方向、数据、中断配置等寄存器
	//这里使能GPIOBANK[6]的GPIO6管脚作为外触发的输入
	*(unsigned volatile int *)GPIO_BINTEN = 0x00000040;//enable BANK6 Interrupt

	GPIO_setDir(GPIO_BANK0, GPIO_PIN13, GPIO_OUTPUT);		  //将GPIO0[13]配置为输出
	GPIO_setOutput(GPIO_BANK0, GPIO_PIN13, OUTPUT_LOW);	//增益54dB(0)  74dB(1)

	GPIO_BANK23->DIR = 0x00000000;       //所有引脚配置为输出
	GPIO_BANK45->DIR = 0x00000000;
	GPIO_BANK67->DIR = 0x00000040;		//CPLD触发DSP GPIO6[6]       0为输出  1为输入     GPIO6[6]为输入
	GPIO_BANK67->SET_RIS_TRIG = 0x00000040;       //GPIO6[6]设置为上升沿触发
	GPIO_BANK67->CLR_FAL_TRIG = 0x00000040;      //下降沿无影响
	GPIO_BANK67->IRQ_STAT = 0x00000040;               //????

	GPIO_setDir(GPIO_BANK5, TRANS_GPIO1, GPIO_OUTPUT);//设置GPIO5-9为输出模式
	GPIO_setDir(GPIO_BANK5, TRANS_GPIO2, GPIO_OUTPUT);//设置GPIO5-6为输出模式
	GPIO_setDir(GPIO_BANK5, TRANS_SYNC, GPIO_OUTPUT);//设置GPIO5-12为输出模式
/*	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO1, OUTPUT_LOW);//设置GPIO输出低电平
	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO2, OUTPUT_LOW);//设置GPIO输出低电平*/
	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO1, OUTPUT_HIGH);//设置GPIO输出高电平
	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO2, OUTPUT_HIGH);//设置GPIO输出高电平
	GPIO_setOutput(GPIO_BANK5,  TRANS_SYNC, OUTPUT_HIGH);//设置GPIO输出高电平
}

void transmit(uint16_t *signal, int n)
{
	int i,j;
	uint16_t temp1,temp2;

	SETBIT(TMR2->TCR, ENAMODE12_CONT);
	SETBIT(GPIO_BANK45->OUT_DATA, (0x10000000));;//设置GPIO输出高电平GPIO5[12]=1     PWM_Signal_Syn
	for( i =0; i < n; i++)
	{
		temp1 = signal[i]; 
		for(j = 0; j<16;j++)
		{
		   temp2 = (temp1>>j)&0x0001;
		   if(temp2)
		   {
			  while(!Trans_flag);
			  SETBIT(GPIO_BANK45->OUT_DATA, (0x02000000));       //GPIO5[9]=1              PWM_Signal_P
			  CLRBIT(GPIO_BANK45->OUT_DATA, (0x00400000));      //GPIO5[6]=0              PWM_Signal_N
			  Trans_flag = 0;
		   }
		   else
		   {
			  while(!Trans_flag);
			  CLRBIT(GPIO_BANK45->OUT_DATA, (0x02000000));
			  SETBIT(GPIO_BANK45->OUT_DATA, (0x00400000));
			  Trans_flag = 0;
		   }
		   
		}
	
	}
	while(!Trans_flag);
	Trans_flag = 0;
	CLRBIT(TMR2->TCR, ENAMODE12_CONT);
	CLRBIT(GPIO_BANK45->OUT_DATA, (0x02000000));//设置GPIO输出低电平
    CLRBIT(GPIO_BANK45->OUT_DATA, (0x00400000));//设置GPIO输出低电平
	CLRBIT(GPIO_BANK45->OUT_DATA, (0x10000000));//设置GPIO输出低电平
	TMR2->TIM12 = 0x00000000; 	
} 


void set_gp2_1()
{
//	GPIO_BANK23->OUT_DATA = 0x00000002;
}
void clear_gp2_1()
{
//	GPIO_BANK23->OUT_DATA = 0x00000000;
}



void DSP_interrupt_ARM()
{
    // unlock the system config registers.
    SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
    SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
   
	SYSCONFIG->CHIPSIG = 0x00000008;                   //Asserts SYSCFG_CHIPINT3 interrupt
	SYSCONFIG->CHIPSIG_CLR = 0x00000008;         //Clears SYSCFG_CHIPINT3 interrupt

    //lock the system config registers.
    SYSCONFIG->KICKR[0] = KICK0R_LOCK;
    SYSCONFIG->KICKR[1] = KICK1R_LOCK;   
}



void evm_init_uart(uart_regs_t *uart)
{
	uint32_t divisor;
	int baud_rate = 9600;

	// enable the psc and config pinmux for the given uart port.
	switch ((uint32_t)uart)
	{
		case UART0_REG_BASE:
			EVMOMAPL138_lpscTransition(PSC0, DOMAIN0, LPSC_UART0, PSC_ENABLE);
			EVMOMAPL138_pinmuxConfig(PINMUX_UART0_REG, PINMUX_UART0_MASK, PINMUX_UART0_VAL);
			break;
		 
		case UART1_REG_BASE:
			EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_UART1, PSC_ENABLE);
			EVMOMAPL138_pinmuxConfig(PINMUX_UART1_REG_0, PINMUX_UART1_MASK_0, PINMUX_UART1_VAL_0);
			EVMOMAPL138_pinmuxConfig(PINMUX_UART1_REG_1, PINMUX_UART1_MASK_1, PINMUX_UART1_VAL_1);
			break;

		case UART2_REG_BASE:
			EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_UART2, PSC_ENABLE);
			EVMOMAPL138_pinmuxConfig(PINMUX_UART2_REG_0, PINMUX_UART2_MASK_0, PINMUX_UART2_VAL_0);
			EVMOMAPL138_pinmuxConfig(PINMUX_UART2_REG_1, PINMUX_UART2_MASK_1, PINMUX_UART2_VAL_1);
			break;

		default:
			break;
	}
	
	// put xmtr/rcvr in reset.
	uart->PWREMU_MGMT = 0;
	
	// set baud rate...assumes default 16x oversampling.
	divisor = SYSCLOCK2_HZ / (baud_rate * 16);
	uart->DLH = (divisor & 0x0000FF00) >> 8;                //高8位
	uart->DLL = divisor & 0x000000FF;                           //低8位

	// enable xmtr/rcvr fifos.
/*	uart->FCR = 0;
	SETBIT(uart->FCR, FIFOEN);
	SETBIT(uart->FCR, UART_FCR_RXFIFTL_1);
	SETBIT(uart->FCR, RXCLR | TXCLR | DMAMODE1);
*/

	// ensable interrupts;disable flow control, and loop back.
	uart->IER = 0;
	SETBIT(uart->IER, UART_IER_ERBI);
	//uart->IER = 0;
	uart->MCR = 0;
	uart->MDR = 0;                         //?????

	// config LCR for no parity, one stop bit, 8 data bits, no flow control.
/*	uart->LCR = 0;
	SETBIT(uart->LCR, WLS_8);
*/
	// take xmtr/rcvr out of reset.
	SETBIT(uart->PWREMU_MGMT, UTRST | URRST | FREE);
	//SETBIT(uart->PWREMU_MGMT, UTRST | URRST); 
} 

