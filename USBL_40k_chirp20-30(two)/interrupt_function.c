/*DMA接收中断函数
  函数功能：接收成功标志置位
*/
interrupt void myhDma_Isr() 
{
	DMAnum++;//DMA计数器自加

	DMAdone = TRUE;//接收完成标志

	
	DMA_FSETH(myhDma,DMACSR,FRAME,0);//帧中断标志清零
	

}

/*DMA0发射中断函数
  函数功能：关闭定时器1，发送成功标志位置位
*/
interrupt void dma0_Isr()//发送信号完毕
{
   TCR1 = 0x0C34;//TSS,TLB=1 停止触发DMA0发送,关闭定时器1
   IODATA = 0x00;//GPIO输出为低
   FLAGSENDOVER = 1;//发送完毕
   if(DMA_RGETH(hDmaSend0,DMACSR))
   {
   ;    /* Dummy check to avoid C++ compiler warning */
   }//DMACSR清零
}

/*定时器0中断函数
  函数功能：定时器1s触发一次同步脉冲
*/
interrupt void timer0_Isr(void)
{
    timer0_cnt++;
	if(timer0_cnt == responder_period)//信标模式下同步周期的值等于100ms的计数器的计数个数//延时完成
	{
		timer0_cnt = 0;
		Flag_timer = 1;
	}
}
/*定时器1中断函数
  函数功能：定时器采集计数
*/
interrupt void timer1_Isr()
{
   timer1_cnt++;//定时1us计数器自加
}


/*DMA0发射中断函数
  函数功能：关闭定时器1，发送成功标志位置位
*/
interrupt void int0_Isr()//发送信号完毕
{
   if(transponder_mode == 2)//信标模式
   {
		count_timer++;
	if(count_timer == 3)   //原始值为3
		{
			count_timer = 0;
			TIMER_start(mhTimer0);
			IER0 &= 0x0fffb;
	  		IFR0 = 0x0ffff;//使能int0中断
	  	}
   }

   if(transponder_mode == 3)//电触发模式
   {
		responder_flag = 1;
   }
}


/*串口中断函数
  函数功能：一次接收24个字节，校验接收正确与否
*/
interrupt void int1_Isr()
{

    short i,R_IER,wait;
	UART_IER232 = 0x00;//中断使能寄存器清零
    R_IER = UART_IIR232;//读中断寄存器
	asm(" nop ");
    for(i = 0; i < 12; i++)
    { 
		PC_DATA[i] = UART_RHR232;//将串口接收数据存在PC_DATA
		for(wait = 0; wait < 300; wait++)
		{
			asm(" nop ");
		}
	}


	if((PC_DATA[0]==0x0aa) && (PC_DATA[3]==0x0bb)  && (PC_DATA[7]==0x0cc) && (PC_DATA[10]==0x0cc) && (PC_DATA[11]==0x0dd))//校验串口数据
	{

		FLAG_RS232_REVOK = 1;//接收串口数据成功
		if(PC_DATA[1] == 0x01)
		{
			transponder_mode = 1;
			
		}
		else if(PC_DATA[1] == 0x02)
		{
			transponder_mode = 2;

		}
		else if(PC_DATA[1] == 0x03)
		{
			transponder_mode = 3;

		}
		else
		{
			transponder_mode = 1;
		}

		if(PC_DATA[2] == 0x01)
		{
			tongbu_flag = 1;
		}
		else
		{
			tongbu_flag = 0;
		}
	}
//	memset(PC_DATA,0,sizeof(short)*12);
	UART_LCR232 = 0x00;//设置传输数据格式
	asm(" nop ");
    UART_FCR232 = 0x0ff;//清零FIFO控制寄存器
	asm(" nop ");
    UART_LCR232 = 0x03;//设置传输数据格式
	asm(" nop ");
	UART_IER232 = 0x01;//打开下次中断使能
	asm(" nop ");
}



interrupt void int2_Isr()//一次中断读28个数
{    
    short i,R_IER,wait;	
	//设置一个IO口测试程序运行时间共运行42.54us
	//TIMER_stop(mhTimer1);

    UART_IER485=0x00;
    R_IER=UART_IIR485;
	asm(" nop ");
    for(i=0;i<12;i++)
    { 
		RS485_DATA[i] = UART_RHR485;
		for(wait=0;wait<300;wait++)
		{
			asm(" nop ");
		}
	}
	if(RS485_DATA[0]==0x0FA && RS485_DATA[1]==0x030)
	{
		FLAG_yl_init = 1;
	}
	if(RS485_DATA[0]==0x0FA && RS485_DATA[1]==0x04a)
	{
		FLAG_yl = 1;//FLAG_yl_data
	}
	UART_LCR485=0x00;
	asm(" nop ");
    UART_FCR485=0x0ff;
	asm(" nop ");
    UART_LCR485=0x03;
	asm(" nop ");
	UART_IER485=0x01;//打开下次中断使能
	asm(" nop ");
	//TIMER_start(mhTimer1);
} 


/*无中断函数
  函数功能：空操作
*/
interrupt void no_isr(void)
{
	asm(" nop ");
}


