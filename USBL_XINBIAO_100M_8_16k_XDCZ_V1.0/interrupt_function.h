/*DMA�����жϺ���
  �������ܣ����ճɹ���־��λ*/
interrupt void myhDma_Isr()
{
	DMAnum++;//DMA�������Լ�
	DMAdone = TRUE;//������ɱ�־
	DMA_FSETH(myhDma,DMACSR,FRAME,0);//֡�жϱ�־����
}

/*DMA0�����жϺ���
  �������ܣ��رն�ʱ��1�����ͳɹ���־λ��λ*/
interrupt void dma0_Isr()//�����ź����
{
   TCR1 = 0x0C34;//TSS,TLB=1 ֹͣ����DMA0����,�رն�ʱ��1
   IODATA = 0x00;//GPIO���Ϊ��
   FLAGSENDOVER = 1;//�������
   if(DMA_RGETH(hDmaSend0,DMACSR))
   {
   ;    /* Dummy check to avoid C++ compiler warning */
   }//DMACSR����
}
interrupt void dma3_Isr()//�����ź����(CW�ź�ר��)
{
	DMA_send_over_flag = 1;//�������
	if(DMA_RGETH(hDmaSend3,DMACSR))
	{
		;    /* Dummy check to avoid C++ compiler warning */
	}//DMACSR����
}
interrupt void dma1_Isr()//�����ź����
{

	//int i;
	Frame_count++;
	if(Frame_count<3240)
	{
		DMA_FSETH(hDmaSend1,DMACCR,ENDPROG,1);//��������һ֡
		DMA_frame_send_over_flag = 1;//֡������ϱ�־λ
	}
	else
	{
		TIMER_stop(mhTimer1);;//�رն�ʱ��1��ֹͣ����DMA
		DMA_stop(hDmaSend1);
		GPIO_RSET(IODATA,0x00);//GPIO���Ϊ��
		DMA_send_over_flag = 1;//�������
	}

	if(DMA_RGETH(hDmaSend1,DMACSR))
	{
		;    /* Dummy check to avoid C++ compiler warning */
	}//DMACSR����
}


/*��ʱ��0�жϺ���
  �������ܣ���ʱ��1s����һ��ͬ������*/
interrupt void timer0_Isr(void)
{
    timer0_cnt++;
	if(timer0_cnt == responder_period)
	{
		timer0_cnt = 0;
		Flag_timer = 1;
	}
}

/*��ʱ��1�жϺ���
  �������ܣ���ʱ���ɼ�����*/
interrupt void timer1_Isr()
{
   timer1_cnt++;//��ʱ1us�������Լ�
}


/*DMA0�����жϺ���
  �������ܣ��رն�ʱ��1�����ͳɹ���־λ��λ*/
interrupt void int0_Isr()//�����ź����
{
   if(main_node == 1)     //��Բ���ģʽ
   {
		DMA_count = 0;
		SPCR2_0 = 0x00c1; //��McBSP
		SPCR1_0 = 0x0001;
		DMA_start(myhDma);
   }

   if(transponder_mode == 2)//�ű�ģʽ
   {
		count_timer++;
		if(count_timer == 3)
		{
			count_timer = 0;
			//TIMER_start(mhTimer0);
			IER0 &= 0x0fffb;
	  		IFR0  = 0x0ffff;//ʹ��int0�ж�
	  	}
   }

   if(transponder_mode == 3)//�紥��ģʽ
   {
		/*�紥������*/
		responder_flag = 1;
		DMANum = DMA_DMACDAC2;
//		IER0 &= 0x0fffb;    //�ر��ⲿ�ж�0
//		IFR0  = 0x0ffff;
   }

}

/*�����жϺ���
  �������ܣ�һ�ν���24���ֽڣ�У�������ȷ���
*/
interrupt void int1_Isr()
{
    short i,wait;//,R_IER

	UART_IER232 = 0x00;//�ж�ʹ�ܼĴ�������
    //R_IER = UART_IIR232;//���жϼĴ���
	asm(" nop ");
    for(i = 0; i < 17; i++)
    {
		PC_DATA[i] = UART_RHR232;//�����ڽ������ݴ���PC_DATA
		for(wait = 0; wait < 300; wait++)
		{
			asm(" nop ");
		}
	}

	if((PC_DATA[0]==0x0aa) && (PC_DATA[16]==0x0bb))//У�鴮������
	{

		FLAG_RS232_REVOK = 1;//���մ������ݳɹ�

        if((PC_DATA[1]<24) && (PC_DATA[2]<24))
		{
	        RAMSES_ref_CIF = PC_DATA[1];  //�ο��ź�CIF
			RAMSES_ref_IIF = PC_DATA[2];  //�ο��ź�IIF
		}

        if((PC_DATA[3]<17) &&  (PC_DATA[4]<17))
		{
			ID1 = PC_DATA[3];  //Ƶ��1
			ID2 = PC_DATA[4];  //Ƶ��2
		}

        //��������ִ������
		if(PC_DATA[9]<13)
		{
			cmd_indx = PC_DATA[9];
			sum_count_FSK_cmd1[cmd_indx]=10;
		}

        //��ģʽ���紥��ģʽ
		if((PC_DATA[10]>0) && (PC_DATA[10]<4))
		{
			transponder_mode = PC_DATA[10];
		}

        //ˮ�¹���ģʽѡ��1����Բ���2����վ��3����վ
		if((PC_DATA[11]>0) && (PC_DATA[11]<4))
		{
			main_node = PC_DATA[11];
		}

        // ��Բ������
		if((PC_DATA[12]>0) && (PC_DATA[12]<51))
		{
			XDCZ_num = PC_DATA[12];
		}

        // ��վ��������
		if((PC_DATA[13]>0) && (PC_DATA[13]<31))
		{
			responder_period = PC_DATA[13];
		}

	}

//	memset(PC_DATA,0,sizeof(short)*12);
	UART_LCR232 = 0x00;//���ô������ݸ�ʽ
	asm(" nop ");
    UART_FCR232 = 0x0ff;//����FIFO���ƼĴ���
	asm(" nop ");
    UART_LCR232 = 0x03;//���ô������ݸ�ʽ
	asm(" nop ");
	UART_IER232 = 0x01;//���´��ж�ʹ��
	asm(" nop ");
}



interrupt void int2_Isr()//һ���ж϶�28����
{
	//����һ��IO�ڲ��Գ�������ʱ�乲����42.54us
    short i,wait;	 //,R_IER

    UART_IER485=0x00;
//    R_IER=UART_IIR485;
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
	UART_IER485=0x01;//���´��ж�ʹ��
	asm(" nop ");
}


/*���жϺ���  �������ܣ��ղ���*/
interrupt void no_isr(void)
{
	asm(" nop ");
}


