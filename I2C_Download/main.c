/*
 * Copyright (C) 2003 Texas Instruments Incorporated
 * All Rights Reserved
 */
#include <csl.h>
#include <csl_pll.h>
#include <csl_i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <file.h>
#include "System.h"
#include "I2C_function.h"

//---------Global data definition---------
#define Page_Length  128          //Page_Length为写入的芯片按照页写入一次能够写入的最大长度值

Uint8 Rtmp[Page_Length];
Uint8 tmp[Page_Length];

/* Reference start of interrupt vector table       */
/* This symbol is defined in the file, vectors.s55 */
extern void VECSTART(void);
//---------main routine---------
void main(void)
{
	FILE *fp = NULL;
	Uint32 i, j;
	Uint32 addr = 0;   //写入从IIC数据地址
	Uint32 len = 0;    //打开文件总长度
	Uint32 block;      //分块
	Uint32 blockl;     //剩余分块
	/* Initialize CSL library - This is REQUIRED !!! */
	CSL_init();
	/* Set IVPD/IVPH to start of interrupt vector table */
	IRQ_setVecs((Uint32)(&VECSTART));
	PLL_config(&PLLconfig);

	/* CSL的I2C SETUP函数有问题，不推荐使用，包括写入和读取函数，需要将I2CSTR寄存器中的BB位置1*/
	I2C_FSET(I2CSTR,BB,0x1);
	I2C_config(&I2Cconfig);
	I2C_RSET(I2COAR, 0x0000);  //主IIC模块自身的地址

	if ((I2C_getEventId()) != 0)
		printf("AT24C512B ID is %d.\r\n",(Uint16)(I2C_getEventId()));
	else
	{
		printf("AT24C512B有问题，请更换！！！\r\n");
		while(1);
	}

	if ((fp = fopen("D:\\Program Files\\bin\\input.bin", "rb")) != NULL)
	{
		printf("Open input.bin success!\r\n");
		addr = 0;
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		printf("File size is %d%04d byte.\r\n", (Uint16)(len/10000), (Uint16)(len%10000));
		block=len/(sizeof(tmp));
		blockl=len%(sizeof(tmp));
		printf("Split Block Count is %d.\r\n",(Uint16)block);
		printf("Split Block Left is %d.\r\n",(Uint16)blockl);
		fseek(fp, 0, SEEK_SET);
		/*
		 * 对AT24C512进行擦除，一般情况不推荐使用，耗时太长，自动对没有写入的数据做擦除

		memset(tmp, 0xff, sizeof(tmp));
		addr = block*Page_Length;
		j = (524288 - addr)/Page_Length; //524288为AT24C512B总的数据长度，Page_Length为写入的芯片按照页写入一次能够写入的最大长度值
		for (i = 0; i < j; i++)
		{
			I2C_Write(addr, tmp, sizeof(tmp), 0x0050);
			delay(30000);
			addr += sizeof(tmp);
		}
		addr = 0;
		 */
		if(block > 0)
		{
			for (i = 0; i < block; i++)
			{
				memset(tmp, 0, sizeof(tmp));
				memset(Rtmp, 0, sizeof(Rtmp));

				for (j=0; j < sizeof(tmp); j++)
					tmp[j] |= (fgetc(fp));

				I2C_Write(addr, tmp, sizeof(tmp), 0x0050);  //分页写入128Bytes的数据
				delay(30000);                               //写入数据后一定要延时一段时间，让芯片完成数据的写入
				I2C_Read(addr, Rtmp, sizeof(Rtmp), 0x0050); //读出写入的128Bytes的数据
				if (check(tmp,Rtmp,(sizeof(tmp))) == -1)    //写入数据与读出的数据进行校验
				{
					printf("Check error !!!\r\n");
					printf("Block %d !!!\r\n",(Uint16)i);
					fclose(fp);
					while(1);
				}
				addr += sizeof(tmp);
			//	delay(30000);
			}
			printf("write-%d Check OK!\r\n", (Uint16)i);
		}

		if (blockl > 1)
		{
			memset(tmp, 0, sizeof(tmp));
			memset(Rtmp, 0, sizeof(Rtmp));
			for (j=0; j < blockl; j++)
			{
				tmp[j] |= (fgetc(fp));
			}

			I2C_Write(addr, tmp, blockl, 0x0050);  //分页写入128Bytes的数据
			delay(30000);                          //写入数据后一定要延时一段时间，让芯片完成数据的写入
			I2C_Read(addr, Rtmp, blockl, 0x0050);  //读出写入的128Bytes的数据
			if (check(tmp,Rtmp,blockl) == -1)      //写入数据与读出的数据进行校验
			{   perror("Check error");
			printf("Block Left !!!\r\n");
			fclose(fp);
			while(1);
			}
			printf("write-Left Check OK! \r\n");
		}
		addr += j;
		printf("Write flash finished.\r\n");
		printf("Total: %d%04d byte. \r\n", (Uint16)(addr/10000), (Uint16)(addr%10000));
		perror("OK!!!!!");
		fclose(fp);
	}
	else
	{
		printf("Cann\'t open input.bin\r\n");
		fclose(fp);
	}
	while(1);
}

