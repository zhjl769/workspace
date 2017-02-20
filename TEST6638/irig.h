/*
 * irig.h
 *
 *  Created on: 2016年3月5日
 *      Author: zane
 */

#ifndef IRIG_H_
#define IRIG_H_

long FIR(int ADC_temp);		//FIR滤波函数
void IRIG();				//B码机解码
void ADC_ready();			//AD单次采样处理
void FIR_high();			//FIR滤波高幅判决
void FIR_low();				//FIR滤波低幅判决


#endif /* IRIG_H_ */
