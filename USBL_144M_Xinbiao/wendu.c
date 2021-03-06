
/******************* 初始化ds18b2子函数**************************/ 
void Init_DS18B20()                 //反回一个应答信号，是否完成初始化
  { 
	volatile short flag;
	volatile Uint32 delay;

	flag = 1;
  	while (flag)
	{
	 IODOR |= 0x80;	//设置为输出状态
	 IODATA |= 0xc0;   //输出高电平
	 for(delay = 0;delay < 24;delay++)
	 {
	 }//延时5us
     IODATA &= 0x7f;   //输出低电平
     
     for(delay = 0;delay < 2334;delay++)
	 {
	 }//延时500us                 

     IODATA |= 0xc0;	 //输出高电平
                        
	 for(delay = 0;delay < 264;delay++)
	 {
	 }//延时55us

	 IODOR &= 0x7f;	   //设置为输入状态
                    
	 for(delay = 0;delay < 288;delay++)
	 {
	 }//延时60us

     if( (IODATA & 0x80) == 0)
		 flag = 0;		   //应答信号，是否完成初始化
                         
	 for(delay = 0;delay <1920;delay++)
	 {
	 }//延时400us	 

	 }
 }

/*******  读字节子函数*******/     
short Read(void) 
 { 
	short dat;
      short i=0;
      volatile Uint32 delay; 
      dat = 0;
      for (i=8;i>0;i--) 
      { 
	      dat>>=1;   //读到数据后右移一位，只需移动7次
        IODATA &= 0x7f;	 //输出低电平
                                       
		 for(delay = 0;delay < 29;delay++)
		 {
		 }//延时6us

        IODATA |= 0x80;
		 for(delay = 0;delay <39;delay++)
		 {
		 }//延时8us
        IODOR &= 0x7f;	    //设置为输入状态
                          
		 for(delay = 0;delay < 24;delay++)
		 {
		 }//延时5us

        if((IODATA&0x80) == 0x80)     // 如果输入为高电平，读1
	    {
	    	dat|=0x80;			//取回最高位数据
		}
		 for(delay = 0;delay < 240;delay++)
		 {
		 }//延时50us

		IODOR |= 0x80;			//设置为输出状态    
		IODATA |= 0x80;    
  
		 for(delay = 0;delay < 48;delay++)
		 {
		 }//延时10us

     }
   return(dat); 
 } 

/*******  写字节子函数*******/ 
void Write(unsigned char cmd)
  { 
       volatile Uint32 delay;
       short i=0; 
	   IODOR |= 0x80;

	   IODATA &= 0x7f;

		 for(delay = 0;delay < 24;delay++)
		 {
		 }//延时5us

    for (i=8; i>0; i--) 
     {    
	   IODOR |= 0x80;

       IODATA &= 0x7f;

		 for(delay = 0;delay < 29;delay++)
		 {
		 }//延时6us

       if(cmd&0x01)
	      IODATA |= 0x80;                      //写入一位数据 
	   else
	      IODATA &= 0x7f;
		 for(delay = 0;delay < 240;delay++)
		 {
		 }//延时50us
       IODATA |= 0x80;

		 for(delay = 0;delay < 48;delay++)
		 {
		 }//延时10us
       cmd >>= 1;    	   //右移一位数据                               
   } 
} 

/*         读序列号函数             */
void read_serial(void)
{
	short i,j;
	volatile Uint32 delay;
	for(j=0;j<8;j++)
	   { 
	     serial[j]=0;
	     for(i=8;i>0;i--) 
	         { 
	          IODOR |= 0x80;	//设置为输出状态 
              IODATA |= 0x80;    //输出高电平                             
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//延时5us

	          serial[j]>>=1;                       //读到数据后右移一位
              IODATA &= 0x7f;	   //输出低电平
							  
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//延时5us	                                       

              IODOR &= 0x7f;	   //设置为输入状态
                        
			 for(delay = 0;delay < 24;delay++)
			 {
			 }//延时5us

			  if((IODATA&0x80) == 0x80)
			  {
				  serial[j]|=0x80;				  //取回最高位数据
			  }
			 for(delay = 0;delay < 240;delay++)
			 {
			 }//延时50us                              

	         }
	    }
 }

 /******************************************/
/*             发序列号函数               */
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
		 }//延时2us

         for(i=8; i>0; i--) 
            { 
			IODOR |= 0x80;
			IODATA &= 0x7f;

			 for(delay = 0;delay < 24;delay++)
			 {
			 }//延时5us
            //延时5微秒，发送开始
             if(temp&0x01)              //写入最低位数据
				IODATA |= 0x80;
	   		 else
				 IODATA &= 0x7f;

			 for(delay = 0;delay < 264;delay++)
			 {
			 }//延时55us

 			IODATA |= 0x80;

			 for(delay = 0;delay < 24;delay++)
			 {
			 }//延时5us

             temp>>=1;                  //右移一位数据
            } 
        } 
}  
 
/******************************************/
/*            读出温度函数                */ 
/******************************************/ 
/*获取温度信息
	函数功能：利用GPIO1控制DS18B20获取温度信息
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
    
    //******写寄存器******
	// 初始化
	Init_DS18B20();                       
	 for(delay = 0;delay < 960;delay++)
	 {
	 }//延时200us
    // 发送跳过ROM的ID匹配的命令 
    Write(0xcc);     
    // 发送写寄存器命令 
    Write(0x4E);                         
    Write(0x60);   // TH 最大125摄氏度
    Write(0xCE);   // TL X-2^8=-50   X=CE
    Write(0x1F);   // 配置寄存器, R1 R0 分辨率配置 9bit

	//******启动温度转换******
	// 初始化
    Init_DS18B20();
	// 发送跳过ROM的ID匹配的命令 
    Write(0xcc); 
    // 发送启动温度转换命令
    Write(0x44);
    

   //******等待温度转化完毕******                                
//    delay = 500000;  // IO线保持至少500ms(12bit时)高电平，以完成温度转换
//   	Delay_diff_1us(&delay);
   do
  {
    translate_OK=Read();
   }while(translate_OK != 0xFF);// 测试转换是否结束：反复读取总线，直到连续8个1出现标志转换结束

	//******读取温度******
    // 温度转换完成，再次初始化
    Init_DS18B20();
                           
//    Write(0x55);     // 发送发序列号命令
//    send_serial();   // 发序列号

    // 发送跳过ROM的ID匹配的命令
	Write(0xcc);
	                            	
  	// 发送读取温度命令
    Write(0xbe);   
                           
    // 连续读取两个字节数据，temp_data0低位
    temp_data0=Read();                    
    temp_data1=Read(); 
   
    //******温度换算******   
	IODOR |= 0x80;
	IODATA |= 0x80;
    tem_short=temp_data1; 
    tem_short<<=8;                                              
    tem_short=(tem_short|temp_data0);     //两字节合成一个整型变量 
//    tt=tem_short*0.0625;                  //得到真实十进制温度值，精确到0.0625度
//    tt=tt*10+0.5;                         //放大十倍，将小数点后第一位也转换为可显示数字，四舍五入 
    tt=tem_short>>3;    //低3位不起作用
    tt=tt*0.5;                  //得到真实十进制温度值，精确到0.5度
	tt=tt*10;                         //放大十倍，将小数点后第一位也转换为可显示数字，四舍五入
    temperature = tt/10;                  //返回真实温度值 
} 


void uart_config()
{
	memset(SD_write,0,sizeof(short)*256);
	SD_write[0] = 0x0ee;   //发送字节头
	SD_write[1] = transponder_mode;//工作模式
	SD_write[2] = tongbu_flag;//命令位
	SD_write[3] = 0x0bb;//校验位
	SD_write[4] = (NOISE_TH_NEW) & 0x0ff;//检测门限低8bit
	SD_write[5] = (NOISE_TH_NEW>>8) & 0x0ff;//检测门限高8bit
	SD_write[6] = responder_period/10;//校验位
	SD_write[7] = 0x0cc;//校验位
	SD_write[8] = ((Time_Delay/1000)) & 0x0ff;//转发时延低8bit
	SD_write[9] = ((Time_Delay/1000)>>8) & 0x0ff;//转发时延高8bit
	SD_write[10] = 0x0cc;//校验位
	SD_write[11] = 0x01;//校验位
	SD_write[12] = (voltage_AD) & 0x0ff;//电压量化值低8bit
	SD_write[13] = (voltage_AD>>8) & 0x0ff;//电压量化值高8bit
	SD_write[14] = 0x01;//校验位
	SD_write[15] = (RS485_DATA[5]) & 0xff;
	SD_write[16] = ((RS485_DATA[4])) & 0xff;
	SD_write[17] = ((RS485_DATA[3])) & 0xff;
	SD_write[18] = ((RS485_DATA[2])) & 0xff;//压力值高8bit
	SD_write[19] = ((Uint16)(temperature/0.5)) & 0xff;//温度量化值低8bit
	SD_write[20] = (((Uint16)(temperature/0.5))>>8) & 0xff;//温度量化值高8bit
	SD_write[21] = (noise_10ms) & 0x0ff;//噪声量化值低8bit
	SD_write[22] = (noise_10ms>>8) & 0x0ff;//噪声量化值高8bit
	SD_write[23] = 0x00;//FSK频率1---9kHz
	SD_write[24] = 0x08;//FSK频�2---9+8*0.5=13kHz
	SD_write[25] = 0x0ff;//校槲�
	SD_write[26] = SD_status;//SD卡状态，1为正常，0为不正常
 	SD_write[27] = 0x0ff;//校验位
}






