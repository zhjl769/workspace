/*
 * Interrupt.h
 *
 *  Created on: 2015-12-10
 *      Author: zhjl769
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

// 串口 1 的接收标志
unsigned char RecBuf[10];//接收数据缓存
unsigned char TecBuf[10];//接收数据缓存
unsigned char *RDat;//指向接收到的数据
unsigned char *SDat;//指向要发送的数据
unsigned char Rindex = 0;//接收数据计数
unsigned char Tindex = 0;//发送数据计数
unsigned char  SendLen = 5;//发送数据长度
//extern unsigned int Rec_flag = 0;//接收状态标志位
//串口数据包解包对应的标志位
unsigned char Flag_get = 0;// 获取下位机温度、电量、速度、雷达信号的标志位，为1表示获取
unsigned char Flag_speedset = 0;// 获取左右侧的速度的标志位，为1表示进行设置


// 温度相关变量
unsigned char dis1[] = "T:";
unsigned char dis2[] = "00.00";
unsigned char dis3[] = {0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};//摄氏温度字模

// 电压值相关变量
unsigned char dis_v1[] = "V:";
unsigned char dis_v2[] = "00.00V";


__interrupt void UART1_RX (void);//接收中断
 __interrupt void UART1_TX (void); //发送中断
void temperature_get(void);//温度获取函数
void Voltage_get(void);//电压获取函数




#endif /* INTERRUPT_H_ */
