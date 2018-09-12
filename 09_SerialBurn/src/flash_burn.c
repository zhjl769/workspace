/*********************************************************************
 * Filename:      flash_burn.c
 * Description:   支持 AT25FX系列24位地址spi flash 烧写，带校验功能
 *
 ********************************************************************/

#include <csl.h>
#include <csl_pll.h>
#include <csl_chip.h>
#include <csl_gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <csl_irq.h>
#include <csl_mcbsp.h>
#include <file.h>

#include "AT25F1024.h"

MCBSP_Handle mcbsp_handle;


extern void VECSTART(void);
Uint16 Rtmp[128];
Uint16 tmp[128];

int check( unsigned short *sword, unsigned short *dword,int len)
{
    while (len>0)
    {
        if (*sword!= *dword)
        {
            return -1;
        }
        sword++;
        dword++;
        len--;

    }

    return 1;


}

void main(void)
{
    FILE *fp=NULL;
    Uint32 i, j;
    Uint32 addr=0;
    Uint32 len=0;
    Uint32 block;//分块
    Uint32 blockl;//剩余分块
    /* Init CSL Lib*/
    CSL_init();
    IRQ_globalDisable();

    /*Init McBSP0*/
    mcbsp_handle = MCBSP_open(MCBSP_PORT0, MCBSP_OPEN_RESET);
    MCBSP_FSET(PCR0,XIOEN,1);
    MCBSP_FSET(PCR0,RIOEN,1);
    MCBSP_FSET(PCR0,FSXM,1);
    MCBSP_FSET(PCR0,FSRM,0);
    MCBSP_FSET(PCR0,CLKXM,1);
    MCBSP_FSET(PCR0,CLKRM,0);

    /* Set GPIO4 to Output*/
    GPIO_FSET(IODIR, IO4DIR, 1);

    flash_init();

    /* /\* unprotect all the blocks *\/ */
    
    flash_write_status_register(0);
    flash_write_disable();
    printf("Flash status register: 0x%02x\n\r", flash_read_status_register());
    printf("Flash  ID: 0x%06x \n\r", flash_jedec_id_read());
    printf("Addr 0x000001: 0x%02x.\n\r", flash_read_byte(0x01));  //读地址数据
    printf("Addr 0x000007: 0x%02x.\n\r", flash_read_byte(0x07));

    /* erase flash */
    printf("Chip erasing....\n\r");
    flash_chip_erase();
    
    printf("Chip erase finish.\n\r");
    printf("Addr 0x000001: 0x%02x.\n\r", flash_read_byte(0x01));  //读地址数据，观察擦除是否完成
    printf("Addr 0x000007: 0x%02x.\n\r", flash_read_byte(0x07));

    if ((fp = fopen("..\\OutToBin\\input.bin", "rb")) != NULL)  //读文件成功
    {
        printf("Open input.bin success!\n\r");

        addr = 0;
        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        printf("File size is %d%04d byte.\n\r", (Uint16)(len/10000), (Uint16)(len%10000));//数据拆分显示。超过16位数据显示不了
        block=len/(sizeof(tmp)<<1);  
        blockl=len%(sizeof(tmp)<<1)+1;
		printf("Split Block Count is %d.\n\r",(Uint16)block);
		printf("Split Block Left is %d.\n\r",(Uint16)blockl);

        fseek(fp, 0, SEEK_SET);
        for (i=0; i<block; i++)
        {
            memset(tmp, 0, sizeof(tmp)); //清0 
            for (j=0; j<sizeof(tmp); j++)   //16位数据，故128个元素装载256个数据
            {
                tmp[j] = fgetc(fp)<< 8;  //高8位
                tmp[j] |= (fgetc(fp) );  //低8位   
            }

            flash_write_nword(addr, tmp, sizeof(tmp));
            flash_fast_read_nbyte(addr,Rtmp,sizeof(Rtmp));
            
            if (check(tmp,Rtmp,sizeof(tmp))==-1)
            {   perror("Check error");
                printf("Block %d !!!\n\r",(Uint16)i);

                 while(1);

            }
            printf("write-%d Check OK!\n\r", (Uint16)i); 
            addr += (sizeof(tmp) << 1);

        }

        if (blockl>1)//分块后剩余数据
        {
	 
            memset(tmp, 0, sizeof(tmp));
            for (j=0; j<blockl/2; j++)  
            {
                tmp[j] = fgetc(fp)<< 8;
                tmp[j] |= (fgetc(fp) );
            }
            flash_write_nword(addr, tmp,blockl/2);
            flash_fast_read_nbyte(addr,Rtmp,blockl/2);
            
            if (check(tmp,Rtmp,blockl/2)==-1)
            {   perror("Check error");
                printf("Block Left !!!\n\r");
                 while(1);
            }
            printf("write-Left Check OK! \n\r");
			

        }

        addr += j*2;
        printf("\n\rWrite flash finished.\n\r");
        printf("Total: %d%04d byte. \n\r", (Uint16)(addr/10000), (Uint16)(addr%10000));
		perror("OK!!!!!");
        fclose(fp);    
    }
    else
    {
		printf("Cann\'t open input.bin\n\r");

    }

    while(1);
}
