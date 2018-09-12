//#include <string.h>
#include "sys_interrupts.h"

interrupt void c_int4()//��ͬ��  �ⲿGPIOͬ���ź�����
{
	Syn_flag = 1;
}

interrupt void c_int5() //״̬�ı�   
{
    Configure_flag = 1;
}

/*interrupt void c_int6()//LBLѯ���źŷ���
{
	if(send_sig_buffer[send_num++] > 0) //���Խ�����ת����16λ������ �������ݵ��ڴ�ռ���� �˹���δʵ��
	{
		GPIO_BANK45->OUT_DATA = 0x12000000;	//GPIO5[12][9][6]
	}
	else
	{
		GPIO_BANK45->OUT_DATA = 0x10400000;
	}
}
*/

interrupt void c_int6()//LBLѯ���źŷ��� ���ڲ�����
{
  Trans_flag=1;
}

interrupt void c_int7()       //EDMA�ж�
{
	unsigned int regIPR, IxBitMask, IxCounter;
	IxBitMask = 0x00000010;
	while(Edma30cc_0_RegsOvly->IPR != 0){
		// Read Interrupt Pending Register
		regIPR = Edma30cc_0_RegsOvly->IPR;
		IxCounter = regIPR & IxBitMask;
		if(IxCounter == 0x10){
			Rx_flag = 1;
			Pp_flag = !Pp_flag;
		}
		Edma30cc_0_RegsOvly->ICR = regIPR;
	}
}

interrupt void c_int8()       //CPLD ��ʱ���ж�
{
	Time_over = 1;
}

interrupt void c_int9()    //����USBL�����
{

}

interrupt void c_int10()//�ӳ�BTʱ���ѵ�
{

}

interrupt void c_int11()
{

}
