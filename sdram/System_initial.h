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
   UART_TCR232 = 0x08;//RCVFIFO����Ϊ32�ֽ�
   asm(" nop ");
   UART_TLR232 = 0x06f;//RCVFIFO�жϴ�������0x06*4=24��24���ֽڴ���һ���ж�
   asm(" nop ");
   UART_LCR232 = 0x03;//�������ݸ�ʽ������żУ�飬1����ֹͣλ��8��������λ
   asm(" nop ");
   UART_IER232 = 0x01;//ʹ��FIFO�����ж�
   asm(" nop ");
}


void System_initial()
{

	CLKMD = 0x2C92;//����ϵͳʱ��Ϊ200MHz 0x2C92

	EBSR=0x0201;//����Ϊȫ��EMIFģʽ���ޱ����ź�Ӧ��
	EMI_RST=0x0000;

	EGCR = EGCR & 0x0ffdf;//SDRAM��ʼ��
    EGCR = 0x020f;//CLKMEM=CPUʱ�ӵ�һ�룻����Ӧ�ⲿ������������
	CE2_1 = 0x3000;//16bit���SDRAM;
	CE3_1 = 0x3000;//16bit���SDRAM;       
	SDC1 = 0x5958;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF��SDRAM����ˢ��;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
	SDC2 = 0x028f;//SDACC=0,EMIF���߿��Ϊ16bit;TMRD=2,tMRD=30ns;TRAS=8,tRAS=90ns;TACTV2ACTV=15,tRRD=160ns
    SDPER = 0x061a;//PERIOD=15620ns
    EGCR = EGCR|0x0020;//CLKMEM�������ʱ��ʹ��
    INIT = 0x0000;//��ʼ��SDRAM

	CE0_1=0x103f;//16bit����첽�洢��;������ʱ��5ns;��ѡͨʱ��75ns;������ʱ��15ns
	CE0_2=0x00ff;//���ӳ�����ʱ��5ns;д�ӳ�����ʱ��5ns;д����ʱ��5ns;дѡͨʱ��315ns;д����ʱ��15ns
	CE0_3=0x00ff;//��ʱʱ��Ϊ256��CPUʱ��

	IODOR=0xff;//����GPIOΪ���
	IODATA=0xff; //���Ϊ��

	
	IVPH=0x00fb; 
	IVPD=0x00fb;
	IFR0=0x0ffff;
	IFR1=0x0ffff;
	IER0=0x0000;//�˴��������ѹ�����ݣ�ʹ��MCBSP1�Ľ����жϣ�ʹ��DMA0�����䣩��int2��ʱͬ���жϣ���ʱ��0�ӳ��ж�
	IER1=0x0000;//ʹ�ܴ��ڽ����ж�int1

	receive_752_init();	
}






