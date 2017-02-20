 #include <msp430f1611.h>
#include <string.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

/**************宏定义***************/
#define DataDir     P4DIR
#define DataPort    P4OUT
#define rs P3OUT&=~BIT2
#define rs P3OUT|=BIT2
#define rw P3OUT&=~BIT1	//RW = P3.1
#define rw P3OUT|=BIT1
#define en P3OUT&=~BIT0	//EN = P3.2
#define en P3OUT|=BIT0

uchar code dis2[] = {     0x04,0x1F,0x15,0x1F,0x15,0x1F,0x04,0x07,
						  0x1F,0x14,0x14,0x1F,0x14,0x15,0x1F,0x10,
						  0x01,0x02,0x06,0x02,0x02,0x02,0x02,0x02,
						  0x04,0x0E,0x04,0x0E,0x0A,0x0E,0x0A,0x1F,
						  0x00,0x06,0x06,0x00,0x00,0x06,0x06,0x00,};
						  		//第一行显示内容
uchar code dis1[]="0001.00V";
void LCD1602()
{
en=0;
rs=1;
rw=1;
DataPort=0xff; //这里P0为与LCD D0~D7相连的I/O 口
}
void read_busy()
{
DataPort=0xff;
rs=0;
rw=1;
en=1;
while(DataPort&0x80); //P0和10000000相与，D7位若不为0，停在此处
en=0; //若为0跳出进入下一步；这条语句的作用就是检测D7位
}
void write(uchar i,bit j)
{
read_busy();
DataPort=i; //其中i=0,写指令；i=1，写数据；
rs=j;
rw=0;
en=1;
en=0;
}

void display_lcd_byte(uchar y,uchar x,uchar z) //Y=0,1（起始行）X=0~15（起始列）Z=想写字符的ASCII 码
{
if(y) //是否显示在第二行（若在第一行Y=0,不进入IF 语句，若在第二行，进入IF 语句
{
x+=0x40; //第二行起始地址加上列数为字符显示地址
}
x+=0x80; //设置数据指针位置
write(x,0);
write(z,1); //写入数据
}
void display_lcd_zi(uchar y,uchar x,uchar a,uchar dis2[]) //Y=0,1（起始行）X=0~15（起始列）a为自建模数的位数值  Z=想写字符的数组
{
	  uint i;
	  uchar t,z=0;
	write(0x40,0);
	for(i = 0; i < 64 ; i ++ )
	{
	write(dis2[i],1);
	}
if(y) //是否显示在第二行（若在第一行Y=0,不进入IF 语句，若在第二行，进入IF 语句
{
x+=0x40; //第二行起始地址加上列数为字符显示地址
}
x+=0x80; //设置数据指针位置

write(x,0);

t=a+x; // 求得字符串长度加上起始列位置
while(x<t) //功能为LCD 显示到字符串最后一个字符，防止字符串没有16个字符，从而不够位产生乱码；
{
write(z,1);; //逐位显示数组内字符
x++;
z++;
}
}
void delay(uint c) //功能为提供初始化等其他子程序中的延时1xc MS
{
uint a,b;
for(a=0;a<c;a++)
for(b=0;b<110;b++);
 }
void display_lcd_text(uchar y,uchar x,uchar dis1[]) //Y，X 同上字符显示，table[] 字符串数组
{
uchar z=0;
uchar t;
t=strlen(dis1)+x; // 求得字符串长度加上起始列位置
while(x<t) //功能为LCD 显示到字符串最后一个字符，防止字符串没有16个字符，从而不够位产生乱码；
{
display_lcd_byte(y,x,dis1[z]); //逐位显示数组内字符
x++;
z++;
}
}
void init() //完全按照要求初始化流程来，中间省略了一步写指令38H
{
delay(15);
write(0x38,0);//8位数据,5*8字形
delay(5);
write(0x38,0);
write(0x08,0);	//关闭显示
write(0x01,0); //清屏
write(0x06,0);	//显示地址递增,即写一个数据后,显示位置右移一位
write(0x0f,0);	//开启显示屏,开光标,光标闪烁
}

void main()
{
	LCD1602();
	init();
		display_lcd_zi(1,0,5,dis2); // 显示一个字符
		display_lcd_text(1,5,dis1); // 显示字符串
	while(1);
}
