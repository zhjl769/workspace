/* Create DMA Receive Side Configuration */
DMA_Config  myConfig = { 
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,//��ַ�����
    DMA_DMACSDP_DSTPACK_OFF,
    DMA_DMACSDP_DST_DARAM,//Ŀ�ĵ�ַΪDARAM
    DMA_DMACSDP_SRCBEN_NOBURST,
    DMA_DMACSDP_SRCPACK_OFF,
    DMA_DMACSDP_SRC_PERIPH,//Դ��ַΪ����
    DMA_DMACSDP_DATATYPE_16BIT//16bit����
  ),                                       /* DMACSDP  */
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_POSTINC,//Ŀ�ĵ�ַ�Լ�
    DMA_DMACCR_SRCAMODE_CONST,//Դ��ַΪ����
    DMA_DMACCR_ENDPROG_OFF,
    DMA_DMACCR_REPEAT_OFF,
    DMA_DMACCR_AUTOINIT_ON,//���������ȴ�ENDPROG��Ϊ1�����´δ���
    DMA_DMACCR_EN_STOP,//��ֹDMA����
    DMA_DMACCR_PRIO_LOW,//���ȼ�Ϊ��
    DMA_DMACCR_FS_DISABLE,//���ݵ�Ԫ����ȴ�ͬ���¼�
    DMA_DMACCR_SYNC_REVT0//ͬ���¼�ΪMcBSP0�����ж�
  ),                                       /* DMACCR   */
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,
    DMA_DMACICR_LASTIE_OFF,
    DMA_DMACICR_FRAMEIE_ON,//֡�ж�ʹ��
    DMA_DMACICR_FIRSTHALFIE_OFF,
    DMA_DMACICR_DROPIE_OFF,
    DMA_DMACICR_TIMEOUTIE_OFF
  ),                                       /* DMACICR  */
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),        /* DMACSSAL *///McBSP�������ݼĴ���
    0,                                     /* DMACSSAU */
    (DMA_AdrPtr)&Rev1,                      /* DMACDSAL *///��������
    0,                                     /* DMACDSAU */
    BUF_LEN_MAX,                                     /* DMACEN   *///�������ݳ���
    1,                                     /* DMACFN   */
    0,                                     /* DMACFI  */
    0                                     /* DMACEI  */
};

/*DMA����ģʽ����*/
DMA_Config  myconfigdma0 = { 
  DMA_DMACSDP_RMK(
    DMA_DMACSDP_DSTBEN_NOBURST,    /**/
    DMA_DMACSDP_DSTPACK_OFF,       /**/  
    DMA_DMACSDP_DST_PERIPH,         /*Ŀ�ĵ�ַ:����������'_PERIPH'GPIO��*/
    DMA_DMACSDP_SRCBEN_NOBURST,    /**/
    DMA_DMACSDP_SRCPACK_OFF,       /**/
    DMA_DMACSDP_SRC_SARAM,         /*Դ��ַEMIF*/
    DMA_DMACSDP_DATATYPE_16BIT     /*_16BIT:��ͨ���������������*/
  ),                                       /* DMACSDP  */
  DMA_DMACCR_RMK(
    DMA_DMACCR_DSTAMODE_CONST,   /*Ŀ�ĵ�ַΪ������Դ��ַ�Լ�*/
    DMA_DMACCR_SRCAMODE_POSTINC,   /**/
    DMA_DMACCR_ENDPROG_OFF,         /**/
    DMA_DMACCR_REPEAT_OFF,         /**/
    DMA_DMACCR_AUTOINIT_ON,        /*���������ȴ�ENDPROG��Ϊ1�����´δ���*/
    DMA_DMACCR_EN_STOP,            /**/
    DMA_DMACCR_PRIO_HI,            /**/
    DMA_DMACCR_FS_ELEMENT,         /*��Ԫͬ��*/
    DMA_DMACCR_SYNC_TIM2INT           /*��ʱ����ʱ����*/
  ),                                       /* DMACCR   */
  DMA_DMACICR_RMK(
    DMA_DMACICR_BLOCKIE_OFF,       /*0*/
    DMA_DMACICR_LASTIE_OFF,        /*0*/
    DMA_DMACICR_FRAMEIE_ON,        /*֡�ж�*/
    DMA_DMACICR_FIRSTHALFIE_OFF,   /*0*/
    DMA_DMACICR_DROPIE_OFF,        /*0*/
    DMA_DMACICR_TIMEOUTIE_OFF      /*0*/
  ),                                      
    (DMA_AdrPtr)&Send_sig,                /* DMACSSAL *///CE2�ռ�
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
                      TIMER_TCR_FUNC_OF(11),        /* FUNC   == 11�ⲿʱ��Դ */ \
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
                      TIMER_TCR_FUNC_OF(01),        /* FUNC   == 01CPUʱ��Դ */ \
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

TIMER_Config timCfg0 = {	//Timer0Ϊ100ms  �ⲿʱ��Ƶ��=10MHz  
   TIMER_CTRL0,               /* TCR0 */
   0xf423u,                  /* PRD0=62499 */
   0x000fu                    /* PRSC=15 */
};//��ʱ100ms 

//change by wyh 
TIMER_Config timCfg1 = {	//Timer1Ϊ1uS
   TIMER_CTRL1,               /* TCR0 */
   0x008fu,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;*/
   0x0000                    /* PRSC */
};


//MMC�������ĳ�ʼ���������û����Ը���
//change by wyh

MMC_SetupNative Init = {
    0,   /* ��ֹMMC��DMA���� */
    0,   /* Set level of edge detection for DAT3 pin        */
    0,   /* Determines if MMC goes IDLE during IDLE instr   */
    1,   /* Memory clk reflected on CLK Pin                 */
    17,  /* 23��MMC��������Ƶ�ʷ�Ƶ���ӣ�Fmmc=Fsystem/(a+1),aΪ�ô���ʼֵ����Χ��1~255   91*/
    23,  /* 17��MMC��������SD����ͨ��Ƶ�ʣ�Fsd=Fsystem/(2(a+1)(b+1)),aΪ�ϸ�ֵ��bΪ�ô���ʼֵ����Χ��0~255 17*/
    0,   /* No. memory clks to wait before response timeout */
    0,   /* No. memory clks to wait before data timeout     */
    512, /* ÿ������Ϊ512�ֽڣ������SD��CSD�е�������ͬ��  */
  };


void DMA_init(void)
{
	RevsrcAddrHi = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) >> 15) & 0xFFFFu;//McBSP���ռĴ������ֽڵ�ַ
    RevsrcAddrLo = (Uint16)(((Uint32)(MCBSP_ADDR(DRR10))) << 1) & 0xFFFFu;//McBSP���ռĴ������ֽڵ�ַ
    Revdst1AddrHi = (Uint16)(((Uint32)(&Rev1)) >> 15) & 0xFFFFu;//�ص���������1���ֽڵ�ַ
    Revdst1AddrLo = (Uint16)(((Uint32)(&Rev1)) << 1) & 0xFFFFu;//�ص���������1���ֽڵ�ַ
	Revdst2AddrHi = (Uint16)(((Uint32)(&Rev2)) >> 15) & 0xFFFFu;//�ص���������2���ֽڵ�ַ
    Revdst2AddrLo = (Uint16)(((Uint32)(&Rev2)) << 1) & 0xFFFFu;//�ص���������2���ֽڵ�ַ
 

    myConfig.dmacssal = (DMA_AdrPtr)RevsrcAddrLo;//DMAͨ��Դ��ַ���ֽ�
    myConfig.dmacssau = RevsrcAddrHi;//DMAͨ��Դ��ַ���ֽ�
    myConfig.dmacdsal = (DMA_AdrPtr)Revdst1AddrLo;//DMAͨ��Ŀ�ĵ�ַ���ֽ�
    myConfig.dmacdsau = Revdst1AddrHi;//DMAͨ��Ŀ�ĵ�ַ���ֽ�

	sendsrc0AddrHi = (Uint16)(((Uint32)(myconfigdma0.dmacssal)) >> 15) & 0xFFFFu;//DMA0����Դ��ַ���ֽ�
    sendsrc0AddrLo = (Uint16)(((Uint32)(myconfigdma0.dmacssal)) << 1) & 0xFFFFu;//DMA0����Դ��ַ���ֽ�
    senddst0AddrHi = (Uint16)(((Uint32)(myconfigdma0.dmacdsal)) >> 15) & 0xFFFFu;//DMA0����Ŀ�ĵ�ַ���ֽ�
    senddst0AddrLo = (Uint16)(((Uint32)(myconfigdma0.dmacdsal)) << 1) & 0xFFFFu;//DMA0����Ŀ�ĵ�ַ���ֽ�

    myconfigdma0.dmacssal = (DMA_AdrPtr)sendsrc0AddrLo;//����Դ��ַEMIF
    myconfigdma0.dmacssau = sendsrc0AddrHi;
    myconfigdma0.dmacdsal = (DMA_AdrPtr)senddst0AddrLo;//����Ŀ�ĵ�ַGPIO
    myconfigdma0.dmacdsau = senddst0AddrHi;

  

    hDmaSend0 = DMA_open(DMA_CHA0,DMA_OPEN_RESET);//DMA0����GPIO����

    myhDma = DMA_open(DMA_CHA2,DMA_OPEN_RESET);  //DMA2����McBSP�ɼ�����

	mhTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);//��ʱ��0��Ϊͬ��ʱ��
	mhTimer1 = TIMER_open(TIMER_DEV1, TIMER_OPEN_RESET);//��ʱ��1��Ϊ�����ź�ͬ���¼�

    DMA_config(myhDma,&myConfig);
    DMA_config(hDmaSend0,&myconfigdma0);
	
	TIMER_config(mhTimer0, &timCfg0);
	TIMER_config(mhTimer1, &timCfg1);
}




