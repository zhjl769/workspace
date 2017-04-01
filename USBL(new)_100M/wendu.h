/*
 * wendu.h
 *
 *  Created on: 2016-12-21
 *      Author: zhjl769
 */

#ifndef WENDU_H_
#define WENDU_H_


/******************* 初始化ds18b2子函数**************************/
void Init_DS18B20()                 //反回一个应答信号，是否完成初始化
  {
	volatile short flag;
	volatile Uint32 delay;

	flag = 1;
  	while (flag)
	{
	 IODOR |= 0x80;	//设置为输出状态
	 IODATA |= 0x80;   //输出高电平
	 for(delay = 0;delay < 12;delay++)
	 {
	 }//延时5us
     IODATA &= 0x3f;   //输出低电平

     for(delay = 0;delay < 880;delay++)
	 {
	 }//延时500us

     IODATA |= 0x80;	 //输出高电平

	 for(delay = 0;delay < 100;delay++)
	 {
	 }//延时55us

	 IODOR &= 0x7f;	   //设置为输入状态

	 for(delay = 0;delay < 108;delay++)
	 {
	 }//延时60us

     if( (IODATA & 0x80) == 0)
		 flag = 0;		   //应答信号，是否完成初始化

	 for(delay = 0;delay <770;delay++)
	 {
	 }//延时400us
    break;
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
        IODATA &= 0x3f;	 //输出低电平

		 for(delay = 0;delay < 14;delay++)
		 {
		 }//延时6us

        IODATA |= 0x80;
		 for(delay = 0;delay <17;delay++)
		 {
		 }//延时8us
        IODOR &= 0x7f;	    //设置为输入状态

		 for(delay = 0;delay < 12;delay++)
		 {
		 }//延时5us

        if((IODATA&0x80) == 0x80)     // 如果输入为高电平，读1
	    {
	    	dat|=0x80;			//取回最高位数据
		}
		 for(delay = 0;delay < 91;delay++)
		 {
		 }//延时50us

		IODOR |= 0x80;			//设置为输出状态
		IODATA |= 0x80;

		 for(delay = 0;delay < 21;delay++)
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

	   IODATA &= 0x3f;

		 for(delay = 0;delay < 13;delay++)
		 {
		 }//延时5us

    for (i=8; i>0; i--)
     {
	   IODOR |= 0x80;

       IODATA &= 0x3f;

		 for(delay = 0;delay < 13;delay++)
		 {
		 }//延时6us

       if(cmd&0x01)
	      IODATA |= 0x80;                      //写入一位数据
	   else
	      IODATA &= 0x3f;
		 for(delay = 0;delay < 90;delay++)
		 {
		 }//延时50us
       IODATA |= 0x80;

		 for(delay = 0;delay < 21;delay++)
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
			 for(delay = 0;delay < 13;delay++)
			 {
			 }//延时5us

	          serial[j]>>=1;                       //读到数据后右移一位
              IODATA &= 0x3f;	   //输出低电平

			 for(delay = 0;delay < 13;delay++)
			 {
			 }//延时5us

              IODOR &= 0x7f;	   //设置为输入状态

			 for(delay = 0;delay < 13;delay++)
			 {
			 }//延时5us

			  if((IODATA&0x80) == 0x80)
			  {
				  serial[j]|=0x80;				  //取回最高位数据
			  }
			 for(delay = 0;delay < 91;delay++)
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
		IODATA &= 0x3f;

		 for(delay = 0;delay < 6;delay++)
		 {
		 }//延时2us

         for(i=8; i>0; i--)
            {
			IODOR |= 0x80;
			IODATA &= 0x3f;

			 for(delay = 0;delay < 13;delay++)
			 {
			 }//延时5us
            //延时5微秒，发送开始
             if(temp&0x01)              //写入最低位数据
				IODATA |= 0x80;
	   		 else
				 IODATA &= 0x3f;

			 for(delay = 0;delay < 100;delay++)
			 {
			 }//延时55us

 			IODATA |= 0x80;

			 for(delay = 0;delay < 13;delay++)
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
	函数功能：利用GPIO7控制DS18B20获取温度信息
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
	 for(delay = 0;delay < 380;delay++)
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

#endif /* WENDU_H_ */
