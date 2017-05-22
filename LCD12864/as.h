#ifndef  _LCD12864_H
#define	 _LCD12864_H

sbit cs = P1^0; 	//液晶片选
sbit sid = P1^1; 	//液晶数据口
sbit lcd_clk = P1^2; 	//液晶时钟

#define nop _nop_()
#define com 0
#define dat 1

void delay(uint z)	//1T51,24M晶振，毫秒
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=100;y>0;y--);
}

void write_byte(uchar byte)		//写字节函数
{
	uchar i;
	for(i=0;i<8;i++)
	{
		lcd_clk=0;
		byte<<=1;
		sid=CY;
		lcd_clk=1;
	}
}

uchar read_byte()  //读字节函数
{
	uchar i,temp1,temp2;
	temp1 = 0;
	temp2 = 0;
	for(i=0;i<8;i++)		//读高四位
	{
		temp1=temp1<<1;
		lcd_clk = 0;
		lcd_clk = 1;
		lcd_clk = 0;
		if(sid)
			temp1++;
	}
	for(i=0;i<8;i++)		//读低四位
	{
		temp2=temp2<<1;
		lcd_clk = 0;
		lcd_clk = 1;
		lcd_clk = 0;
		if(sid)
			temp2++;
	}
	return ((0xf0&temp1)+(0x0f&temp2));
}

void lcd_busy()		//检测忙函数
{
	do write_byte(0xfc);	//11111,A(1),B(0),0
	while(0x80&read_byte());
}

void write(bit dat_com,uchar byt) //写入函数(先写com活dat，再写所需指令或数据)
{
	uchar temp;
	if(dat_com==0) //为零,写入指令
		temp=0xf8; //11111,A(0),B(0),0
	else //否则,写入数据
		temp=0xfa;  //11111,A(0),B(1),0
	cs=1;
	lcd_busy();
	write_byte(temp);
	write_byte(byt&0xf0); //写入高四位
	write_byte(byt<<4); //写入低四位
	cs=0;
}

void write_zi(uchar x,uchar y,uchar *buff)		//括号中依次填行数，列数，字符串数组名称
{
	uchar addr,i=0;
	if(x==0)
		addr=0x80;
	else if(x==1)
		addr=0x90;
	else if(x==2)
		addr=0x88;
	else if(x==3)
		addr=0x98;
	addr=addr+y;
	write(com,0x30);
	write(com,addr);
	while(buff[i]!='\0')
	{
		write(dat,buff[i]);
		i++;
	}
}

void lcd_init()		//初始化函数
{
	delay(40);
	write(com,0x30);//8位数据格式，基本指令显示
	delay(5);    //延时时间
	write(com,0x30);//8位数据格式，基本指令显示
	delay(37);
	write(com,0x0C);//开显示、关闭光标
	delay(5);
	write(com,0x01);//清屏指令
	delay(5);   //延时
	write(com,0x06);//设置显示点：指针自加1
}

#endif
