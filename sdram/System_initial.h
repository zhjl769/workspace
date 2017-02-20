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
   UART_TCR232 = 0x08;//RCVFIFO设置为32字节
   asm(" nop ");
   UART_TLR232 = 0x06f;//RCVFIFO中断触发级别0x06*4=24，24个字节触发一次中断
   asm(" nop ");
   UART_LCR232 = 0x03;//设置数据格式：无奇偶校验，1比特停止位，8比特数据位
   asm(" nop ");
   UART_IER232 = 0x01;//使能FIFO接收中断
   asm(" nop ");
}


void System_initial()
{

	CLKMD = 0x2C92;//设置系统时钟为200MHz 0x2C92

	EBSR=0x0201;//设置为全部EMIF模式，无保持信号应答
	EMI_RST=0x0000;

	EGCR = EGCR & 0x0ffdf;//SDRAM初始化
    EGCR = 0x020f;//CLKMEM=CPU时钟的一半；不响应外部总线征用请求
	CE2_1 = 0x3000;//16bit宽度SDRAM;
	CE3_1 = 0x3000;//16bit宽度SDRAM;       
	SDC1 = 0x5958;//TRC=11,tRC=120ns;SDSIZE=00,4M*16bit;RFEN=1,EMIF对SDRAM进行刷新;TRCD=5,tRCD=60ns;TRP=8,tRP=90ns;
	SDC2 = 0x028f;//SDACC=0,EMIF总线宽度为16bit;TMRD=2,tMRD=30ns;TRAS=8,tRAS=90ns;TACTV2ACTV=15,tRRD=160ns
    SDPER = 0x061a;//PERIOD=15620ns
    EGCR = EGCR|0x0020;//CLKMEM引脚输出时钟使能
    INIT = 0x0000;//初始化SDRAM

	CE0_1=0x103f;//16bit宽度异步存储器;读建立时间5ns;读选通时间75ns;读保持时间15ns
	CE0_2=0x00ff;//读延长保持时间5ns;写延长保持时间5ns;写建立时间5ns;写选通时间315ns;写保持时间15ns
	CE0_3=0x00ff;//超时时间为256个CPU时钟

	IODOR=0xff;//设置GPIO为输出
	IODATA=0xff; //输出为低

	
	IVPH=0x00fb; 
	IVPD=0x00fb;
	IFR0=0x0ffff;
	IFR1=0x0ffff;
	IER0=0x0000;//此处仅仅检测压电数据，使能MCBSP1的接收中断，使能DMA0（发射），int2定时同步中断，定时器0延迟中断
	IER1=0x0000;//使能串口接收中断int1

	receive_752_init();	
}






