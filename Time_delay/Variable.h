/*
 * Variable.h
 *
 *  Created on: 2015-12-7
 *      Author: zhjl769
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

 unsigned int refClock = 0;
 unsigned int Point = 0;  //检测的个数的值
 unsigned int Tip = 0;  //检测标志位
 unsigned long Time = 0;
 unsigned long Len = 100;
 unsigned long dis[200] = {0x00};
 unsigned char dis1[] = "+:";
 unsigned char dis2[] = "-:";
 unsigned int i = 0,j = 0;
 unsigned int one,two,three,four,five,six,seven;
 long temp,temp1,temp2,dif_positive,dif_negative;
 unsigned int Num;//定时器转换为ms的系数，也就是时钟频率/1MHz的系数



#endif /* VARIABLE_H_ */
