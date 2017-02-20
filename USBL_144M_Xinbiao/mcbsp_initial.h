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
  ),                                       /* DMACSDP  */
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
  ),                                       /* DMACICR  */
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP接收数据寄存器
    0,                                     /* DMACSSAU */
    (DMA_AdrPtr)&Rev1,                      /* DMACDSAL *///接收数组
    0,                                     /* DMACDSAU */
    BUF_LEN_MAX,                                     /* DMACEN   *///接收数据长度
    1,                                     /* DMACFN   */
    0,                                     /* DMACFI  */
    0                                     /* DMACEI  */
};

/*DMA发送模式配置*/
DMA_Config  myconfigdma0 = { 
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,    /**/
    DMA_DMACSDP_DSTPACK_OFF,       /**/  
    DMA_DMACSDP_DST_PERIPH,         /*目的地址:外设总线型'_PERIPH'GPIO口*/
    DMA_DMACSDP_SRCBEN_NOBURST,    /**/
    DMA_DMACSDP_SRCPACK_OFF,       /**/
    DMA_DMACSDP_SRC_SARAM,         /*源地址EMIF*/
    DMA_DMACSDP_DATATYPE_16BIT     /*_16BIT:的通道传输的数据类型*/
  ),                                       /* DMACSDP  */
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_CONST,   /*目的地址为常数，源地址自加*/
    DMA_DMACCR_SRCAMODE_POSTINC,   /**/
    DMA_DMACCR_ENDPROG_OFF,         /**/
    DMA_DMACCR_REPEAT_OFF,         /**/
    DMA_DMACCR_AUTOINIT_ON,        /*传输结束后等待ENDPROG变为1进行下次传输*/
    DMA_DMACCR_EN_STOP,            /**/
    DMA_DMACCR_PRIO_HI,            /**/
    DMA_DMACCR_FS_ELEMENT,         /*单元同步*/
    DMA_DMACCR_SYNC_TIM2INT           /*定时器定时发送*/
  ),                                       /* DMACCR   */
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,       /*0*/
    DMA_DMACICR_LASTIE_OFF,        /*0*/
    DMA_DMACICR_FRAMEIE_ON,        /*帧中断*/
    DMA_DMACICR_FIRSTHALFIE_OFF,   /*0*/
    DMA_DMACICR_DROPIE_OFF,        /*0*/
    DMA_DMACICR_TIMEOUTIE_OFF      /*0*/
  ),                                      
    (DMA_AdrPtr)&Send_sig,                /* DMACSSAL *///CE2空间
    0,                     				 /* DMACSSAU */
    (DMA_AdrPtr)&IODATA,    
    0,                                     /* DMACDSAU */
    20000,                                   
    1,                                     /* DMACFN   */
    0,                                     /* DMACFI   */
    0                                      /* DMACEI   */
};


#define TIMER_CTRL0    TIMER_TCR_RMK(\
                      TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
                      TIMER_TCR_FUNC_OF(11),        /* FUNC   == 11外部时钟源 */ \
                      TIMER_TCR_TLB_RESET,         /* TLB    == 1 */ \
                      TIMER_TCR_SOFT_BRKPTNOW,     /* SOFT   == 0 */ \
                      TIMER_TCR_FREE_WITHSOFT,     /* FREE   == 0 */ \
                      TIMER_TCR_PWID_OF(0),        /* PWID   == 01 */ \
                      TIMER_TCR_ARB_RESET,         /* ARB    == 1 */ \
                      TIMER_TCR_TSS_STOP,         /* TSS    == 1 */ \
                      TIMER_TCR_CP_PULSE,          /* CP     == 0 */ \
                      TIMER_TCR_POLAR_HIGH,         /* POLAR  == 1 */ \
                      TIMER_TCR_DATOUT_0           /* DATOUT == 0 */ \
) 

#define TIMER_CTRL1    TIMER_TCR_RMK(\
                      TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
                      TIMER_TCR_FUNC_OF(01),        /* FUNC   == 01CPU时钟源 */ \
                      TIMER_TCR_TLB_RESET,         /* TLB    == 1 */ \
                      TIMER_TCR_SOFT_BRKPTNOW,     /* SOFT   == 0 */ \
                      TIMER_TCR_FREE_WITHSOFT,     /* FREE   == 0 */ \
                      TIMER_TCR_PWID_OF(0),        /* PWID   == 01 */ \
                      TIMER_TCR_ARB_RESET,         /* ARB    == 1 */ \
                      TIMER_TCR_TSS_STOP,         /* TSS    == 1 */ \
                      TIMER_TCR_CP_PULSE,          /* CP     == 0 */ \
                      TIMER_TCR_POLAR_HIGH,         /* POLAR  == 1 */ \
                      TIMER_TCR_DATOUT_0           /* DATOUT == 0 */ \
) 

TIMER_Config timCfg0 = {	//Timer0为100ms  外部时钟频率=10MHz  
   TIMER_CTRL0,               /* TCR0 */
   0xf423u,                  /* PRD0=62499 */
   0x000fu                    /* PRSC=15 */
};//定时100ms 

//change by wyh 
TIMER_Config timCfg1 = {	//Timer1为1uS
   TIMER_CTRL1,               /* TCR0 */
   0x008fu,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;*/
   0x0000                    /* PRSC */
};


//MMC控制器的初始化参数，用户可以更改
//change by wyh

MMC_SetupNative Init = {
    0,   /* 禁止MMC的DMA申请 */
    0,   /* Set level of edge detection for DAT3 pin        */
    0,   /* Determines if MMC goes IDLE during IDLE instr   */
    1,   /* Memory clk reflected on CLK Pin                 */
    17,  /* 23。MMC控制器的频率分频因子：Fmmc=Fsystem/(a+1),a为该处初始值，范围：1~255   91*/
    23,  /* 17。MMC控制器给SD卡的通信频率：Fsd=Fsystem/(2(a+1)(b+1)),a为上个值，b为该处初始值。范围：0~255 17*/
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

  

    hDmaSend0 = DMA_open(DMA_CHA0,DMA_OPEN_RESET);//DMA0用于GPIO发射

    myhDma = DMA_open(DMA_CHA2,DMA_OPEN_RESET);  //DMA2用于McBSP采集数据

	mhTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);//定时器0作为同步时钟
	mhTimer1 = TIMER_open(TIMER_DEV1, TIMER_OPEN_RESET);//定时器1作为发射信号同步事件

    DMA_config(myhDma,&myConfig);
    DMA_config(hDmaSend0,&myconfigdma0);
	
	TIMER_config(mhTimer0, &timCfg0);
	TIMER_config(mhTimer1, &timCfg1);
}




