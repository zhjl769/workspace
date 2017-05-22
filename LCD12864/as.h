#ifndef  _LCD12864_H
#define	 _LCD12864_H

sbit cs = P1^0; 	//Һ��Ƭѡ
sbit sid = P1^1; 	//Һ�����ݿ�
sbit lcd_clk = P1^2; 	//Һ��ʱ��

#define nop _nop_()
#define com 0
#define dat 1

void delay(uint z)	//1T51,24M���񣬺���
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=100;y>0;y--);
}

void write_byte(uchar byte)		//д�ֽں���
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

uchar read_byte()  //���ֽں���
{
	uchar i,temp1,temp2;
	temp1 = 0;
	temp2 = 0;
	for(i=0;i<8;i++)		//������λ
	{
		temp1=temp1<<1;
		lcd_clk = 0;
		lcd_clk = 1;
		lcd_clk = 0;
		if(sid)
			temp1++;
	}
	for(i=0;i<8;i++)		//������λ
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

void lcd_busy()		//���æ����
{
	do write_byte(0xfc);	//11111,A(1),B(0),0
	while(0x80&read_byte());
}

void write(bit dat_com,uchar byt) //д�뺯��(��дcom��dat����д����ָ�������)
{
	uchar temp;
	if(dat_com==0) //Ϊ��,д��ָ��
		temp=0xf8; //11111,A(0),B(0),0
	else //����,д������
		temp=0xfa;  //11111,A(0),B(1),0
	cs=1;
	lcd_busy();
	write_byte(temp);
	write_byte(byt&0xf0); //д�����λ
	write_byte(byt<<4); //д�����λ
	cs=0;
}

void write_zi(uchar x,uchar y,uchar *buff)		//�������������������������ַ�����������
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

void lcd_init()		//��ʼ������
{
	delay(40);
	write(com,0x30);//8λ���ݸ�ʽ������ָ����ʾ
	delay(5);    //��ʱʱ��
	write(com,0x30);//8λ���ݸ�ʽ������ָ����ʾ
	delay(37);
	write(com,0x0C);//����ʾ���رչ��
	delay(5);
	write(com,0x01);//����ָ��
	delay(5);   //��ʱ
	write(com,0x06);//������ʾ�㣺ָ���Լ�1
}

#endif
