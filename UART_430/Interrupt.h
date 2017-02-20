/*
 * Interrupt.h
 *
 *  Created on: 2015-12-10
 *      Author: zhjl769
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

// ���� 1 �Ľ��ձ�־
unsigned char RecBuf[10];//�������ݻ���
unsigned char TecBuf[10];//�������ݻ���
unsigned char *RDat;//ָ����յ�������
unsigned char *SDat;//ָ��Ҫ���͵�����
unsigned char Rindex = 0;//�������ݼ���
unsigned char Tindex = 0;//�������ݼ���
unsigned char  SendLen = 5;//�������ݳ���
//extern unsigned int Rec_flag = 0;//����״̬��־λ
//�������ݰ������Ӧ�ı�־λ
unsigned char Flag_get = 0;// ��ȡ��λ���¶ȡ��������ٶȡ��״��źŵı�־λ��Ϊ1��ʾ��ȡ
unsigned char Flag_speedset = 0;// ��ȡ���Ҳ���ٶȵı�־λ��Ϊ1��ʾ��������


// �¶���ر���
unsigned char dis1[] = "T:";
unsigned char dis2[] = "00.00";
unsigned char dis3[] = {0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};//�����¶���ģ

// ��ѹֵ��ر���
unsigned char dis_v1[] = "V:";
unsigned char dis_v2[] = "00.00V";


__interrupt void UART1_RX (void);//�����ж�
 __interrupt void UART1_TX (void); //�����ж�
void temperature_get(void);//�¶Ȼ�ȡ����
void Voltage_get(void);//��ѹ��ȡ����




#endif /* INTERRUPT_H_ */
