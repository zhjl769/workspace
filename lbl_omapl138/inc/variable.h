/*
 * variable.h
 *
 *  Created on: 2014-8-5
 *      Author: Administrator
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#define IN_LENGTH    1024

//ȫ�ֱ�������
extern volatile int rx_flag;      //EDMA������ɱ�־
extern volatile int test_flag ;
extern volatile int pp_flag ;      //ƹ�һ����־����ʼ��Ϊping
extern volatile int block_num ;

extern short buffer1[IN_LENGTH];
extern short buffer2[IN_LENGTH];


#endif /* VARIABLE_H_ */
