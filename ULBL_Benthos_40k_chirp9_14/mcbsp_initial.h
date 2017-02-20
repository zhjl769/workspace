/* Create DMA Receive Side Configuration */
DMA_Config  myConfig = { 
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,//µØÖ·²»´ò°ü
    DMA_DMACSDP_DSTPACK_OFF,
    DMA_DMACSDP_DST_DARAM,//Ä¿µÄµØÖ·ÎªDARAM
    DMA_DMACSDP_SRCBEN_NOBURST,
    DMA_DMACSDP_SRCPACK_OFF,
    DMA_DMACSDP_SRC_PERIPH,//Ô´µØÖ·ÎªÍâÉè
    DMA_DMACSDP_DATATYPE_16BIT//16bitÊý¾Ý
  ),                                       /* DMACSDP  */
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_POSTINC,//Ä¿µÄµØÖ·×Ô¼Ó
    DMA_DMACCR_SRCAMODE_CONST,//Ô´µØÖ·Îª³£Êý
    DMA_DMACCR_ENDPROG_OFF,
    DMA_DMACCR_REPEAT_OFF,
    DMA_DMACCR_AUTOINIT_ON,//´«Êä½áÊøºóµÈ´ýENDPROG±äÎª1½øÐÐÏÂ´Î´«Êä
    DMA_DMACCR_EN_STOP,//½ûÖ¹DMA´«Êä
    DMA_DMACCR_PRIO_LOW,//ÓÅÏÈ¼¶ÎªµÍ
    DMA_DMACCR_FS_DISABLE,//Êý¾Ýµ¥Ôª´«ÊäµÈ´ýÍ¬²½ÊÂ¼þ
    DMA_DMACCR_SYNC_REVT0//Í¬²½ÊÂ¼þÎªMcBSP0½ÓÊÕÖÐ¶Ï
  ),                                       /* DMACCR   */
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,
    DMA_DMACICR_LASTIE_OFF,
    DMA_DMACICR_FRAMEIE_ON,//Ö¡ÖÐ¶ÏÊ¹ÄÜ
    DMA_DMACICR_FIRSTHALFIE_OFF,
    DMA_DMACICR_DROPIE_OFF,
    DMA_DMACICR_TIMEOUTIE_OFF
  ),                                       /* DMACICR  */
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP½ÓÊÕÊý¾Ý¼Ä´æÆ÷
    0,                                     /* DMACSSAU */
    (DMA_AdrPtr)&Rev1,                      /* DMACDSAL *///½ÓÊÕÊý×é
    0,                                     /* DMACDSAU */
    BUF_LEN_MAX,                                     /* DMACEN   *///½ÓÊÕÊý¾Ý³¤¶È
    1,                                     /* DMACFN   */
    0,                                     /* DMACFI  */
    0                                     /* DMACEI  */
};

/*DMA·¢ËÍÄ£Ê½ÅäÖÃ*/
DMA_Config  myconfigdma0 = { 
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,    /**/
    DMA_DMACSDP_DSTPACK_OFF,       /**/  
    DMA_DMACSDP_DST_PERIPH,         /*Ä¿µÄµØÖ·:ÍâÉè×ÜÏßÐÍ'_PERIPH'GPIO¿Ú*/
    DMA_DMACSDP_SRCBEN_NOBURST,    /**/
    DMA_DMACSDP_SRCPACK_OFF,       /**/
    DMA_DMACSDP_SRC_SARAM,         /*Ô´µØÖ·EMIF*/
    DMA_DMACSDP_DATATYPE_16BIT     /*_16BIT:µÄÍ¨µÀ´«ÊäµÄÊý¾ÝÀàÐÍ*/
  ),                                       /* DMACSDP  */
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_CONST,   /*Ä¿µÄµØÖ·Îª³£Êý£¬Ô´µØÖ·×Ô¼Ó*/
    DMA_DMACCR_SRCAMODE_POSTINC,   /**/
    DMA_DMACCR_ENDPROG_OFF,         /**/
    DMA_DMACCR_REPEAT_OFF,         /**/
    DMA_DMACCR_AUTOINIT_ON,        /*´«Êä½áÊøºóµÈ´ýENDPROG±äÎª1½øÐÐÏÂ´Î´«Êä*/
    DMA_DMACCR_EN_STOP,            /**/
    DMA_DMACCR_PRIO_HI,            /**/
    DMA_DMACCR_FS_ELEMENT,         /*µ¥ÔªÍ¬²½*/
    DMA_DMACCR_SYNC_TIM2INT           /*¶¨Ê±Æ÷¶¨Ê±·¢ËÍ*/
  ),                                       /* DMACCR   */
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,       /*0*/
    DMA_DMACICR_LASTIE_OFF,        /*0*/
    DMA_DMACICR_FRAMEIE_ON,        /*Ö¡ÖÐ¶Ï*/
    DMA_DMACICR_FIRSTHALFIE_OFF,   /*0*/
    DMA_DMACICR_DROPIE_OFF,        /*0*/
    DMA_DMACICR_TIMEOUTIE_OFF      /*0*/
  ),                                      
    (DMA_AdrPtr)&Send_sig,                /* DMACSSAL *///CE2¿Õ¼ä
    0,                     				 /* DMACSSAU */
    (DMA_AdrPtr)&IODATA,    
    0,                                     /* DMACDSAU */
    25000,                                   
    1,                                     /* DMACFN   */
    0,                                     /* DMACFI   */
    0                                      /* DMACEI   */
};
#define TIMER_CTRL0    TIMER_TCR_RMK(\
                      TIMER_TCR_IDLEEN_DEFAULT,    /* IDLEEN == 0 */ \
                      TIMER_TCR_FUNC_OF(11),        /* FUNC   == 11Íâ²¿Ê±ÖÓÔ´ */ \
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
                      TIMER_TCR_FUNC_OF(01),        /* FUNC   == 01CPUÊ±ÖÓÔ´ */ \
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

TIMER_Config timCfg0 = {	//Timer0Îª100ms  Íâ²¿Ê±ÖÓÆµÂÊ=10MHz  
   TIMER_CTRL0,               /* TCR0 */
   0xf423u,                  /* PRD0=62499 */
   0x000fu                    /* PRSC=15 */
};//¶¨Ê±100ms 

//change by wyh 
TIMER_Config timCfg1 = {	//Timer1Îª1uS
   TIMER_CTRL1,               /* TCR0 */
   0x0063u,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;100MHz->0x0063*/
   0x0000                    /* PRSC */
};

//MMC¿ØÖÆÆ÷µÄ³õÊ¼»¯²ÎÊý£¬ÓÃ»§¿ÉÒÔ¸ü¸Ä
//change by wyh
MMC_SetupNative Init = {
    0,   /* ½ûÖ¹MMCµÄDMAÉêÇë */
    0,   /* Set level of edge detection for DAT3 pin        */
    0,   /* Determines if MMC goes IDLE during IDLE instr   */
    1,   /* Memory clk reflected on CLK Pin                 */
    11,  /* 23¡£MMC¿ØÖÆÆ÷µÄÆµÂÊ·ÖÆµÒò×Ó£ºFmmc=Fsystem/(a+1),aÎª¸Ã´¦³õÊ¼Öµ£¬·¶Î§£º1~255   91*/
    18,  /* 17¡£MMC¿ØÖÆÆ÷¸øSD¿¨µÄÍ¨ÐÅÆµÂÊ£ºFsd=Fsystem/(2(a+1)(b+1)),aÎªÉÏ¸öÖµ£¬bÎª¸Ã´¦³õÊ¼Öµ¡£·¶Î§£º0~255 17*/
    0,   /* No. memory clks to wait before response timeout */
    0,   /* No. memory clks to wait before data timeout     */
    512, /* Ã¿¸öÉÈÇøÎª512×Ö½Ú£¨±ØÐëºÍSDµÄCSDÖÐµÄÊý¾ÝÏàÍ¬£©  */
  };

void DMA_init(void)
{
	RevsrcAddrHi = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) >> 15) & 0xFFFFu;//McBSP½ÓÊÕ¼Ä´æÆ÷¸ß×Ö½ÚµØÖ·
    RevsrcAddrLo = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) << 1) & 0xFFFFu;//McBSP½ÓÊÕ¼Ä´æÆ÷µÍ×Ö½ÚµØÖ·
    Revdst1AddrHi = (Uint16)(((Uint32)(&Rev1)) >> 15) & 0xFFFFu;//ÖØµþ±£ÁôÊý×é1¸ß×Ö½ÚµØÖ·
    Revdst1AddrLo = (Uint16)(((Uint32)(&Rev1)) << 1) & 0xFFFFu;//ÖØµþ±£ÁôÊý×é1µÍ×Ö½ÚµØÖ·
	Revdst2AddrHi = (Uint16)(((Uint32)(&Rev2)) >> 15) & 0xFFFFu;//ÖØµþ±£ÁôÊý×é2¸ß×Ö½ÚµØÖ·
    Revdst2AddrLo = (Uint16)(((Uint32)(&Rev2)) << 1) & 0xFFFFu;//ÖØµþ±£ÁôÊý×é2µÍ×Ö½ÚµØÖ·

    myConfig.dmacssal = (DMA_AdrPtr)RevsrcAddrLo;//DMAÍ¨µÀÔ´µØÖ·µÍ×Ö½Ú
    myConfig.dmacssau = RevsrcAddrHi;//DMAÍ¨µÀÔ´µØÖ·¸ß×Ö½Ú
    myConfig.dmacdsal = (DMA_AdrPtr)Revdst1AddrLo;//DMAÍ¨µÀÄ¿µÄµØÖ·µÍ×Ö½Ú
    myConfig.dmacdsau = Revdst1AddrHi;//DMAÍ¨µÀÄ¿µÄµØÖ·¸ß×Ö½Ú

	sendsrc0AddrHi = (Uint16)(((Uint32)(myconfigdma0.dmacssal)) >> 15) & 0xFFFFu;//DMA0·¢ÉäÔ´µØÖ·¸ß×Ö½Ú
    sendsrc0AddrLo = (Uint16)(((Uint32)(myconfigdma0.dmacssal)) << 1) & 0xFFFFu;//DMA0·¢ÉäÔ´µØÖ·µÍ×Ö½Ú
    senddst0AddrHi = (Uint16)(((Uint32)(myconfigdma0.dmacdsal)) >> 15) & 0xFFFFu;//DMA0·¢ÉäÄ¿µÄµØÖ·¸ß×Ö½Ú
    senddst0AddrLo = (Uint16)(((Uint32)(myconfigdma0.dmacdsal)) << 1) & 0xFFFFu;//DMA0·¢ÉäÄ¿µÄµØÖ·µÍ×Ö½Ú

    myconfigdma0.dmacssal = (DMA_AdrPtr)sendsrc0AddrLo;//·¢ÉäÔ´µØÖ·EMIF
    myconfigdma0.dmacssau = sendsrc0AddrHi;
    myconfigdma0.dmacdsal = (DMA_AdrPtr)senddst0AddrLo;//·¢ÉäÄ¿µÄµØÖ·GPIO
    myconfigdma0.dmacdsau = senddst0AddrHi;

    hDmaSend0 = DMA_open(DMA_CHA0,DMA_OPEN_RESET);//DMA0ÓÃÓÚGPIO·¢Éä

    myhDma = DMA_open(DMA_CHA2,DMA_OPEN_RESET);  //DMA2ÓÃÓÚMcBSP²É¼¯Êý¾Ý

	mhTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);//¶¨Ê±Æ÷0×÷ÎªÍ¬²½Ê±ÖÓ
	mhTimer1 = TIMER_open(TIMER_DEV1, TIMER_OPEN_RESET);//¶¨Ê±Æ÷1×÷Îª·¢ÉäÐÅºÅÍ¬²½ÊÂ¼þ

    DMA_config(myhDma,&myConfig);
    DMA_config(hDmaSend0,&myconfigdma0);
	
	TIMER_config(mhTimer0, &timCfg0);
	TIMER_config(mhTimer1, &timCfg1);
}


/*//change by wyh 
  º¯Êý¹¦ÄÜ£º³õÊ¼»¯ÄÚ²¿ADCÊ±ÖÓÎª2MHz	 ²ÉÑùÂÊÎª10kHz
ADC Sample and Hold Period =  (ADC Clock Period) *(2 *(CONVRATEDIV + 1 + SAMPTIMEDIV))
*/
void InitADC()
{
	ADCCLKCTL = 0x17; // 4MHz ADCLK   ADC Clock = (CPU Clock)/(CPUCLKDIV + 1)
	ADCCLKDIV = 0x0ba00; //10kHz²ÉÑùÂÊ
}
  
/*McBSP³õÊ¼»¯ÅäÖÃ  º¯Êý¹¦ÄÜ£º³õÊ¼»¯ÅäÖÃAD²ÉÑùÂÊÎª40kHz  */
//change by wyh£¬Ö»ÐèÒª¸Ä £ºSRGR1_0=0x0223  
void McBSP_init()
{
	SPCR2_0 = 0x0000;
	SPCR1_0 = 0x0000;
	XCR2_0  = 0x0000;
	XCR1_0  = 0x0040;//XWDLEN1=16bit
	SRGR2_0 = 0x3063;//CLKSM=1£¬McBSP internal input clock£»    Ö¡Í¬²½ÐÅºÅÖÜÆÚÎª20;²ÉÑùÂÊÎª40kHz;FPER=99;CLKin=144M,CLKG= 
	SRGR1_0 = 0x0218;//Ö¡Í¬²½Âö³å4¸öCLKG¿í¶È;Ê±ÖÓ·ÖÆµ´ÎÊýÎª100//¾ö¶¨FWID,CLKGDV,  fs=40k,clk=200MHz,0231h
	PCR0    = 0x0b0d;//FSXM=1£¬Ö¡Í¬²½ÓÉmcbsp²úÉú;FSRM=0£¬½ÓÊÕÍ¬²½ÓÉÍâ²¿²úÉú£»CLKXM=CLKRM=1;FSXP=1£¬·¢ÉäÖ¡Í¬²½µÍÓÐÐ§£»FSRP=1;CLKRP=CLKXP½ÓÊÕ·¢ÉäÍ¬Ô´£»
	RCR2_0  = 0x0005;//RWDLEN2=000£»RCOMPAND=01£»ÎÞÑ¹ËõLSBÏÈ½ÓÊÕ¡£RFIG=0£»Frame-sync detect.
	RCR1_0  = 0x0040;//½ÓÊÕ×Ö³¤Îª16Î»
}  

/*´®¿Ú½ÓÊÕÊý¾Ý³õÊ¼»¯
  º¯Êý¹¦ÄÜ£º´®¿Ú³õÊ¼»¯ÅäÖÃ£¬²¨ÌØÂÊÎª38400
*/

void receive_752_init()
{
   UART_IER232 = 0x00;//ÇåÖÐ¶Ï
   asm(" nop ");
   UART_LCR232 = 0x80;//DLAB=1,ÏÂ²½½øÐÐ²¨ÌØÂÊÉèÖÃ
   asm(" nop ");
   UART_DLL232 = 0x18;//²¨ÌØÂÊÉèÖÃ0x60->9600
   asm(" nop ");    //²¨ÌØÂÊÉèÖÃ0x18->38400
   UART_DLH232 = 0x00;
   asm(" nop ");
   UART_LCR232 = 0x0bf;//ÉèÖÃÆäËû¼Ä´æÆ÷Ç°±ØÐë½«LCRÉèÖÃÎª0x0bf£¨²Î¼ûÐ¾Æ¬ÎÄµµ£©
   asm(" nop ");
   UART_EFR232 = 0x10;//Ê¹ÄÜIER£ºbit4~7;FCR:bit4~5;MCR:bit5~7
   asm(" nop ");
   UART_LCR232 = 0x00;//»¹Ô­LCR
   asm(" nop ");
   UART_FCR232 = 0x0ff;//³õÊ¼»¯ÉèÖÃFIFO£¬Ê¹ÄÜ£¬ÇåÁã
   asm(" nop ");
   UART_MCR232 = 0x48;//
   asm(" nop ");
   UART_TCR232 = 0x03;//RCVFIFOÉèÖÃÎª28×Ö½ÚÍ£Ö¹·¢ËÍ
   asm(" nop ");
   UART_TLR232 = 0x03f;//RCVFIFOÖÐ¶Ï´¥·¢¼¶±ð0x03*4=12£¬12¸ö×Ö½Ú´¥·¢Ò»´ÎÖÐ¶Ï
   asm(" nop ");
   UART_LCR232 = 0x03;//ÉèÖÃÊý¾Ý¸ñÊ½£ºÎÞÆæÅ¼Ð£Ñé£¬1±ÈÌØÍ£Ö¹Î»£¬8±ÈÌØÊý¾ÝÎ»
   asm(" nop ");
   UART_IER232 = 0x01;//Ê¹ÄÜFIFO½ÓÊÕÖÐ¶Ï
   asm(" nop ");
//Í¨µÀ2³õÊ¼»¯ÓÃÓÚÑ¹Á¦´«¸ÐÆ÷´«ÊäÍ¨ÐÅ£¬Ó³ÉäCE1¿Õ¼ä
   UART_IER485 = 0x00;//ÇåÖÐ¶Ï
   asm(" nop ");
   UART_LCR485 = 0x80;//DLAB=1,ÏÂ²½½øÐÐ²¨ÌØÂÊÉèÖÃ
   asm(" nop ");
   UART_DLL485 = 0x60;//²¨ÌØÂÊÉèÖÃ0x18->38400
   asm(" nop ");    //0x60->9600
   UART_DLH485 = 0x00;//0x30->19200
   asm(" nop ");
   UART_LCR485 = 0x0bf;//ÉèÖÃÆäËû¼Ä´æÆ÷Ç°±ØÐë½«LCRÉèÖÃÎª0x0bf£¨²Î¼ûÐ¾Æ¬ÎÄµµ£©
   asm(" nop ");
   UART_EFR485 = 0x10;//Ê¹ÄÜIER£ºbit4~7;FCR:bit4~5;MCR:bit5~7
   asm(" nop ");
   UART_LCR485 = 0x00;//»¹Ô­LCR
   asm(" nop ");
   UART_FCR485 = 0x0ff;//³õÊ¼»¯ÉèÖÃFIFO£¬Ê¹ÄÜ£¬ÇåÁã
   asm(" nop ");
   UART_MCR485 = 0x48;//
   asm(" nop ");
   UART_TCR485 = 0x06;//RCVFIFOÖÐ¶Ï¼¶±ð´¥·¢Í£Ö¹·¢ËÍ
   asm(" nop ");
   UART_TLR485 = 0x03f;//RCVFIFOÖÐ¶Ï´¥·¢¼¶±ð0x06*4=24£¨¿ÉÄÜÐèÒªÐÞ¸ÄÏÂ£©
   asm(" nop ");
   UART_LCR485 = 0x03;//ÉèÖÃÊý¾Ý¸ñÊ½£ºÎÞÆæÅ¼Ð£Ñé£¬1±ÈÌØÍ£Ö¹Î»£¬8±ÈÌØÊý¾ÝÎ»
   asm(" nop ");
   UART_IER485 = 0x01;//Ê¹ÄÜFIFO½ÓÊÕÖÐ¶Ï
   asm(" nop ");
}
/*ÏµÍ³³õÊ¼»¯ÅäÖÃ
  º¯Êý¹¦ÄÜ£º³õÊ¼»¯Ê±ÖÓÎª200MHz; ´®¿ÚÅäÖÃÔÚCE0¡¢CE1¿Õ¼ä; SDRAMÅäÖÃÔÚCE2¡¢CE3¿Õ¼ä
			ÖÐ¶ÏÏòÁ¿±íÓ³ÉäÔÚ0x000e;	³õÊ¼»¯GPIOÎªÊä³ö£¬Êä³öÎªµÍ
*/
void System_initial()
{
	CLKMD = 0x0000;
	while(CLKMD & 0x0001){}
	CLKMD = 0x2CB2;//clk = 8M; mult=50;div=1;50/2*8M=200M.CLKMD = 0x2c92 ;¸Ä144M CLKMD = 0x2912 bywyh,100M CLKMD = 0xCB2 bywyh
	EBSR=0x0211;//ÉèÖÃÎªÈ«²¿EMIFÄ£Ê½£¬ÎÞ±£³ÖÐÅºÅÓ¦´ð
	EMI_RST=0x0000;
    //CE2ºÍCE3ÎªSDRAM
/*	
	EGCR = EGCR & 0x0ffdf;//SDRAM³õÊ¼»¯
    EGCR = 0x020f;//CLKMEM=CPUÊ±ÖÓµÄÒ»°ë£»²»ÏìÓ¦Íâ²¿×ÜÏßÕ÷ÓÃÇëÇó
	CE2_1 = 0x3000;//16bit¿í¶ÈSDRAM;
	CE3_1 = 0x3000;//16bit¿í¶ÈSDRAM;       
	SDC1 = 0x5958;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF¶ÔSDRAM½øÐÐË¢ÐÂ;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
//	SDC1 = 0x5858;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF¶ÔSDRAM½øÐÐË¢ÐÂ;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
	SDC2 = 0x028f;//SDACC=0,EMIF×ÜÏß¿í¶ÈÎª16bit;TMRD=2,tMRD=30ns;TRAS=8,tRAS=90ns;TACTV2ACTV=15,tRRD=160ns
    SDPER = 0x061a;//PERIOD=15620ns
    EGCR = EGCR|0x0020;//CLKMEMÒý½ÅÊä³öÊ±ÖÓÊ¹ÄÜ
    INIT = 0x0000;//³õÊ¼»¯SDRAM
	*/
//CE0ºÍCE1³õÊ¼»¯ÎªÒì²½´®¿Ú yuanshi
/*
	CE0_1=0x103f;//16bit¿í¶ÈÒì²½´æ´¢Æ÷;¶Á½¨Á¢Ê±¼ä5ns;¶ÁÑ¡Í¨Ê±¼ä75ns;¶Á±£³ÖÊ±¼ä15ns
	CE0_2=0x00ff;//¶ÁÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´ÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´½¨Á¢Ê±¼ä5ns;Ð´Ñ¡Í¨Ê±¼ä315ns;Ð´±£³ÖÊ±¼ä15ns
	CE0_3=0x00ff;//³¬Ê±Ê±¼äÎª256¸öCPUÊ±ÖÓ

	CE1_1=0x103f;//16bit¿í¶ÈÒì²½´æ´¢Æ÷;¶Á½¨Á¢Ê±¼ä5ns;¶ÁÑ¡Í¨Ê±¼ä75ns;¶Á±£³ÖÊ±¼ä15ns
	CE1_2=0x00ff;//¶ÁÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´ÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´½¨Á¢Ê±¼ä5ns;Ð´Ñ¡Í¨Ê±¼ä315ns;Ð´±£³ÖÊ±¼ä15ns
	CE1_3=0x00ff;//³¬Ê±Ê±¼äÎª256¸öCPUÊ±ÖÓ
*///bywyh EMIF pdf clk=144MHz
	CE0_1=0x112F;//16bit¿í¶ÈÒì²½´æ´¢Æ÷;¶Á½¨Á¢Ê±¼ä5ns;¶ÁÑ¡Í¨Ê±¼ä75ns;¶Á±£³ÖÊ±¼ä15ns
	CE0_2=0x00BB;//¶ÁÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´ÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´½¨Á¢Ê±¼ä5ns;Ð´Ñ¡Í¨Ê±¼ä315ns;Ð´±£³ÖÊ±¼ä15ns
	CE0_3=0x00ff;//³¬Ê±Ê±¼äÎª256¸öCPUÊ±ÖÓ
	CE1_1=0x112F;//16bit¿í¶ÈÒì²½´æ´¢Æ÷;¶Á½¨Á¢Ê±¼ä5ns;¶ÁÑ¡Í¨Ê±¼ä75ns;¶Á±£³ÖÊ±¼ä15ns
	CE1_2=0x00BB;//¶ÁÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´ÑÓ³¤±£³ÖÊ±¼ä5ns;Ð´½¨Á¢Ê±¼ä5ns;Ð´Ñ¡Í¨Ê±¼ä315ns;Ð´±£³ÖÊ±¼ä15ns
	CE1_3=0x00ff;//³¬Ê±Ê±¼äÎª256¸öCPUÊ±ÖÓ
	IVPH=0x000e;//ÖÐ¶ÏÏòÁ¿±íÓ³ÉäµØÖ·
	IVPD=0x000e;
	receive_752_init();//´®¿Ú½ÓÊÕÊý¾Ý³õÊ¼»¯
}
/******************************************************************
º¯Êý£ºvoid SD_WriteData(unsigned int SectorNumber,Uint16 *SendData)
¹¦ÄÜ£ºÏòÖ¸¶¨µÄÉÈÇøÖÐÐ´Èë512×Ö½ÚµÄÊý¾Ý
ÊäÈë²ÎÊý£ºÉÈÇøºÅ£¨0~¸ù¾Ý¿¨µÄÈÝÁ¿¼ÆËã£©£¬Ð´ÈëÊý¾ÝµÄÊ×µØÖ·
·µ»Ø£ºÎÞ
*******************************************************************/
void SD_WriteData(Uint32 SectorNumber,Uint16 *SendData)
{
	  volatile Uint16 i,j;
	  //asm(" BCLR XF ");//²âÊÔÐ´ÈëÊ±¼ä,µÚÒ»¸öÒýºÅºó±ØÐëÓÐÖÁÉÙÒ»¸ö¿Õ¸ñ
	 
	  MMC_write(mmc1,512*SectorNumber,SendData,512);
	 // busy_flag_0=MMCST0_1;
	  //³¤ÑÓÊ±£¬µÈ´ýSD¿¨×Ô±à³Ì½áÊø(¸ù¾ÝÏµÍ³Ê±ÖÓÆµÂÊºÍMMC¿ØÖÆÆ÷ÓëSD¿¨Í¨ÐÅÊ±ÖÓÆµÂÊÀ´¾ö¶¨ÑÓÊ±£©
	  //ÏÂÃæµÄÑÓÊ±¶ÔÓÚFsd=666.7KHz £¬Fsystem=192MHzµÄÕý³£µ÷ÊÔ½á¹û¡££¨¾­Ñé¹«Ê½£ºµ±Fsd=500KHz £¬Fsystem=24MHzÊ±£¬ÒÔÏÂÑÓÊ±ÖÁÉÙ9¸ö£©
	  //¾­±ÊÕßµ÷ÊÔÇé¿ö¿´£¬´Ë´¦ÑÓÊ±ºÜÖØÒª  272
	  for(i=0;i<84;i++)
	  {
	      for (j = 0; j <= 25000; j++)
	      asm("	NOP ");
	  } 

  //asm(" BSET XF");	
}

/******************************************************************
º¯Êý£ºSD_ReadData(unsigned int SectorNumber,Uint16 *ReceiveData)
¹¦ÄÜ£ºÏòÖ¸¶¨µÄÉÈÇøÖÐ¶Á³ö512×Ö½ÚµÄÊý¾Ý
ÊäÈë²ÎÊý£ºÉÈÇøºÅ£¨0~¸ù¾Ý¿¨µÄÈÝÁ¿¼ÆËã£©£¬´æ´¢¶Á³öÊý¾ÝµÄÊ×µØÖ·
·µ»Ø£ºÎÞ
*******************************************************************/
void SD_ReadData(Uint32 SectorNumber,Uint16 *ReceiveData)
{
  	MMC_read(mmc1,512*SectorNumber,ReceiveData,512);  
}

/*************************************************************************
º¯Êý£ºUint16 ReadSectorNumber_Usable(void)
¹¦ÄÜ£º´ÓµÚ2012159ÉÈÇøÖÐ¶Á³ö¿ÉÓÃµÄÉÈÇøºÅ(¶ÔÓ¦1G¿¨µÄµ¹ÊýµÚ¶þ¸öÉÈÇø£©
ÊäÈë²ÎÊý£ºÎÞ
·µ»Ø£º¿ÉÓÃµÄÉÈÇøºÅ
***************************************************************************/
Uint32 ReadSectorNumber_Usable(void)
{
  SD_ReadData(10000000,SecCounter);
  
  return((Uint32)((((Uint32)SecCounter[1])<<16)|(Uint32)SecCounter[0]));
}
/*************************************************************************
º¯Êý£ºvoid SaveSectorCounter(void)
¹¦ÄÜ£º±£´æ×îºóÏÂÒ»¸ö¿ÕÉÈÇøºÅ£¬±£´æÔÚµÚ2012159ÉÈÇøÖÐ?ÒÔ±¸ÏÂ´Î½Ó×Å¸ÃÉÈÇøÐ´ÈëÊý¾Ý
ÊäÈë²ÎÊý£ºÎÞ
·µ»Ø£ºÎÞ
***************************************************************************/
void SaveSectorCounter(void)
{
  SecCounter[0]=(Uint16)SectorCounter;
  SecCounter[1]=(Uint16)(SectorCounter>>16);
  SD_WriteData(10000000,SecCounter);
}
/****************************************************
º¯Êý£ºvoid MMC_Init(void)
¹¦ÄÜ£º³õÊ¼»¯MMC¿ØÖÆÆ÷
ÊäÈë²ÎÊý£ºÎÞ
·µ»Ø£ºÎÞ
****************************************************/
void MMC_Init(void)
{
  volatile short SD_jishu;
  //×¢£º¶ÔÓÚ5509aµÄMMC¿ØÖÆÆ÷£¬Ö»Ö§³ÖMMC¿¨ºÍSD¿¨
  mmc1 = MMC_open(MMC_DEV1);//Ê¹ÄÜMMC0¿ØÖÆÆ÷
  MMC_setupNative(mmc1,&Init);//°´ÕÕ³õÊ¼²ÎÊýÀ´³õÊ¼»¯MMC0¿ØÖÆÆ÷

  MMC_sendGoIdle(mmc1);//¸´Î»×ÜÏßÉÏµÄËµÓÐ´æ´¢¿¨
  for (count=0;count<4016;count++)
  {
  	asm(" NOP ");
  }
  SD_jishu = 0;
  do
  {
	  cardtype = MMC_sendOpCond(mmc1,0x00100000);//¹¤×÷µçÑ¹ÃÅÏÞ£º3.2~3.3V£¬²»Í¬µÄ¿¨¶Ô¸ÃÃüÁîµÄÓ¦´ðÖµ²»Í¬£¬MMC¿ØÖÆÆ÷¸ù¾ÝÓ¦´ðÖµÀ´ÅÐ¶Ï¸Ã´æ´¢¿¨ÀàÐÍ
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
     //¼ì²âµ½ÎªSD¿¨
     cid = &cardid;
     SD_sendAllCID(mmc1,cid); // get the CID structure for all cards.     
     card = &cardalloc;
     rca = SD_sendRca(mmc1,card);
     SD_status = 1;
  
	                       
	  retVal = MMC_selectCard(mmc1,card);//Ñ¡ÔñMMC0À´ºÍSD¿¨½øÐÐÍ¨ÐÅ
	  
	  SD_setWidth(mmc1, 0x4);//MMC¿ØÖÆÆ÷ºÍSD¿¨µÄÍ¨ÐÅÊý¾Ý¿í¶ÈÎª4bits
  }
}   

void uart_config()
{
	memset(SD_write,0,sizeof(short)*256);
	SD_write[0] = 0x0ee;   //·¢ËÍ×Ö½ÚÍ·
	SD_write[1] = transponder_mode;//¹¤×÷Ä£Ê½
	SD_write[2] = tongbu_flag;//ÃüÁîÎ»
	SD_write[3] = 0x0bb;//Ð£ÑéÎ»
	SD_write[4] = (NOISE_TH_NEW) & 0x0ff;//¼ì²âÃÅÏÞµÍ8bit
	SD_write[5] = (NOISE_TH_NEW>>8) & 0x0ff;//¼ì²âÃÅÏÞ¸ß8bit
	SD_write[6] = responder_period/10;//Ð£ÑéÎ»
	SD_write[7] = 0x0cc;//Ð£ÑéÎ»
	SD_write[8] = ((Time_Delay/1000)) & 0x0ff;//×ª·¢Ê±ÑÓµÍ8bit
	SD_write[9] = ((Time_Delay/1000)>>8) & 0x0ff;//×ª·¢Ê±ÑÓ¸ß8bit
	SD_write[10] = 0x0cc;//Ð£ÑéÎ»
	SD_write[11] = 0x01;//Ð£ÑéÎ»
	SD_write[12] = (voltage_AD) & 0x0ff;//µçÑ¹Á¿»¯ÖµµÍ8bit
	SD_write[13] = (voltage_AD>>8) & 0x0ff;//µçÑ¹Á¿»¯Öµ¸ß8bit
	SD_write[14] = 0x01;//Ð£ÑéÎ»
	SD_write[15] = (RS485_DATA[5]) & 0xff;//SD_write[15] = (RS485_DATA[5]) & 0xff;
	SD_write[16] = ((RS485_DATA[4])) & 0xff;//SD_write[16] = ((RS485_DATA[4])) & 0xff
	SD_write[17] = ((RS485_DATA[3])) & 0xff;//SD_write[17] = ((RS485_DATA[3])) & 0xff
	SD_write[18] = ((RS485_DATA[2])) & 0xff;//SD_write[18] = ((RS485_DATA[2])) & 0xff
	SD_write[19] = ((Uint16)(temperature/0.5)) & 0xff;//ÎÂ¶ÈÁ¿»¯ÖµµÍ8bit
	SD_write[20] = (((Uint16)(temperature/0.5))>>8) & 0xff;//ÎÂ¶ÈÁ¿»¯Öµ¸ß8bit
	SD_write[21] = (noise_10ms) & 0x0ff;//ÔëÉùÁ¿»¯ÖµµÍ8bit
	SD_write[22] = (noise_10ms>>8) & 0x0ff;//ÔëÉùÁ¿»¯Öµ¸ß8bit
	SD_write[23] = 0x00;//FSKÆµÂÊ1---9kHz
	SD_write[24] = 0x08;//FSKÆµÊ2---9+8*0.5=13kHz
	SD_write[25] = 0x0ff;//éÎ
	SD_write[26] = SD_status;//SD¿¨×´Ì¬£¬1ÎªÕý³££¬0Îª²»Õý³£
 	SD_write[27] = 0x0ff;//Ð£ÑéÎ»
}
