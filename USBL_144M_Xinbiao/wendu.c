
/******************* ��ʼ��ds18b2�Ӻ���**************************/ 
void Init_DS18B20()                 //����һ��Ӧ���źţ��Ƿ���ɳ�ʼ��
  { 
	volatile short flag;
	volatile Uint32 delay;

	flag = 1;
  	while (flag)
	{
	 IODOR |= 0x80;	//����Ϊ���״̬
	 IODATA |= 0xc0;   //����ߵ�ƽ
	 for(delay = 0;delay < 24;delay++)
	 {
	 }//��ʱ5us
     IODATA &= 0x7f;   //����͵�ƽ
     
     for(delay = 0;delay < 2334;delay++)
	 {
	 }//��ʱ500us                 

     IODATA |= 0xc0;	 //����ߵ�ƽ
                        
	 for(delay = 0;delay < 264;delay++)
	 {
	 }//��ʱ55us

	 IODOR &= 0x7f;	   //����Ϊ����״̬
                    
	 for(delay = 0;delay < 288;delay++)
	 {
	 }//��ʱ60us

     if( (IODATA & 0x80) == 0)
		 flag = 0;		   //Ӧ���źţ��Ƿ���ɳ�ʼ��
                         
	 for(delay = 0;delay <1920;delay++)
	 {
	 }//��ʱ400us	 

	 }
 }

/*******  ���ֽ��Ӻ���*******/     
short Read(void) 
 { 
	short dat;
      short i=0;
      volatile Uint32 delay; 
      dat = 0;
      for (i=8;i>0;i--) 
      { 
	      dat>>=1;   //�������ݺ�����һλ��ֻ���ƶ�7��
        IODATA &= 0x7f;	 //����͵�ƽ
                                       
		 for(delay = 0;delay < 29;delay++)
		 {
		 }//��ʱ6us

        IODATA |= 0x80;
		 for(delay = 0;delay <39;delay++)
		 {
		 }//��ʱ8us
        IODOR &= 0x7f;	    //����Ϊ����״̬
                          
		 for(delay = 0;delay < 24;delay++)
		 {
		 }//��ʱ5us

        if((IODATA&0x80) == 0x80)     // �������Ϊ�ߵ�ƽ����1
	    {
	    	dat|=0x80;			//ȡ�����λ����
		}
		 for(delay = 0;delay < 240;delay++)
		 {
		 }//��ʱ50us

		IODOR |= 0x80;			//����Ϊ���״̬    
		IODATA |= 0x80;    
  
		 for(delay = 0;delay < 48;delay++)
		 {
		 }//��ʱ10us

     }
   return(dat); 
 } 

/*******  д�ֽ��Ӻ���*******/ 
void Write(unsigned char cmd)
  { 
       volatile Uint32 delay;
       short i=0; 
	   IODOR |= 0x80;

	   IODATA &= 0x7f;

		 for(delay = 0;delay < 24;delay++)
		 {
		 }//��ʱ5us

    for (i=8; i>0; i--) 
     {    
	   IODOR |= 0x80;

       IODATA &= 0x7f;

		 for(delay = 0;delay < 29;delay++)
		 {
		 }//��ʱ6us

       if(cmd&0x01)
	      IODATA |= 0x80;                      //д��һλ���� 
	   else
	      IODATA &= 0x7f;
		 for(delay = 0;delay < 240;delay++)
		 {
		 }//��ʱ50us
       IODATA |= 0x80;

		 for(delay = 0;delay < 48;delay++)
		 {
		 }//��ʱ10us
       cmd >>= 1;    	   //����һλ����                               
   } 
} 

/*         �����кź���             */
void read_serial(void)
{
	short i,j;
	volatile Uint32 delay;
	for(j=0;j<8;j++)
	   { 
	     serial[j]=0;
	     for(i=8;i>0;i--) 
	         { 
	          IODOR |= 0x80;	//����Ϊ���״̬ 
              IODATA |= 0x80;    //����ߵ�ƽ                             
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//��ʱ5us

	          serial[j]>>=1;                       //�������ݺ�����һλ
              IODATA &= 0x7f;	   //����͵�ƽ
							  
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//��ʱ5us	                                       

              IODOR &= 0x7f;	   //����Ϊ����״̬
                        
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//��ʱ5us

			  if((IODATA&0x80) == 0x80)
			  {
				  serial[j]|=0x80;				  //ȡ�����λ����
			  }
			 for(delay = 0;delay < 240;delay++)
			 {
			 }//��ʱ50us                              

	         }
	    }
 }

 /******************************************/
/*             �����кź���               */
/******************************************/

void send_serial()
{
	short i,j,temp;
	volatile Uint32 delay; 
     for (j=0;j<8;j++)
        { 
		temp = serial_x[j];
		IODOR |= 0x80;
		IODATA &= 0x7f;

		 for(delay = 0;delay < 10;delay++)
		 {
		 }//��ʱ2us

         for(i=8; i>0; i--) 
            { 
			IODOR |= 0x80;
			IODATA &= 0x7f;

			 for(delay = 0;delay < 24;delay++)
			 {
			 }//��ʱ5us
            //��ʱ5΢�룬���Ϳ�ʼ
             if(temp&0x01)              //д�����λ����
				IODATA |= 0x80;
	   		 else
				 IODATA &= 0x7f;

			 for(delay = 0;delay < 264;delay++)
			 {
			 }//��ʱ55us

 			IODATA |= 0x80;

			 for(delay = 0;delay < 24;delay++)
			 {
			 }//��ʱ5us

             temp>>=1;                  //����һλ����
            } 
        } 
}  
 
/******************************************/
/*            �����¶Ⱥ���                */ 
/******************************************/ 
/*��ȡ�¶���Ϣ
	�������ܣ�����GPIO1����DS18B20��ȡ�¶���Ϣ
*/

void get_temperature()
{
	short  temp_data0,temp_data1,tem_short; 
	short  translate_OK;
	float tt;
	volatile Uint32 delay; 
	temp_data0 = 0;
	temp_data1 = 0;
	tt = 0.0;
	IODOR |= 0x80;
	IODATA |= 0x80;
    
    //******д�Ĵ���******
	// ��ʼ��
	Init_DS18B20();                       
	 for(delay = 0;delay < 960;delay++)
	 {
	 }//��ʱ200us
    // ��������ROM��IDƥ������� 
    Write(0xcc);     
    // ����д�Ĵ������� 
    Write(0x4E);                         
    Write(0x60);   // TH ���125���϶�
    Write(0xCE);   // TL X-2^8=-50   X=CE
    Write(0x1F);   // ���üĴ���, R1 R0 �ֱ������� 9bit

	//******�����¶�ת��******
	// ��ʼ��
    Init_DS18B20();
	// ��������ROM��IDƥ������� 
    Write(0xcc); 
    // ���������¶�ת������
    Write(0x44);
    

   //******�ȴ��¶�ת�����******                                
//    delay = 500000;  // IO�߱�������500ms(12bitʱ)�ߵ�ƽ��������¶�ת��
//   	Delay_diff_1us(&delay);
   do
  {
    translate_OK=Read();
   }while(translate_OK != 0xFF);// ����ת���Ƿ������������ȡ���ߣ�ֱ������8��1���ֱ�־ת������

	//******��ȡ�¶�******
    // �¶�ת����ɣ��ٴγ�ʼ��
    Init_DS18B20();
                           
//    Write(0x55);     // ���ͷ����к�����
//    send_serial();   // �����к�

    // ��������ROM��IDƥ�������
	Write(0xcc);
	                            	
  	// ���Ͷ�ȡ�¶�����
    Write(0xbe);   
                           
    // ������ȡ�����ֽ����ݣ�temp_data0��λ
    temp_data0=Read();                    
    temp_data1=Read(); 
   
    //******�¶Ȼ���******   
	IODOR |= 0x80;
	IODATA |= 0x80;
    tem_short=temp_data1; 
    tem_short<<=8;                                              
    tem_short=(tem_short|temp_data0);     //���ֽںϳ�һ�����ͱ��� 
//    tt=tem_short*0.0625;                  //�õ���ʵʮ�����¶�ֵ����ȷ��0.0625��
//    tt=tt*10+0.5;                         //�Ŵ�ʮ������С������һλҲת��Ϊ����ʾ���֣��������� 
    tt=tem_short>>3;    //��3λ��������
    tt=tt*0.5;                  //�õ���ʵʮ�����¶�ֵ����ȷ��0.5��
	tt=tt*10;                         //�Ŵ�ʮ������С������һλҲת��Ϊ����ʾ���֣���������
    temperature = tt/10;                  //������ʵ�¶�ֵ 
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
	SD_write[15] = (RS485_DATA[5]) & 0xff;
	SD_write[16] = ((RS485_DATA[4])) & 0xff;
	SD_write[17] = ((RS485_DATA[3])) & 0xff;
	SD_write[18] = ((RS485_DATA[2])) & 0xff;//ѹ��ֵ��8bit
	SD_write[19] = ((Uint16)(temperature/0.5)) & 0xff;//�¶�����ֵ��8bit
	SD_write[20] = (((Uint16)(temperature/0.5))>>8) & 0xff;//�¶�����ֵ��8bit
	SD_write[21] = (noise_10ms) & 0x0ff;//��������ֵ��8bit
	SD_write[22] = (noise_10ms>>8) & 0x0ff;//��������ֵ��8bit
	SD_write[23] = 0x00;//FSKƵ��1---9kHz
	SD_write[24] = 0x08;//FSKƵ�2---9+8*0.5=13kHz
	SD_write[25] = 0x0ff;//У�λ
	SD_write[26] = SD_status;//SD��״̬��1Ϊ������0Ϊ������
 	SD_write[27] = 0x0ff;//У��λ
}






