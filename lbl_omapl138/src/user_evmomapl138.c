#include "user_evmomapl138_header.h"

/*-----------------------------------------**
** 	�û�ʹ�õ�Ӳ����ʼ����������           **
** 	���ӿ��ļ���	evmomapl138_bsl.lib	   **
**-----------------------------------------*/

//�û������ӳٺ�������λ11us
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
									   
void evm_init_omapl138()			//Ӳ��ȫ�ֳ�ʼ����������û��GEL�ļ������
{
	// configure power, sysconifg, and clocks.
	init_psc_user();
	init_clocks_user();
}

void evm_init_omapl138_DDR2()		//DDR2ʹ�ó�ʼ����������û��GEL�ļ������
{
   // unlock the system config registers and set the ddr 2x clock source.
   SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
   SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
   CLRBIT(SYSCONFIG->CFGCHIP[3], CLK2XSRC);
   //���ϲ�������ɲ���1,�ƺ�PLL1�Ѿ��������1

   // enable emif3a clock.ʵ�ʾ���DDR2��ʱ��Դ�򿪣���ɲ���2
   EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_EMIF3A, PSC_ENABLE);
   
   // check if vtp calibration is enabled.
   //���POWERDNλ����λ����ʾû�н������ģʽ��VTPû�б���ʼ������Ҫ����һ�γ�ʼ����
   if (CHKBIT(VTPIO_CTL, 0x00000040))
   {
      // vtp cal disabled, begin cal.

      // enable input buffer and vtp.
	  //����IOPWRDNλ
      SETBIT(VTPIO_CTL, 0x00004000);
	  //���POWERDNλ
      CLRBIT(VTPIO_CTL, 0x00000040);
      
      // pulse clrz to init vtp cal.����CLKRZ��־λ
      SETBIT(VTPIO_CTL, 0x00002000);
      CLRBIT(VTPIO_CTL, 0x00002000);
      SETBIT(VTPIO_CTL, 0x00002000);
      
      // poll ready bit to wait for cal to complete.�ȴ�READYλ����λ
      while (!CHKBIT(VTPIO_CTL, 0x00008000)) {}
      
      // set lock and power save bits.
      SETBIT(VTPIO_CTL, 0x00000180);
   }

   // config ddr timing.
   DDR->DDRPHYCTL1 = 0x000000C4;//�ⲿѡͨ��ģʽ���������ص硢RL=4
   
   //���SDCRλ������ֵ���ڴ�ʱ����������
   //DDR2TERM1			=1(��λֵ��û����)
   //BOOTUNLOCK			=1(���ģ���ʾ����)дĳЩλʱ���뽫��һλ��λ
   //DDR2EN 			=1(��λֵ��û����)
   //DDREN				=1(��λֵ��û����)
   //SDRAMEN			=1(��λֵ��û����)
   //TIMUNLOCK			=1(���ģ���ʾ����)дCLʱ���뽫��һλ��λ
   //NM					=1(��λֵ��û����)
   //CL					=3(����ֵ�����ⲿ�洢��оƬ�й�)
   //IBANK				=3(8bank�����ⲿ�洢��оƬ�й�)
   //PAGESIZE			=2(10bit�е�ַ�����ⲿ�洢��оƬ�й�)
   DDR->SDCR = 0x0893C632;
   //�޸�DDR2ENΪ0ֵ����ʱBOOTUNLOCKΪ1��ʾ����
   DDR->SDCR &= ((DDR->SDCR & 0xFF0FFFFF) | 0x00800000);
   //�޸���DDR2ENΪ1ֵ,��ʱBOOTUNLOCKΪ0��ʾ�޸ĳɹ�������
   DDR->SDCR = ((DDR->SDCR & 0xFF0FFFFF) | 0x00100000);
   //�޸���DDR2��������CLֵ����ʱTIMUNLOCKΪ0��ʾ�޸ĳɹ�������
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
	timer_x->TGCR = 0x00000000;             //��ʼ��
	timer_x->TCR = 0x00000000;               //�ڲ�ʱ��,��PLL����ʱ����ͬ��24MHz

	// interrupts and set emulation to free run.
	timer_x->INTCTLSTAT = 0;
	SETBIT(timer_x->EMUMGT, SOFT | FREE);

	// config timer0 in 32-bit unchained mode.      ���������Ķ�ʱ��  timer 3:4 ��timer 1:2
	// remove timer0 - 3:4 from reset.                   ���ֵ�ǰֵ
	SETBIT(timer_x->TGCR, TIMMODE_32BIT_UNCHAINED | TIM12RS );	
		
	// Reset the Counter for Timer0:12
	SETBIT(timer_x->TIM12, CSL_TMR_TIM12_TIM12_RESETVAL);

	// Select Internal Clock for Timer0:12 (24 MHz)
	//timer_x->TCR = 0x00000100;                      //External clock on TM64P_IN12
	// Set Timer0:12 Period :�����ʱ��ΪXus����ʱ����Ϊ1/24000000s����д��ֵN = ��X*10e-6��/��1/24000000�� = X*24
	prd_timer = user_timer*24 - 1;
	SETBIT(timer_x->PRD12, prd_timer);

//	timer_x->INTCTLSTAT = 0x00000001;
	// Disable the New Timer Features �Ȳ���Ҫtimer_x->INTCTLSTAT�������ж�
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
		
	// Reset the Counter for Timer0:12          û��prescaler
	SETBIT(timer_x->TIM12, CSL_TMR_TIM12_TIM12_RESETVAL);

	// Select �ⲿʱ�� Clock for Timer0:12 (10MHz)
	timer_x->TCR = 0x00000100;
	// Set Timer0:12 Period :�����ʱ��ΪXus����ʱ����Ϊ1/10000000s����д��ֵN = ��X*10e-6��/��1/10000000�� = X*10
	prd_timer = user_timer*10 - 1; //10->24�Ƿ���ȷ������
	SETBIT(timer_x->PRD12, prd_timer);

//	timer_x->INTCTLSTAT = 0x00000001; //������ʱ�ж�
	// Disable the New Timer Features
	SETBIT(timer_x->TGCR, (CSL_TMR_TGCR_PLUSEN_DISABLE<<CSL_TMR_TGCR_PLUSEN_SHIFT));
	
//	SETBIT(timer_x->TCR, ENAMODE12_CONT);  �ڷ����������򿪶�ʱ��12
}

void evm_init_mcbsp()
{
	// enable the psc and config pinmux for mcasp1.
	EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MCBSP1, PSC_ENABLE);
	EVMOMAPL138_pinmuxConfig(PINMUX_MCBSP1_REG, PINMUX_MCBSP1_MASK, PINMUX_MCBSP1_VAL);

	// configure receive registers (32 bit, single phase, no delay).
	//�������ݸ�ʽ������֡RPHASE=0����2λbit30~21=0ȫ����Ϊ0
	//�����ݸ�ʽ����MSB�ȴ�RCOMPAND=0�����Բ�������֡ͬ���ź�RFIG=0
	//��1����Ϊ���������ӳ�1bit���գ�1֡����һ���֣�һ���ֳ�16bit
 	//�ڵ�һ����������������������ʼ����֡ͬ������
	MCBSP1->RCR = 0x00010040;
	//���ݵĳ���Ϊ16bit
	MCBSP1->XCR = 0x00000040;
	// ֡�����ź�FSX�����ڲ�����FSXM=1������AD���ղ�����ת��;  ֡�����ź�FSR�����ⲿBUSY�ź�����FSRM=0
	//���ͽ���ʱ�����ڲ�����CLKXM=1��CLKRM=1;
	//����MCBSP�ⲿ����ʱ����ΪMCBSP������ʱ�ӷ�����ԴSCLKME=0 mcbsp clksm=0 SCLKME=0 �����ⲿʱ��Դ
	//����ADʱ�򣬷���֡ͬ���źŵ���ЧFSXP=1������֡ͬ���źŵ���ЧFSRP=1��
	//������÷���������CLKX�½��ز���CLKXP=0
	//������ý���������CLKR�½��ز���CLKRP=0
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
	//MCBSP��λ���������н�ֹ
	MCBSP1->SPCR = 0x00000000;

	// configure receive registers (32 bit, single phase, no delay).
	//�������ݸ�ʽ������֡RPHASE=0����2λbit30~21=0ȫ����Ϊ0
	//�����ݸ�ʽ����MSB�ȴ�RCOMPAND=0�����Բ�������֡ͬ���ź�RFIG=0
	//��1����Ϊ���������ӳ�1bit���գ�1֡����һ���֣�һ���ֳ�16bit
	//�ڵ�һ����������������������ʼ����֡ͬ������
//	MCBSP1->RCR = 0x00010040;  //***//

	// configure transmit registers (32 bit, single phase, no delay).
	//�������ݸ�ʽ����ʹ�ã��ݲ�����
//	MCBSP1->XCR = 0x00000000;  //***//

	// Disable int frame generation and enable slave w/ext frame signals on FSX
	// Frame sync is active high, data clocked on rising edge of clkx
	// ֡�����ź�FSX�����ڲ�����FSXM=1������AD���ղ�����ת��;  ֡�����ź�FSR�����ⲿBUSY�ź�����FSRM=0
	//���ͽ���ʱ�����ڲ�����CLKXM=1��CLKRM=1;
	//����MCBSP�ڲ�����ʱ����ΪMCBSP������ʱ�ӷ�����ԴSCLKME=0
	//����ADʱ�򣬷���֡ͬ���źŵ���ЧFSXP=1������֡ͬ���źŵ���ЧFSRP=1��
	//������÷���������CLKX�½��ز���CLKXP=0
	//������ý���������CLKR�½��ز���CLKRP=0
//	MCBSP1->PCR = 0x00000B0C;  //***//
	
   //mcbsp clksm=0 SCLKME=0 �����ⲿʱ��Դ CLKGDV=9 FWID=2  FPER=24
   //CLKG=CLKS/(CLKGDV+1)=1M  FPER=1M/(24+1)=40K  ֡������=(FWID+1)*��1/40khz��=75us  
	MCBSP1->SRGR = 0x10180209;  //3    1M 40K
//	MCBSP1->SRGR = 0x3031021D;         // 5M   100kHz
//	MCBSP1->SRGR = 0x10090218;	//3		
//	for(i=0;i<20;i++){}			//4		600clock
	MCBSP1->SPCR = 0x00400000;	//5		
//	for(i=0;i<20;i++){};					//600clock

	SETBIT(MCBSP1->SPCR, 0x00010000);	//6             Serial port transmitter is enabled
	//����ȴ�һ����ʱ�䣬��ʹ�ܣ��ٽ�ֹ����ֹ���ֵ�һ���󴥷�
	for(i=0;i<210;i++){}					//1200clock
	CLRBIT(MCBSP1->SPCR, 0x00010000);
	//�����һ����֮����뽨��EDMA��ʹ��
	open_EDMA_E4(Edma30cc_0_RegsOvly);		//7

//	MCBSP1->SPCR = 0x00410001;
	MCBSP1->SPCR = 0x00C10001;                               //The serial port receiver is enabled��Serial port transmitter is enabled

//	SETBIT(MCBSP1->SPCR, 0x00010001);	//8
//	SETBIT(MCBSP1->SPCR, 0x00800000);	//11
}

void init_PaRAM_event4 (CSL_Edma3ccRegs *edma3ccRegs)
{
	// Reset EDMA PaRAM OPT Register
	//��λEDMA�Ĳ�����4������ֵΪ0x00000000
	//���е�EDMA_EVENT4������EDMA_EVENT10
	edma3ccRegs->PARAMSET[EDMA_EVENT4].OPT = CSL_EDMA3CC_OPT_RESETVAL;
	
	// Config PaRAM OPT (Enable TC Interrupt; Set TCC)
	//Ȼ�����ò�����4��OPT�Ĵ�����
	//���ô�������ж�ʹ�ܣ�ͬʱ���ô�����ɴ���ΪTCC=4��
	//Ĭ�ϲ���Aͬ��ģʽ��Ŀ�ĵ�ַ����ģʽ
	edma3ccRegs->PARAMSET[EDMA_EVENT4].OPT = 0x00104000;
	edma3ccRegs->PARAMSET[EDMA_EVENT4].SRC = 0x01D11000;//(uint32_t)&(MCBSP1->DDR)
	//0xC0080000;//���ù̶�����ʼ��ַDDR2��ַ;
	// Set EDMA Event PaRAM A,B,C CNT
	//���ô����¼��Ĵ�������������֡��С�������ʽ
	//�˴δ���ĸ�ʽ���£�����ACNTΪ2��2���ֽڳ��ȣ�������BCNTΪ512������CCNTΪ1
	//��ʽ��    	(BCNT)|(ACNT)
	//A��ʾһ���¼��ĵ�Ԫ������B��ʾ���и�����C��ʾ֡����
//	edma3ccRegs->PARAMSET[EDMA_EVENT4].A_B_CNT = 0x03700002;  //880
	edma3ccRegs->PARAMSET[EDMA_EVENT4].A_B_CNT = 0x02000002;
	// Initialize EDMA Event Src and Dst Addresses
	//����ͨ�������Դ��ַ��Ϣ��Ŀ�ĵ�ַ��Ϣ
	edma3ccRegs->PARAMSET[EDMA_EVENT4].DST = (uint32_t)&buffer1;
	// Set EDMA Event PaRAM SRC/DST BIDX
	//����Դ��ַ/Ŀ�ĵ�ַ�ĵ�Ԫ�Լ�����Ϊ2/0���ֽ�:  (DSTBIDX)|(SRCBIDX)
	edma3ccRegs->PARAMSET[EDMA_EVENT4].SRC_DST_BIDX = 0x00020000;
	// Set EDMA Event PaRAM LINK and BCNTRLD
	//����EDMA�Ĳ��������ӵ�ַ��BCNTRLDֵ
	//���������ӣ�ͬʱ����BCNTRLDΪ0
	edma3ccRegs->PARAMSET[EDMA_EVENT4].LINK_BCNTRLD = (1024<<16)|PaRAM_pang_74;
	// Set EDMA Event PaRAM SRC/DST CIDX
	//����֡����ֵΪ0
	edma3ccRegs->PARAMSET[EDMA_EVENT4].SRC_DST_CIDX = 0x00000000;
	edma3ccRegs->PARAMSET[EDMA_EVENT4].CCNT = 0x0001;	
}

void init_PaRAM_event74 (CSL_Edma3ccRegs *edma3ccRegs)
{
	// Reset EDMA PaRAM OPT Register
	//��λEDMA�Ĳ�����4������ֵΪ0x00000000
	edma3ccRegs->PARAMSET[EDMA_EVENT74].OPT = CSL_EDMA3CC_OPT_RESETVAL;
	
	// Config PaRAM OPT (Enable TC Interrupt; Set TCC)
	//Ȼ�����ò�����4��OPT�Ĵ�����
	//���ô�������ж�ʹ�ܣ�ͬʱ���ô�����ɴ���ΪTCC=4��
	//Ĭ�ϲ���Aͬ��ģʽ��Ŀ�ĵ�ַ����ģʽ
	edma3ccRegs->PARAMSET[EDMA_EVENT74].OPT = 0x00104000;
	edma3ccRegs->PARAMSET[EDMA_EVENT74].SRC = 0x01D11000;//(uint32_t)&(MCBSP1->DDR);
	// Set EDMA Event PaRAM A,B,C CNT
	//���ô����¼��Ĵ�������������֡��С�������ʽ
	//�˴δ���ĸ�ʽ���£�����ACNTΪ2������BCNTΪ1024������CCNTΪ1
	//��ʽ��    	(BCNT)|(ACNT)
	//A��ʾһ���¼��ĵ�Ԫ������B��ʾ���и�����C��ʾ֡����
//	edma3ccRegs->PARAMSET[EDMA_EVENT74].A_B_CNT = 0x03700002;
	edma3ccRegs->PARAMSET[EDMA_EVENT74].A_B_CNT = 0x02000002;
	// Initialize EDMA Event Src and Dst Addresses
	//����ͨ�������Դ��ַ��Ϣ��Ŀ�ĵ�ַ��Ϣ
	edma3ccRegs->PARAMSET[EDMA_EVENT74].DST = (uint32_t)&buffer2;
	// Set EDMA Event PaRAM SRC/DST BIDX
	//����Դ��ַ/Ŀ�ĵ�ַ�ĵ�Ԫ�Լ�����Ϊ2/0���ֽ�:  (DSTBIDX)|(SRCBIDX)
	edma3ccRegs->PARAMSET[EDMA_EVENT74].SRC_DST_BIDX = 0x00020000;
	// Set EDMA Event PaRAM LINK and BCNTRLD
	//����EDMA�Ĳ��������ӵ�ַ��BCNTRLDֵ
	//���������ӣ�ͬʱ����BCNTRLDΪ0
	edma3ccRegs->PARAMSET[EDMA_EVENT74].LINK_BCNTRLD = (1024<<16)|PaRAM_ping_75;
	// Set EDMA Event PaRAM SRC/DST CIDX
	//����֡����ֵΪ0
	edma3ccRegs->PARAMSET[EDMA_EVENT74].SRC_DST_CIDX = 0x00000000;
	edma3ccRegs->PARAMSET[EDMA_EVENT74].CCNT = 0x0001;
}

void init_PaRAM_event75 (CSL_Edma3ccRegs *edma3ccRegs)
{
	// Reset EDMA PaRAM OPT Register
	//��λEDMA�Ĳ�����4������ֵΪ0x00000000
	edma3ccRegs->PARAMSET[EDMA_EVENT75].OPT = CSL_EDMA3CC_OPT_RESETVAL;
	
	// Config PaRAM OPT (Enable TC Interrupt; Set TCC)
	//Ȼ�����ò�����4��OPT�Ĵ�����
	//���ô�������ж�ʹ�ܣ�ͬʱ���ô�����ɴ���ΪTCC=4��
	//Ĭ�ϲ���Aͬ��ģʽ��Ŀ�ĵ�ַ����ģʽ
	edma3ccRegs->PARAMSET[EDMA_EVENT75].OPT = 0x00104000;
	edma3ccRegs->PARAMSET[EDMA_EVENT75].SRC = 0x01D11000;//(uint32_t)&(MCBSP1->DDR);
	// Set EDMA Event PaRAM A,B,C CNT
	//���ô����¼��Ĵ�������������֡��С�������ʽ
	//�˴δ���ĸ�ʽ���£�����ACNTΪ2������BCNTΪ1024������CCNTΪ1
	//��ʽ��    	(BCNT)|(ACNT)
	//A��ʾһ���¼��ĵ�Ԫ������B��ʾ���и�����C��ʾ֡����
//	edma3ccRegs->PARAMSET[EDMA_EVENT75].A_B_CNT = 0x03700002;
	edma3ccRegs->PARAMSET[EDMA_EVENT75].A_B_CNT = 0x02000002;
	// Initialize EDMA Event Src and Dst Addresses
	//����ͨ�������Դ��ַ��Ϣ��Ŀ�ĵ�ַ��Ϣ
	edma3ccRegs->PARAMSET[EDMA_EVENT75].DST = (uint32_t)&buffer1;
	// Set EDMA Event PaRAM SRC/DST BIDX
	//����Ŀ�ĵ�ַ/Դ��ַ�ĵ�Ԫ�Լ�����Ϊ2/0���ֽ�:  (DSTBIDX)|(SRCBIDX)
	edma3ccRegs->PARAMSET[EDMA_EVENT75].SRC_DST_BIDX = 0x00020000;
	// Set EDMA Event PaRAM LINK and BCNTRLD
	//����EDMA�Ĳ��������ӵ�ַ��BCNTRLDֵ
	//���������ӣ�����BCNTRLDΪ0
	edma3ccRegs->PARAMSET[EDMA_EVENT75].LINK_BCNTRLD = (1024<<16)|PaRAM_pang_74;
	// Set EDMA Event PaRAM SRC/DST CIDX
	//����֡����ֵΪ0
	edma3ccRegs->PARAMSET[EDMA_EVENT75].SRC_DST_CIDX = 0x00000000;
	edma3ccRegs->PARAMSET[EDMA_EVENT75].CCNT = 0x0001;
}

void evm_init_edma_setup(CSL_Edma3ccRegs *edma3ccRegs)
{
	// Clear Event Registers

	//����EDMAʱ����̣�
	//���һECR�Ĵ���д0xffffffff����ʾ������е�DMA�¼�
	CSL_FINST(edma3ccRegs->ECR, EDMA3CC_ECR_REG, MASK);                 //->��ʾʹ��ָ����ʽṹ���Ա
//	edma3ccRegs->ECR = 0x00000010;//���ͨ��4
	//��ڶ�SECR�Ĵ���д0xffffffff����ʾ������е�EDMA�¼�
	CSL_FINST(edma3ccRegs->SECR, EDMA3CC_SECR_REG, MASK);
	//��Ϊ�������¼��Ĵ����������Ҫд�����Ĵ���
//	edma3ccRegs->SECR = 0x00000010;		
	// Enable Channel 4 to DSP (Region 1)
	//ʹ��EDMA3CC0��ͨ��4����DSP�������ݣ�����Ӱ��1��shadow1��
	//ѡ��ͬ�������¼�ΪMCBSP1�����¼�E4����ϸ��ο�OMAP�ĵ�����EDMA�¼���������
	edma3ccRegs->DRA[CSL_EDMA3_REGION_1].DRAE = 0x00000010;
	//edma3ccRegs->DRA[CSL_EDMA3_REGION_1].DRAE = 0x00000400;
	// Assign Channel 4 to Queue 0
	//ʹ��ͨ������0�Ĵ�����DMAQNUM0����E4ͨ�������£��ҽ�Q0��
	edma3ccRegs->DMAQNUM[0] = 0x00000000;
	//edma3ccRegs->DMAQNUM[1] = 0x00000000;

	// Initialize PaRAM Transfer Context for Event 4

	//��ʼ�������������¼�4���䡣
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
	//��һ��������Ҫ�����Ź���
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO0H_REG, PINMUX_GPIO0H_MASK, PINMUX_GPIO0H_VAL);	//�̵��� ģ������
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO3H_REG, PINMUX_GPIO3H_MASK, PINMUX_GPIO3H_VAL);	//CPLD
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO6L_REG, PINMUX_GPIO6L_MASK, PINMUX_GPIO6L_VAL);	//CPLD ����DSP  GPIO6[6] ����
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO6H_REG, PINMUX_GPIO6H_MASK, PINMUX_GPIO6H_VAL);	//CPLD 
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO5L_REG, PINMUX_GPIO5L_MASK, PINMUX_GPIO5L_VAL);	//CPLD PWM
	EVMOMAPL138_pinmuxConfig(PINMUX_GPIO5H_REG, PINMUX_GPIO5H_MASK, PINMUX_GPIO5H_VAL);	//CPLD PWM


	//�ڶ��������PSCģ��ʹ��GPIO����
	EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_GPIO, PSC_ENABLE);

	//����������̷������ݡ��ж����õȼĴ���
	//����ʹ��GPIOBANK[6]��GPIO6�ܽ���Ϊ�ⴥ��������
	*(unsigned volatile int *)GPIO_BINTEN = 0x00000040;//enable BANK6 Interrupt

	GPIO_setDir(GPIO_BANK0, GPIO_PIN13, GPIO_OUTPUT);		  //��GPIO0[13]����Ϊ���
	GPIO_setOutput(GPIO_BANK0, GPIO_PIN13, OUTPUT_LOW);	//����54dB(0)  74dB(1)

	GPIO_BANK23->DIR = 0x00000000;       //������������Ϊ���
	GPIO_BANK45->DIR = 0x00000000;
	GPIO_BANK67->DIR = 0x00000040;		//CPLD����DSP GPIO6[6]       0Ϊ���  1Ϊ����     GPIO6[6]Ϊ����
	GPIO_BANK67->SET_RIS_TRIG = 0x00000040;       //GPIO6[6]����Ϊ�����ش���
	GPIO_BANK67->CLR_FAL_TRIG = 0x00000040;      //�½�����Ӱ��
	GPIO_BANK67->IRQ_STAT = 0x00000040;               //????

	GPIO_setDir(GPIO_BANK5, TRANS_GPIO1, GPIO_OUTPUT);//����GPIO5-9Ϊ���ģʽ
	GPIO_setDir(GPIO_BANK5, TRANS_GPIO2, GPIO_OUTPUT);//����GPIO5-6Ϊ���ģʽ
	GPIO_setDir(GPIO_BANK5, TRANS_SYNC, GPIO_OUTPUT);//����GPIO5-12Ϊ���ģʽ
/*	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO1, OUTPUT_LOW);//����GPIO����͵�ƽ
	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO2, OUTPUT_LOW);//����GPIO����͵�ƽ*/
	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO1, OUTPUT_HIGH);//����GPIO����ߵ�ƽ
	GPIO_setOutput(GPIO_BANK5, TRANS_GPIO2, OUTPUT_HIGH);//����GPIO����ߵ�ƽ
	GPIO_setOutput(GPIO_BANK5,  TRANS_SYNC, OUTPUT_HIGH);//����GPIO����ߵ�ƽ
}

void transmit(uint16_t *signal, int n)
{
	int i,j;
	uint16_t temp1,temp2;

	SETBIT(TMR2->TCR, ENAMODE12_CONT);
	SETBIT(GPIO_BANK45->OUT_DATA, (0x10000000));;//����GPIO����ߵ�ƽGPIO5[12]=1     PWM_Signal_Syn
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
	CLRBIT(GPIO_BANK45->OUT_DATA, (0x02000000));//����GPIO����͵�ƽ
    CLRBIT(GPIO_BANK45->OUT_DATA, (0x00400000));//����GPIO����͵�ƽ
	CLRBIT(GPIO_BANK45->OUT_DATA, (0x10000000));//����GPIO����͵�ƽ
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
	uart->DLH = (divisor & 0x0000FF00) >> 8;                //��8λ
	uart->DLL = divisor & 0x000000FF;                           //��8λ

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

