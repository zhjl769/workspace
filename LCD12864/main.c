#include <msp430f1611.h>

#include "Init.h"
#include "Other.h"
#include "LCD12864.h"
#include "BMP180.h"

#define CPU_F ((double)8000000)
#define delay_us(x) _delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))//�����ʱ��������ֱ�ӵ���delay_ms (x),xΪ��Ҫ��ʱ��ʱ��

unsigned char tab1[]="�¶ȸ߶Ȳ���";
unsigned char tab2[]="��ǰ�¶ȣ�";
unsigned char tab3[]="��ǰ�߶ȣ�";
//�¶ȼ�ͼ��
unsigned char tab5[] = {0x00,0x00,0x01,0x80,0x02,0x40,0x02,0xC0,0x02,0x40,0x02,0xC0,0x02,0x40,0x02,0xC0,
 0x02,0x40,0x03,0xC0,0x07,0xE0,0x0F,0xF0,0x0F,0xF0,0x07,0xE0,0x03,0xC0,0x00,0x00};
/*
 * main.c
 */
void main( )
{
       WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
       IO_Init();//IO�ڳ�ʼ��
       Init_CLK();//ʱ�ӳ�ʼ��
    //   Init_TimerA();//��ʱ��A��ʼ��
       Init_BMP180();//BMP180��ʼ��
       LCD12864_init();
       P6DIR |= BIT4;
       P6OUT |= BIT4;
       delay_ms(200);
       P6OUT &= ~BIT4;
       delay_ms(200);
        _EINT(); //����C�������ڲ�����ʹ���ж�

    	LCD12864_write_zi(1,2,tab1);
    	LCD12864_write_zi(2,1,tab2);
    	LCD12864_write_zi(3,1,tab3);

    	while (1)
    	{
    		BMP180_Convert();
    		//delay_ms(2000);
    	//	BMP180_SendByte(0xaa);
    		delay_ms(200);

      	}
}
