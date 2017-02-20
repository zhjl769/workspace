/*
 * LCD12864.h
 *
 *  Created on: 2016-2-25
 *      Author: zhjl769
 */

#ifndef LCD12864_H_
#define LCD12864_H_

#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()
#define com 0
#define dat 1

void LCD12864_write_byte(uchar byte);		//写字节函数
uchar LCD12864_read_byte();		//读字节函数
void LCD12864_busy();	//检测忙函数
void LCD12864_write(uchar dat_com,uchar byt); //写入函数(先写com_dat，再写所需指令或数据)

/*以下各函数为主函数中所需调用函数*/
void LCD12864_init();		//初始化函数
void LCD12864_write_bmp(uchar *puts);		//绘图函数(调用时括号里面写图形数组名称)
void LCD12864_lcd_clear();	//清屏函数
void LCD12864_bmp_clear();	//清图形函数
void LCD12864_txt_clear();		//清文本函数
void LCD12864_write_zi(uchar x,uchar y,uchar *buff);		//括号中依次填行数，列数，字符串数组名称
void LCD12864_write_txt(uchar row,uchar position,uchar *byte);//写入函数(先写row+_行以及position_列，再写所需数据)position为1-16,半字符显示
void LCD12864_delay(uint x);	//延时函数


#endif /* LCD12864_H_ */
