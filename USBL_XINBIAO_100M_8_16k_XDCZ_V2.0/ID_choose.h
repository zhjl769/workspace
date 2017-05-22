/*
 * ID_choose.h
 *
 *  Created on: 2017-5-4
 *      Author: Administrator
 */

#ifndef ID_CHOOSE_H_
#define ID_CHOOSE_H_

//�����ű�ID
#define XBID 1
//--------------------------------ͨ�ò���-------------------------------------------------
//5֧�ű��ʱ�ӣ��˴���Ҫ��Ϊ����Բ��󣺲�ͬ���ű�����վ��������ű�ʱ����Ҫ��ȥ��ͬ��ʱ�ӣ�
#define SY_1 (150000)		//1#
#define SY_2 (250000)		//2#
#define SY_3 (350000)		//3#
#define SY_5 (550000)		//5#
#define SY_6 (650000)		//6#
//5֧�ű��RAMSES�źţ�1���˴���Ҫ��Ϊ����Բ��󣺲�ͬ���ű�����վ��������ű�ʱ����Ҫ��ⲻͬ��RAMSES�롣2��ͬʱҲ���������ű��IIF��
#define RAMSES01 (0)	//CIF
#define RAMSES07 (6)	//1#_IIF
#define RAMSES15 (14)	//2#_IIF
#define RAMSES11 (10)	//3#_IIF
#define RAMSES21 (20)	//5#_IIF
#define RAMSES09 (8)	//6#_IIF
//--------------------------------1���ű�-------------------------------------------------
#if (XBID == 1)
#define FSK_f1 		(2)			//FSKƵ��1
#define FSK_f2 		(8)			//FSKƵ��2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES07	//IIF
#define ZFSY 		SY_1		//ת��ʱ��
#define JCIIF1		RAMSES15	//��Բ����������ű��IIF����������һ��
#define JCIIF2		RAMSES11	//��Բ����������ű��IIF���������ڶ���
#define JCIIF3		RAMSES21	//��Բ����������ű��IIF��������������
#define JCIIF4		RAMSES09	//��Բ����������ű��IIF�����������ĸ�
#define JCSY1 		SY_2		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ���������һ��
#define JCSY2		SY_3		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ��������ڶ���
#define JCSY3		SY_5		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ�������������
#define JCSY4		SY_6		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ����������ĸ�
#define JCXBID1		(2)			//��Բ����������ű��ID����������һ��
#define JCXBID2		(3)			//��Բ����������ű��ID���������ڶ���
#define JCXBID3		(5)			//��Բ����������ű��ID��������������
#define JCXBID4		(6)			//��Բ����������ű��ID�����������ĸ�
#endif
//--------------------------------2���ű�-------------------------------------------------
#if (XBID == 2)
#define FSK_f1 		(3)			//FSKƵ��1
#define FSK_f2 		(9)			//FSKƵ��2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES15	//IIF
#define ZFSY 		SY_2		//ת��ʱ��
#define JCIIF1		RAMSES07	//��Բ����������ű��IIF����������һ��
#define JCIIF2		RAMSES11	//��Բ����������ű��IIF���������ڶ���
#define JCIIF3		RAMSES21	//��Բ����������ű��IIF��������������
#define JCIIF4		RAMSES09	//��Բ����������ű��IIF�����������ĸ�
#define JCSY1 		SY_1		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ���������һ��
#define JCSY2		SY_3		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ��������ڶ���
#define JCSY3		SY_5		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ�������������
#define JCSY4		SY_6		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ����������ĸ�
#define JCXBID1		(1)			//��Բ����������ű��ID����������һ��
#define JCXBID2		(3)			//��Բ����������ű��ID���������ڶ���
#define JCXBID3		(5)			//��Բ����������ű��ID��������������
#define JCXBID4		(6)			//��Բ����������ű��ID�����������ĸ�
#endif
//--------------------------------3���ű�-------------------------------------------------
#if (XBID == 3)
#define FSK_f1 		(4)			//FSKƵ��1
#define FSK_f2 		(10)			//FSKƵ��2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES11	//IIF
#define ZFSY 		SY_3		//ת��ʱ��
#define JCIIF1		RAMSES07	//��Բ����������ű��IIF����������һ��
#define JCIIF2		RAMSES15	//��Բ����������ű��IIF���������ڶ���
#define JCIIF3		RAMSES21	//��Բ����������ű��IIF��������������
#define JCIIF4		RAMSES09	//��Բ����������ű��IIF�����������ĸ�
#define JCSY1 		SY_1		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ���������һ��
#define JCSY2		SY_2		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ��������ڶ���
#define JCSY3		SY_5		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ�������������
#define JCSY4		SY_6		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ����������ĸ�
#define JCXBID1		(1)			//��Բ����������ű��ID����������һ��
#define JCXBID2		(2)			//��Բ����������ű��ID���������ڶ���
#define JCXBID3		(5)			//��Բ����������ű��ID��������������
#define JCXBID4		(6)			//��Բ����������ű��ID�����������ĸ�
#endif
//--------------------------------5���ű�-------------------------------------------------
#if (XBID == 5)
#define FSK_f1 		(6)			//FSKƵ��1
#define FSK_f2 		(12)		//FSKƵ��2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES21	//IIF
#define ZFSY 		SY_5		//ת��ʱ��
#define JCIIF1		RAMSES07	//��Բ����������ű��IIF����������һ��
#define JCIIF2		RAMSES15	//��Բ����������ű��IIF���������ڶ���
#define JCIIF3		RAMSES11	//��Բ����������ű��IIF��������������
#define JCIIF4		RAMSES09	//��Բ����������ű��IIF�����������ĸ�
#define JCSY1 		SY_1		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ���������һ��
#define JCSY2		SY_2		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ��������ڶ���
#define JCSY3		SY_3		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ�������������
#define JCSY4		SY_6		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ����������ĸ�
#define JCXBID1		(1)			//��Բ����������ű��ID����������һ��
#define JCXBID2		(2)			//��Բ����������ű��ID���������ڶ���
#define JCXBID3		(3)			//��Բ����������ű��ID��������������
#define JCXBID4		(6)			//��Բ����������ű��ID�����������ĸ�
#endif
//--------------------------------6���ű�-------------------------------------------------
#if (XBID == 6)
#define FSK_f1 		(7)			//FSKƵ��1
#define FSK_f2 		(13)		//FSKƵ��2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES09	//IIF
#define ZFSY 		SY_6		//ת��ʱ��
#define JCIIF1		RAMSES07	//��Բ����������ű��IIF����������һ��
#define JCIIF2		RAMSES15	//��Բ����������ű��IIF���������ڶ���
#define JCIIF3		RAMSES11	//��Բ����������ű��IIF��������������
#define JCIIF4		RAMSES21	//��Բ����������ű��IIF�����������ĸ�
#define JCSY1 		SY_1		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ���������һ��
#define JCSY2		SY_2		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ��������ڶ���
#define JCSY3		SY_3		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ�������������
#define JCSY4		SY_5		//��Բ����������ű���Ҫ��ȥ��ʱ�ӡ����������ĸ�
#define JCXBID1		(1)			//��Բ����������ű��ID����������һ��
#define JCXBID2		(2)			//��Բ����������ű��ID���������ڶ���
#define JCXBID3		(3)			//��Բ����������ű��ID��������������
#define JCXBID4		(5)			//��Բ����������ű��ID�����������ĸ�
#endif


#endif /* ID_CHOOSE_H_ */
