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

void LCD12864_write_byte(uchar byte);		//д�ֽں���
uchar LCD12864_read_byte();		//���ֽں���
void LCD12864_busy();	//���æ����
void LCD12864_write(uchar dat_com,uchar byt); //д�뺯��(��дcom_dat����д����ָ�������)

/*���¸�����Ϊ��������������ú���*/
void LCD12864_init();		//��ʼ������
void LCD12864_write_bmp(uchar *puts);		//��ͼ����(����ʱ��������дͼ����������)
void LCD12864_lcd_clear();	//��������
void LCD12864_bmp_clear();	//��ͼ�κ���
void LCD12864_txt_clear();		//���ı�����
void LCD12864_write_zi(uchar x,uchar y,uchar *buff);		//�������������������������ַ�����������
void LCD12864_write_txt(uchar row,uchar position,uchar *byte);//д�뺯��(��дrow+_���Լ�position_�У���д��������)positionΪ1-16,���ַ���ʾ
void LCD12864_delay(uint x);	//��ʱ����


#endif /* LCD12864_H_ */
