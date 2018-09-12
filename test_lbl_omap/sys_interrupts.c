//#include <string.h>
#include "sys_interrupts.h"

interrupt void c_int4()//外同步  外部GPIO同步信号启动
{
	Syn_flag = 1;
}

interrupt void c_int5() //状态改变   
{
    Configure_flag = 1;
}

/*interrupt void c_int6()//LBL询问信号发射
{
	if(send_sig_buffer[send_num++] > 0) //可以将数据转换成16位的数据 降低数据的内存占有率 此功能未实现
	{
		GPIO_BANK45->OUT_DATA = 0x12000000;	//GPIO5[12][9][6]
	}
	else
	{
		GPIO_BANK45->OUT_DATA = 0x10400000;
	}
}
*/

interrupt void c_int6()//LBL询问信号发射 基于采样率
{
  Trans_flag=1;
}

interrupt void c_int7()       //EDMA中断
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

interrupt void c_int8()       //CPLD 定时器中断
{
	Time_over = 1;
}

interrupt void c_int9()    //超出USBL检测域
{

}

interrupt void c_int10()//延迟BT时间已到
{

}

interrupt void c_int11()
{

}
