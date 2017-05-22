/*
 * ID_choose.h
 *
 *  Created on: 2017-5-4
 *      Author: Administrator
 */

#ifndef ID_CHOOSE_H_
#define ID_CHOOSE_H_

//定义信标ID
#define XBID 1
//--------------------------------通用参数-------------------------------------------------
//5支信标的时延（此处主要是为了相对测阵：不同的信标作主站检测其他信标时，需要减去不同的时延）
#define SY_1 (150000)		//1#
#define SY_2 (250000)		//2#
#define SY_3 (350000)		//3#
#define SY_5 (550000)		//5#
#define SY_6 (650000)		//6#
//5支信标的RAMSES信号（1、此处主要是为了相对测阵：不同的信标作主站检测其他信标时，需要检测不同的RAMSES码。2、同时也方便配置信标的IIF）
#define RAMSES01 (0)	//CIF
#define RAMSES07 (6)	//1#_IIF
#define RAMSES15 (14)	//2#_IIF
#define RAMSES11 (10)	//3#_IIF
#define RAMSES21 (20)	//5#_IIF
#define RAMSES09 (8)	//6#_IIF
//--------------------------------1号信标-------------------------------------------------
#if (XBID == 1)
#define FSK_f1 		(2)			//FSK频率1
#define FSK_f2 		(8)			//FSK频率2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES07	//IIF
#define ZFSY 		SY_1		//转发时延
#define JCIIF1		RAMSES15	//相对测阵检测其他信标的IIF————第一个
#define JCIIF2		RAMSES11	//相对测阵检测其他信标的IIF————第二个
#define JCIIF3		RAMSES21	//相对测阵检测其他信标的IIF————第三个
#define JCIIF4		RAMSES09	//相对测阵检测其他信标的IIF————第四个
#define JCSY1 		SY_2		//相对测阵检测其他信标需要减去的时延————第一个
#define JCSY2		SY_3		//相对测阵检测其他信标需要减去的时延————第二个
#define JCSY3		SY_5		//相对测阵检测其他信标需要减去的时延————第三个
#define JCSY4		SY_6		//相对测阵检测其他信标需要减去的时延————第四个
#define JCXBID1		(2)			//相对测阵检测其他信标的ID————第一个
#define JCXBID2		(3)			//相对测阵检测其他信标的ID————第二个
#define JCXBID3		(5)			//相对测阵检测其他信标的ID————第三个
#define JCXBID4		(6)			//相对测阵检测其他信标的ID————第四个
#endif
//--------------------------------2号信标-------------------------------------------------
#if (XBID == 2)
#define FSK_f1 		(3)			//FSK频率1
#define FSK_f2 		(9)			//FSK频率2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES15	//IIF
#define ZFSY 		SY_2		//转发时延
#define JCIIF1		RAMSES07	//相对测阵检测其他信标的IIF————第一个
#define JCIIF2		RAMSES11	//相对测阵检测其他信标的IIF————第二个
#define JCIIF3		RAMSES21	//相对测阵检测其他信标的IIF————第三个
#define JCIIF4		RAMSES09	//相对测阵检测其他信标的IIF————第四个
#define JCSY1 		SY_1		//相对测阵检测其他信标需要减去的时延————第一个
#define JCSY2		SY_3		//相对测阵检测其他信标需要减去的时延————第二个
#define JCSY3		SY_5		//相对测阵检测其他信标需要减去的时延————第三个
#define JCSY4		SY_6		//相对测阵检测其他信标需要减去的时延————第四个
#define JCXBID1		(1)			//相对测阵检测其他信标的ID————第一个
#define JCXBID2		(3)			//相对测阵检测其他信标的ID————第二个
#define JCXBID3		(5)			//相对测阵检测其他信标的ID————第三个
#define JCXBID4		(6)			//相对测阵检测其他信标的ID————第四个
#endif
//--------------------------------3号信标-------------------------------------------------
#if (XBID == 3)
#define FSK_f1 		(4)			//FSK频率1
#define FSK_f2 		(10)			//FSK频率2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES11	//IIF
#define ZFSY 		SY_3		//转发时延
#define JCIIF1		RAMSES07	//相对测阵检测其他信标的IIF————第一个
#define JCIIF2		RAMSES15	//相对测阵检测其他信标的IIF————第二个
#define JCIIF3		RAMSES21	//相对测阵检测其他信标的IIF————第三个
#define JCIIF4		RAMSES09	//相对测阵检测其他信标的IIF————第四个
#define JCSY1 		SY_1		//相对测阵检测其他信标需要减去的时延————第一个
#define JCSY2		SY_2		//相对测阵检测其他信标需要减去的时延————第二个
#define JCSY3		SY_5		//相对测阵检测其他信标需要减去的时延————第三个
#define JCSY4		SY_6		//相对测阵检测其他信标需要减去的时延————第四个
#define JCXBID1		(1)			//相对测阵检测其他信标的ID————第一个
#define JCXBID2		(2)			//相对测阵检测其他信标的ID————第二个
#define JCXBID3		(5)			//相对测阵检测其他信标的ID————第三个
#define JCXBID4		(6)			//相对测阵检测其他信标的ID————第四个
#endif
//--------------------------------5号信标-------------------------------------------------
#if (XBID == 5)
#define FSK_f1 		(6)			//FSK频率1
#define FSK_f2 		(12)		//FSK频率2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES21	//IIF
#define ZFSY 		SY_5		//转发时延
#define JCIIF1		RAMSES07	//相对测阵检测其他信标的IIF————第一个
#define JCIIF2		RAMSES15	//相对测阵检测其他信标的IIF————第二个
#define JCIIF3		RAMSES11	//相对测阵检测其他信标的IIF————第三个
#define JCIIF4		RAMSES09	//相对测阵检测其他信标的IIF————第四个
#define JCSY1 		SY_1		//相对测阵检测其他信标需要减去的时延————第一个
#define JCSY2		SY_2		//相对测阵检测其他信标需要减去的时延————第二个
#define JCSY3		SY_3		//相对测阵检测其他信标需要减去的时延————第三个
#define JCSY4		SY_6		//相对测阵检测其他信标需要减去的时延————第四个
#define JCXBID1		(1)			//相对测阵检测其他信标的ID————第一个
#define JCXBID2		(2)			//相对测阵检测其他信标的ID————第二个
#define JCXBID3		(3)			//相对测阵检测其他信标的ID————第三个
#define JCXBID4		(6)			//相对测阵检测其他信标的ID————第四个
#endif
//--------------------------------6号信标-------------------------------------------------
#if (XBID == 6)
#define FSK_f1 		(7)			//FSK频率1
#define FSK_f2 		(13)		//FSK频率2
#define CIF 		RAMSES01	//CIF
#define IIF 		RAMSES09	//IIF
#define ZFSY 		SY_6		//转发时延
#define JCIIF1		RAMSES07	//相对测阵检测其他信标的IIF————第一个
#define JCIIF2		RAMSES15	//相对测阵检测其他信标的IIF————第二个
#define JCIIF3		RAMSES11	//相对测阵检测其他信标的IIF————第三个
#define JCIIF4		RAMSES21	//相对测阵检测其他信标的IIF————第四个
#define JCSY1 		SY_1		//相对测阵检测其他信标需要减去的时延————第一个
#define JCSY2		SY_2		//相对测阵检测其他信标需要减去的时延————第二个
#define JCSY3		SY_3		//相对测阵检测其他信标需要减去的时延————第三个
#define JCSY4		SY_5		//相对测阵检测其他信标需要减去的时延————第四个
#define JCXBID1		(1)			//相对测阵检测其他信标的ID————第一个
#define JCXBID2		(2)			//相对测阵检测其他信标的ID————第二个
#define JCXBID3		(3)			//相对测阵检测其他信标的ID————第三个
#define JCXBID4		(5)			//相对测阵检测其他信标的ID————第四个
#endif


#endif /* ID_CHOOSE_H_ */
