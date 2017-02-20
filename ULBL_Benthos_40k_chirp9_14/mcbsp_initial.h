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
    25000,                                   
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
   0x0063u,                  /* PRD0 200MHz->0x00c7; 144MHz->0x008f;100MHz->0x0063*/
   0x0000                    /* PRSC */
};

//MMC�������ĳ�ʼ���������û����Ը���
//change by wyh
MMC_SetupNative Init = {
    0,   /* ��ֹMMC��DMA���� */
    0,   /* Set level of edge detection for DAT3 pin        */
    0,   /* Determines if MMC goes IDLE during IDLE instr   */
    1,   /* Memory clk reflected on CLK Pin                 */
    11,  /* 23��MMC��������Ƶ�ʷ�Ƶ���ӣ�Fmmc=Fsystem/(a+1),aΪ�ô���ʼֵ����Χ��1~255   91*/
    18,  /* 17��MMC��������SD����ͨ��Ƶ�ʣ�Fsd=Fsystem/(2(a+1)(b+1)),aΪ�ϸ�ֵ��bΪ�ô���ʼֵ����Χ��0~255 17*/
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


/*//change by wyh 
  �������ܣ���ʼ���ڲ�ADCʱ��Ϊ2MHz	 ������Ϊ10kHz
ADC Sample and Hold Period =  (ADC Clock Period) *(2 *(CONVRATEDIV + 1 + SAMPTIMEDIV))
*/
void InitADC()
{
	ADCCLKCTL = 0x17; // 4MHz ADCLK   ADC Clock = (CPU Clock)/(CPUCLKDIV + 1)
	ADCCLKDIV = 0x0ba00; //10kHz������
}
  
/*McBSP��ʼ������  �������ܣ���ʼ������AD������Ϊ40kHz  */
//change by wyh��ֻ��Ҫ�� ��SRGR1_0=0x0223  
void McBSP_init()
{
	SPCR2_0 = 0x0000;
	SPCR1_0 = 0x0000;
	XCR2_0  = 0x0000;
	XCR1_0  = 0x0040;//XWDLEN1=16bit
	SRGR2_0 = 0x3063;//CLKSM=1��McBSP internal input clock��    ֡ͬ���ź�����Ϊ20;������Ϊ40kHz;FPER=99;CLKin=144M,CLKG= 
	SRGR1_0 = 0x0218;//֡ͬ������4��CLKG���;ʱ�ӷ�Ƶ����Ϊ100//����FWID,CLKGDV,  fs=40k,clk=200MHz,0231h
	PCR0    = 0x0b0d;//FSXM=1��֡ͬ����mcbsp����;FSRM=0������ͬ�����ⲿ������CLKXM=CLKRM=1;FSXP=1������֡ͬ������Ч��FSRP=1;CLKRP=CLKXP���շ���ͬԴ��
	RCR2_0  = 0x0005;//RWDLEN2=000��RCOMPAND=01����ѹ��LSB�Ƚ��ա�RFIG=0��Frame-sync detect.
	RCR1_0  = 0x0040;//�����ֳ�Ϊ16λ
}  

/*���ڽ������ݳ�ʼ��
  �������ܣ����ڳ�ʼ�����ã�������Ϊ38400
*/

void receive_752_init()
{
   UART_IER232 = 0x00;//���ж�
   asm(" nop ");
   UART_LCR232 = 0x80;//DLAB=1,�²����в���������
   asm(" nop ");
   UART_DLL232 = 0x18;//����������0x60->9600
   asm(" nop ");    //����������0x18->38400
   UART_DLH232 = 0x00;
   asm(" nop ");
   UART_LCR232 = 0x0bf;//���������Ĵ���ǰ���뽫LCR����Ϊ0x0bf���μ�оƬ�ĵ���
   asm(" nop ");
   UART_EFR232 = 0x10;//ʹ��IER��bit4~7;FCR:bit4~5;MCR:bit5~7
   asm(" nop ");
   UART_LCR232 = 0x00;//��ԭLCR
   asm(" nop ");
   UART_FCR232 = 0x0ff;//��ʼ������FIFO��ʹ�ܣ�����
   asm(" nop ");
   UART_MCR232 = 0x48;//
   asm(" nop ");
   UART_TCR232 = 0x03;//RCVFIFO����Ϊ28�ֽ�ֹͣ����
   asm(" nop ");
   UART_TLR232 = 0x03f;//RCVFIFO�жϴ�������0x03*4=12��12���ֽڴ���һ���ж�
   asm(" nop ");
   UART_LCR232 = 0x03;//�������ݸ�ʽ������żУ�飬1����ֹͣλ��8��������λ
   asm(" nop ");
   UART_IER232 = 0x01;//ʹ��FIFO�����ж�
   asm(" nop ");
//ͨ��2��ʼ������ѹ������������ͨ�ţ�ӳ��CE1�ռ�
   UART_IER485 = 0x00;//���ж�
   asm(" nop ");
   UART_LCR485 = 0x80;//DLAB=1,�²����в���������
   asm(" nop ");
   UART_DLL485 = 0x60;//����������0x18->38400
   asm(" nop ");    //0x60->9600
   UART_DLH485 = 0x00;//0x30->19200
   asm(" nop ");
   UART_LCR485 = 0x0bf;//���������Ĵ���ǰ���뽫LCR����Ϊ0x0bf���μ�оƬ�ĵ���
   asm(" nop ");
   UART_EFR485 = 0x10;//ʹ��IER��bit4~7;FCR:bit4~5;MCR:bit5~7
   asm(" nop ");
   UART_LCR485 = 0x00;//��ԭLCR
   asm(" nop ");
   UART_FCR485 = 0x0ff;//��ʼ������FIFO��ʹ�ܣ�����
   asm(" nop ");
   UART_MCR485 = 0x48;//
   asm(" nop ");
   UART_TCR485 = 0x06;//RCVFIFO�жϼ��𴥷�ֹͣ����
   asm(" nop ");
   UART_TLR485 = 0x03f;//RCVFIFO�жϴ�������0x06*4=24��������Ҫ�޸��£�
   asm(" nop ");
   UART_LCR485 = 0x03;//�������ݸ�ʽ������żУ�飬1����ֹͣλ��8��������λ
   asm(" nop ");
   UART_IER485 = 0x01;//ʹ��FIFO�����ж�
   asm(" nop ");
}
/*ϵͳ��ʼ������
  �������ܣ���ʼ��ʱ��Ϊ200MHz; ����������CE0��CE1�ռ�; SDRAM������CE2��CE3�ռ�
			�ж�������ӳ����0x000e;	��ʼ��GPIOΪ��������Ϊ��
*/
void System_initial()
{
	CLKMD = 0x0000;
	while(CLKMD & 0x0001){}
	CLKMD = 0x2CB2;//clk = 8M; mult=50;div=1;50/2*8M=200M.CLKMD = 0x2c92 ;��144M CLKMD = 0x2912 bywyh,100M CLKMD = 0xCB2 bywyh
	EBSR=0x0211;//����Ϊȫ��EMIFģʽ���ޱ����ź�Ӧ��
	EMI_RST=0x0000;
    //CE2��CE3ΪSDRAM
/*	
	EGCR = EGCR & 0x0ffdf;//SDRAM��ʼ��
    EGCR = 0x020f;//CLKMEM=CPUʱ�ӵ�һ�룻����Ӧ�ⲿ������������
	CE2_1 = 0x3000;//16bit���SDRAM;
	CE3_1 = 0x3000;//16bit���SDRAM;       
	SDC1 = 0x5958;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF��SDRAM����ˢ��;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
//	SDC1 = 0x5858;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF��SDRAM����ˢ��;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
	SDC2 = 0x028f;//SDACC=0,EMIF���߿��Ϊ16bit;TMRD=2,tMRD=30ns;TRAS=8,tRAS=90ns;TACTV2ACTV=15,tRRD=160ns
    SDPER = 0x061a;//PERIOD=15620ns
    EGCR = EGCR|0x0020;//CLKMEM�������ʱ��ʹ��
    INIT = 0x0000;//��ʼ��SDRAM
	*/
//CE0��CE1��ʼ��Ϊ�첽���� yuanshi
/*
	CE0_1=0x103f;//16bit����첽�洢��;������ʱ��5ns;��ѡͨʱ��75ns;������ʱ��15ns
	CE0_2=0x00ff;//���ӳ�����ʱ��5ns;д�ӳ�����ʱ��5ns;д����ʱ��5ns;дѡͨʱ��315ns;д����ʱ��15ns
	CE0_3=0x00ff;//��ʱʱ��Ϊ256��CPUʱ��

	CE1_1=0x103f;//16bit����첽�洢��;������ʱ��5ns;��ѡͨʱ��75ns;������ʱ��15ns
	CE1_2=0x00ff;//���ӳ�����ʱ��5ns;д�ӳ�����ʱ��5ns;д����ʱ��5ns;дѡͨʱ��315ns;д����ʱ��15ns
	CE1_3=0x00ff;//��ʱʱ��Ϊ256��CPUʱ��
*///bywyh EMIF pdf clk=144MHz
	CE0_1=0x112F;//16bit����첽�洢��;������ʱ��5ns;��ѡͨʱ��75ns;������ʱ��15ns
	CE0_2=0x00BB;//���ӳ�����ʱ��5ns;д�ӳ�����ʱ��5ns;д����ʱ��5ns;дѡͨʱ��315ns;д����ʱ��15ns
	CE0_3=0x00ff;//��ʱʱ��Ϊ256��CPUʱ��
	CE1_1=0x112F;//16bit����첽�洢��;������ʱ��5ns;��ѡͨʱ��75ns;������ʱ��15ns
	CE1_2=0x00BB;//���ӳ�����ʱ��5ns;д�ӳ�����ʱ��5ns;д����ʱ��5ns;дѡͨʱ��315ns;д����ʱ��15ns
	CE1_3=0x00ff;//��ʱʱ��Ϊ256��CPUʱ��
	IVPH=0x000e;//�ж�������ӳ���ַ
	IVPD=0x000e;
	receive_752_init();//���ڽ������ݳ�ʼ��
}
/******************************************************************
������void SD_WriteData(unsigned int SectorNumber,Uint16 *SendData)
���ܣ���ָ����������д��512�ֽڵ�����
��������������ţ�0~���ݿ����������㣩��д�����ݵ��׵�ַ
���أ���
*******************************************************************/
void SD_WriteData(Uint32 SectorNumber,Uint16 *SendData)
{
	  volatile Uint16 i,j;
	  //asm(" BCLR XF ");//����д��ʱ��,��һ�����ź����������һ���ո�
	 
	  MMC_write(mmc1,512*SectorNumber,SendData,512);
	 // busy_flag_0=MMCST0_1;
	  //����ʱ���ȴ�SD���Ա�̽���(����ϵͳʱ��Ƶ�ʺ�MMC��������SD��ͨ��ʱ��Ƶ����������ʱ��
	  //�������ʱ����Fsd=666.7KHz ��Fsystem=192MHz���������Խ���������鹫ʽ����Fsd=500KHz ��Fsystem=24MHzʱ��������ʱ����9����
	  //�����ߵ�����������˴���ʱ����Ҫ  272
	  for(i=0;i<84;i++)
	  {
	      for (j = 0; j <= 25000; j++)
	      asm("	NOP ");
	  } 

  //asm(" BSET XF");	
}

/******************************************************************
������SD_ReadData(unsigned int SectorNumber,Uint16 *ReceiveData)
���ܣ���ָ���������ж���512�ֽڵ�����
��������������ţ�0~���ݿ����������㣩���洢�������ݵ��׵�ַ
���أ���
*******************************************************************/
void SD_ReadData(Uint32 SectorNumber,Uint16 *ReceiveData)
{
  	MMC_read(mmc1,512*SectorNumber,ReceiveData,512);  
}

/*************************************************************************
������Uint16 ReadSectorNumber_Usable(void)
���ܣ��ӵ�2012159�����ж������õ�������(��Ӧ1G���ĵ����ڶ���������
�����������
���أ����õ�������
***************************************************************************/
Uint32 ReadSectorNumber_Usable(void)
{
  SD_ReadData(10000000,SecCounter);
  
  return((Uint32)((((Uint32)SecCounter[1])<<16)|(Uint32)SecCounter[0]));
}
/*************************************************************************
������void SaveSectorCounter(void)
���ܣ����������һ���������ţ������ڵ�2012159������?�Ա��´ν��Ÿ�����д������
�����������
���أ���
***************************************************************************/
void SaveSectorCounter(void)
{
  SecCounter[0]=(Uint16)SectorCounter;
  SecCounter[1]=(Uint16)(SectorCounter>>16);
  SD_WriteData(10000000,SecCounter);
}
/****************************************************
������void MMC_Init(void)
���ܣ���ʼ��MMC������
�����������
���أ���
****************************************************/
void MMC_Init(void)
{
  volatile short SD_jishu;
  //ע������5509a��MMC��������ֻ֧��MMC����SD��
  mmc1 = MMC_open(MMC_DEV1);//ʹ��MMC0������
  MMC_setupNative(mmc1,&Init);//���ճ�ʼ��������ʼ��MMC0������

  MMC_sendGoIdle(mmc1);//��λ�����ϵ�˵�д洢��
  for (count=0;count<4016;count++)
  {
  	asm(" NOP ");
  }
  SD_jishu = 0;
  do
  {
	  cardtype = MMC_sendOpCond(mmc1,0x00100000);//������ѹ���ޣ�3.2~3.3V����ͬ�Ŀ��Ը������Ӧ��ֵ��ͬ��MMC����������Ӧ��ֵ���жϸô洢������
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
     //��⵽ΪSD��
     cid = &cardid;
     SD_sendAllCID(mmc1,cid); // get the CID structure for all cards.     
     card = &cardalloc;
     rca = SD_sendRca(mmc1,card);
     SD_status = 1;
  
	                       
	  retVal = MMC_selectCard(mmc1,card);//ѡ��MMC0����SD������ͨ��
	  
	  SD_setWidth(mmc1, 0x4);//MMC��������SD����ͨ�����ݿ��Ϊ4bits
  }
}   

void uart_config()
{
	memset(SD_write,0,sizeof(short)*256);
	SD_write[0] = 0x0ee;   //�����ֽ�ͷ
	SD_write[1] = transponder_mode;//����ģʽ
	SD_write[2] = tongbu_flag;//����λ
	SD_write[3] = 0x0bb;//У��λ
	SD_write[4] = (NOISE_TH_NEW) & 0x0ff;//������޵�8bit
	SD_write[5] = (NOISE_TH_NEW>>8) & 0x0ff;//������޸�8bit
	SD_write[6] = responder_period/10;//У��λ
	SD_write[7] = 0x0cc;//У��λ
	SD_write[8] = ((Time_Delay/1000)) & 0x0ff;//ת��ʱ�ӵ�8bit
	SD_write[9] = ((Time_Delay/1000)>>8) & 0x0ff;//ת��ʱ�Ӹ�8bit
	SD_write[10] = 0x0cc;//У��λ
	SD_write[11] = 0x01;//У��λ
	SD_write[12] = (voltage_AD) & 0x0ff;//��ѹ����ֵ��8bit
	SD_write[13] = (voltage_AD>>8) & 0x0ff;//��ѹ����ֵ��8bit
	SD_write[14] = 0x01;//У��λ
	SD_write[15] = (RS485_DATA[5]) & 0xff;//SD_write[15] = (RS485_DATA[5]) & 0xff;
	SD_write[16] = ((RS485_DATA[4])) & 0xff;//SD_write[16] = ((RS485_DATA[4])) & 0xff
	SD_write[17] = ((RS485_DATA[3])) & 0xff;//SD_write[17] = ((RS485_DATA[3])) & 0xff
	SD_write[18] = ((RS485_DATA[2])) & 0xff;//SD_write[18] = ((RS485_DATA[2])) & 0xff
	SD_write[19] = ((Uint16)(temperature/0.5)) & 0xff;//�¶�����ֵ��8bit
	SD_write[20] = (((Uint16)(temperature/0.5))>>8) & 0xff;//�¶�����ֵ��8bit
	SD_write[21] = (noise_10ms) & 0x0ff;//��������ֵ��8bit
	SD_write[22] = (noise_10ms>>8) & 0x0ff;//��������ֵ��8bit
	SD_write[23] = 0x00;//FSKƵ��1---9kHz
	SD_write[24] = 0x08;//FSKƵ�2---9+8*0.5=13kHz
	SD_write[25] = 0x0ff;//��
	SD_write[26] = SD_status;//SD��״̬��1Ϊ������0Ϊ������
 	SD_write[27] = 0x0ff;//У��λ
}
