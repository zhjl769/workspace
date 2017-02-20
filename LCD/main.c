 #include <msp430f1611.h>
#include <string.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

/**************�궨��***************/
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
						  		//��һ����ʾ����
uchar code dis1[]="0001.00V";
void LCD1602()
{
en=0;
rs=1;
rw=1;
DataPort=0xff; //����P0Ϊ��LCD D0~D7������I/O ��
}
void read_busy()
{
DataPort=0xff;
rs=0;
rw=1;
en=1;
while(DataPort&0x80); //P0��10000000���룬D7λ����Ϊ0��ͣ�ڴ˴�
en=0; //��Ϊ0����������һ���������������þ��Ǽ��D7λ
}
void write(uchar i,bit j)
{
read_busy();
DataPort=i; //����i=0,дָ�i=1��д���ݣ�
rs=j;
rw=0;
en=1;
en=0;
}

void display_lcd_byte(uchar y,uchar x,uchar z) //Y=0,1����ʼ�У�X=0~15����ʼ�У�Z=��д�ַ���ASCII ��
{
if(y) //�Ƿ���ʾ�ڵڶ��У����ڵ�һ��Y=0,������IF ��䣬���ڵڶ��У�����IF ���
{
x+=0x40; //�ڶ�����ʼ��ַ��������Ϊ�ַ���ʾ��ַ
}
x+=0x80; //��������ָ��λ��
write(x,0);
write(z,1); //д������
}
void display_lcd_zi(uchar y,uchar x,uchar a,uchar dis2[]) //Y=0,1����ʼ�У�X=0~15����ʼ�У�aΪ�Խ�ģ����λ��ֵ  Z=��д�ַ�������
{
	  uint i;
	  uchar t,z=0;
	write(0x40,0);
	for(i = 0; i < 64 ; i ++ )
	{
	write(dis2[i],1);
	}
if(y) //�Ƿ���ʾ�ڵڶ��У����ڵ�һ��Y=0,������IF ��䣬���ڵڶ��У�����IF ���
{
x+=0x40; //�ڶ�����ʼ��ַ��������Ϊ�ַ���ʾ��ַ
}
x+=0x80; //��������ָ��λ��

write(x,0);

t=a+x; // ����ַ������ȼ�����ʼ��λ��
while(x<t) //����ΪLCD ��ʾ���ַ������һ���ַ�����ֹ�ַ���û��16���ַ����Ӷ�����λ�������룻
{
write(z,1);; //��λ��ʾ�������ַ�
x++;
z++;
}
}
void delay(uint c) //����Ϊ�ṩ��ʼ���������ӳ����е���ʱ1xc MS
{
uint a,b;
for(a=0;a<c;a++)
for(b=0;b<110;b++);
 }
void display_lcd_text(uchar y,uchar x,uchar dis1[]) //Y��X ͬ���ַ���ʾ��table[] �ַ�������
{
uchar z=0;
uchar t;
t=strlen(dis1)+x; // ����ַ������ȼ�����ʼ��λ��
while(x<t) //����ΪLCD ��ʾ���ַ������һ���ַ�����ֹ�ַ���û��16���ַ����Ӷ�����λ�������룻
{
display_lcd_byte(y,x,dis1[z]); //��λ��ʾ�������ַ�
x++;
z++;
}
}
void init() //��ȫ����Ҫ���ʼ�����������м�ʡ����һ��дָ��38H
{
delay(15);
write(0x38,0);//8λ����,5*8����
delay(5);
write(0x38,0);
write(0x08,0);	//�ر���ʾ
write(0x01,0); //����
write(0x06,0);	//��ʾ��ַ����,��дһ�����ݺ�,��ʾλ������һλ
write(0x0f,0);	//������ʾ��,�����,�����˸
}

void main()
{
	LCD1602();
	init();
		display_lcd_zi(1,0,5,dis2); // ��ʾһ���ַ�
		display_lcd_text(1,5,dis1); // ��ʾ�ַ���
	while(1);
}
