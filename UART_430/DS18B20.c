#include <msp430f1611.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "DS18B20.h"


#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//软件延时，程序中直接调用delay_ms (x),x为需要延时的时间

#define DATA_OUT P6DIR |= BIT3// DS18B20数据脚为输出,P6.3
#define DATA_IN  P6DIR &= ~BIT3// DS18B20数据脚为输入,P6.3
#define DATA_H   P6OUT |= BIT3//DS18B20数据脚输出高电平,P6.3
#define DATA_L   P6OUT &= ~BIT3//DS18B20数据脚输出低电平,P6.3
#define DATA_IO  (P6IN & BIT3)//DS18B20数据脚,P6.3
#define DATA_ST  BIT3//DS18B20数据脚,P6.3


/******************* 初始化ds18b2子函数**************************/
void Init_DS18B20()                 //反回一个应答信号，是否完成初始化
  { 
	volatile short flag;

	flag = 1;
  	while (flag)
	{
  	 DATA_OUT;	//设置为输出状态
  	 DATA_H;   //输出高电平
  	 delay_us(5);
	 DATA_L;   //输出低电平
	 delay_us(500);
     DATA_H;
     delay_us(30);
	 DATA_IN;	   //设置为输入状态
	 delay_us(20);
     if(DATA_IO == 0x00)
		 flag = 0;		   //应答信号，是否完成初始化
         delay_us(480);
         DATA_OUT;	//设置为输出状态
         DATA_H;   //输出高电平
	 }
   }

/*******  读字节子函数*******/
short Read(void) 
 { 
	  short dat;
      short i=0;
      dat = 0;
      DATA_OUT;//设置为输出状态
      DATA_H;
      delay_us(2);
      for (i=8;i>0;i--)
      { 
    	dat >>= 1;   //读到数据后右移一位，只需移动7次
    	DATA_L;	 //输出低电平
        delay_us(6);
        DATA_H;
        delay_us(8);//延时8us
        DATA_IN;//设置为输入状态
        delay_us(5);//延时5us
        if(DATA_IO == DATA_ST)     // 如果输入为高电平，读1
	    {
	    	dat |= 0x80;	//取回最高位数据
		}
        delay_us(50);//延时50us
		DATA_OUT;//设置为输出状态
		DATA_H;
		delay_us(10);//延时10us
     }
   return(dat); 
 } 

/*******  写字节子函数*******/
void Write(unsigned char cmd)
  { 
       short i=0; 
	   DATA_OUT;
	   DATA_L;
	   delay_us(15);//延时15us

    for (i=8; i>0; i--) 
     {    
	   DATA_OUT;
       DATA_L;
       delay_us(6);//延时6us
       if(cmd & 0x01)
	      DATA_H;//写入一位数据
	   else
	      DATA_L;
          delay_us(50);//延时50us
          DATA_H;
          delay_us(10);//延时10us
          cmd >>= 1;//右移一位数据
      }
} 

/*         读序列号函数             */
void read_serial(void)
{
	short i,j;
	for(j=0;j<8;j++)
	   { 
	     serial[j]=0;
	     for(i=8;i>0;i--) 
	         { 
	          DATA_OUT;	//设置为输出状态
              DATA_H;    //输出高电平
              delay_us(5);//延时5us
	          serial[j]>>=1;//读到数据后右移一位
              DATA_L;	   //输出低电平
              delay_us(5);//延时5us
              DATA_IN;	   //设置为输入状态
                        
              delay_us(5);//延时5us
			  if(DATA_IO == DATA_ST)
			  {
				serial[j]|=0x08;//取回最高位数据
			  }
			  delay_us(50);//延时50us
	         }
	    }
 }

 /******************************************/
/*             发序列号函数               */
/******************************************/

void send_serial()
{
	short i,j,temp;
     for (j=0;j<8;j++)
        { 
		temp = serial_x[j];
		DATA_OUT;
		DATA_L;

		delay_us(2);//延时2us

         for(i=8; i>0; i--) 
            { 
			DATA_OUT;
			DATA_L;

			delay_us(5);//延时5us
            //延时5微秒，发送开始
             if(temp & 0x01) //写入最低位数据
				DATA_H;
	   		 else
				 DATA_L;
             delay_us(55);//延时55us
 			 DATA_H;
 			delay_us(5);//延时5us
             temp>>=1;//右移一位数据
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
	temp_data0 = 0;
	temp_data1 = 0;
	tt = 0.0;
	DATA_OUT;
	DATA_H;
    
    //******写寄存器******
	// 初始化
	Init_DS18B20();                       
	delay_us(200);//延时200us
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
    translate_OK = Read();
   }while(translate_OK != 0xff);// 测试转换是否结束：反复读取总线，直到连续8个1出现标志转换结束

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
	DATA_OUT;
	DATA_H;
    tem_short=temp_data1; 
    tem_short<<=8;                                              
    tem_short=(tem_short|temp_data0);     //两字节合成一个整型变量
    tt=tem_short*0.0625;                  //得到真实十进制温度值，精确到0.0625度
    tt=tt*10+0.5;                         //放大十倍，将小数点后第一位也转换为可显示数字，四舍五入
//    tt=tem_short>>3;    //低3位不起作用
//    tt=tt*0.5;                  //得到真实十进制温度值，精确到0.5度
//	tt=tt*10;                         //放大十倍，将小数点后第一位也转换为可显示数字，四舍五入
	TPT = tt/10;                  //返回真实温度值
} 







