*------------------------------------------------------------------------------------
* \file    I2C.pjt and I2C.asm
* Describe how to use I2C module for bootloader of TMS320C5509/5509A
* This file show the memory map and values of I2C registers.\n
* Also, it defines the basic I/O operations to access I2C module.*
* \author zhangzhe (HEU)
* \date 08/06/2009
* \version 1.1.0*
* Date        Version  Author       Description
* 18/05/2009  1.0.0    zhangzhe  Refer to AT24C512B.pdf Realize the basic r/w operation of AT24C512B
* 03/06/2009  1.0.1    zhangzhe  Refer to 5509boot.pdf Realize basic I2C boot of test programme
* 08/06/2009  1.1.0    zhangzhe  Realize I2C bootloader of Doppler_master.out
* 25/06/2009  1.1.1    zhangzhe  Add wait time 0.2s,"run" ok!
;7.21                            
*-------------------------------------------------------------------------------------
        .title   "I2C_bootloader.asm"
        .include "regs.asm"
        .include "vector.asm" 
        .include "USBL.asm"          ;第1/2：在此更改文件名   
        .global   start
        .data    
stack_len		.set	200
sysstack_len	.set	200
boot_len        .set    17100        ;第2/2:在此更改boot_len的长度
read_len        .set    128
test            .set    0xcc
stack			.usect	"stack",stack_len
sysstack		.usect	"stack",sysstack_len 
pageaddress     .usect  "buffer",1
wordaddress     .usect  "buffer",2
data_counter1   .usect	"buffer",1
data_counter2   .usect	"buffer",1
data_counter3   .usect	"buffer",1
data_counter4   .usect	"buffer",1
flag1           .usect	"buffer",1
flag2           .usect	"buffer",1
receive_data    .usect	"buffer",200
transmit_data1  .usect	"buffer",300              
       .text
start:   bclr  c54cm
        .c54cm_off        	 
       bset  intm    
       bclr  M40                 
       amov  #(stack+200),xsp 
	   mov   #(sysstack+200),ssp 
       mov   #2312h,port(#CLKMD);48M	
	   mov   #0201h,port(#EBSR)	   
	   mov   #035h,mmap(IVPH)     
	   mov   #035h,mmap(IVPD) 	   
	   mov   #0000h,mmap(IER1)
	   mov   #0ffffh,mmap(IFR1)
	   mov   #0000h,mmap(IER0)
	   mov   #0ffffh,mmap(IFR0)
	   mov   #0080h,mmap(IER1)	  ;使能I2C中断
	   mov   #0,*(#data_counter1)
	   mov   #0,*(#data_counter2)
	   mov   #0,*(#data_counter3)
	   mov   #0,*(#data_counter4)
	   mov   #0,*(#pageaddress)
	   mov   #0,*(#wordaddress)   
	   mov   #0,*(#flag1)
	   mov   #0,*(#flag2)	   
	   mov   #receive_data,ar1
	   mov   #boot,ar2
	   mov   #wordaddress,ar3	   
	   mov   #transmit_data1,ar4
init_i2c	   	   
	   mov   #0000h,port(#I2CMDR);i2c module reset  复位I2C模式寄存器
	   mov   #0000h,port(#I2CMDR2);i2c module reset	 复位I2C模式2寄存器,特别是使能或者禁止I2C 	   
	   mov   #0000h,port(#I2COAR);i2c own slave address 复位I2C自身的10bit地址
	   mov   #000fh,port(#I2CPSC);i2c mode_clock 3M  设置预分频计数器，产生I2C模块的时钟频率：48MHz/（15+1）=3MHz
;	   mov   #0037h,port(#I2CCLKL);i2c SCL_clock 25k
;	   mov   #0037h,port(#I2CCLKH);i2c SCL_clock 25k
	   mov   #000ah,port(#I2CCLKL);i2c SCL_clock 100k 设置I2C的SCL时钟= 3MHz/【（10+d）*（10+d）】；其中d=5，因为IPSC>1,SCL= 13.333kHz
	   mov   #000ah,port(#I2CCLKH);i2c SCL_clock 100k
;	   mov   #0000h,port(#I2CCLKL);i2c SCL_clock 300k
;	   mov   #0000h,port(#I2CCLKH);i2c SCL_clock 300k
	   mov   #0000h,port(#I2CIER);i2c close all interrupts
	   mov   #00ffh,port(#I2CCNT);i2c data count
	   mov   #0ffffh,port(#I2CSTR);i2c interrupts 写1进行复位
	   mov   #0ffffh,port(#I2CIER);i2c open XRDY/RRDY interrupt	   
	   mov   #0020h,port(#I2CMDR);i2c module enable  set IRS 使能I2C模块
	   rpt   #10
	   nop	   
	   ;b read_i2c
***************************************************************************
       ;i2c write module
write_i2c  
       mov   #0050h,port(#I2CSAR);i2c slave address      设置I2C从设备地址，E2PROM芯片文档要求该设备地址：10100 A1 A0 硬件连接时A1A0=00;
;       rpt   #10 
       nop       
       or    #0680h,port(#I2CMDR);send mode/master mode SCL 100khz  设置为循环模式，不管CNT计数器的值，直到手动将STP置1
 ;      rpt   #10
       nop
check_busy  
	   mov   port(#I2CSTR),ac0  ;读状态寄存器 确定此时I2C总线是否空闲
       and   #1000h,ac0
       rpt   #10
       nop
       bcc   check_busy,ac0!=#0 
       bclr  intm       ;执行此处，表示总线空闲 ，打开全局中断
;       rpt   #10
       nop      
*****************************page write****************************
       or    #2000h,port(#I2CMDR);start signal  产生开始条件
       rpt   #10
       nop
       mov   #0050h,port(#I2CSAR);i2c slave address  设置从地址
;       rpt   #10
       nop
check_ACK0       
      mov   port(#I2CSTR),ac0  ;测试从设备是否有握手
      and   #0002h,ac0       
      bcc   check_ACK0,ac0!=#0     
      rpt   #100    ;执行此处标明NACK=0,接到一个握手成功信号
      nop                 
send_wordaddr1   
       mov   *ar3+,ac1   ;发送第一个7bit地址 wordaddress->ar3
       mov   ac1,port(#I2CDXR)
       ;rpt   #10000
       nop
check_XRDY1      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0
       bcc   check_XRDY1,ac0==#0     
       ;rpt   #10000
       nop             ;执行此处标明XRDY1=1,DXR数据就绪，准备发送   
check_ACK1       
      mov   port(#I2CSTR),ac0  ;测试从设备是否有握手
      and   #0002h,ac0       
      bcc   check_ACK1,ac0!=#0     
      rpt   #100       ;执行此处标明NACK=0,接到一个握手成功信号
      nop  
      mov   #20000,mmap(CSR)  ;设置循环寻址寄存器  ？？？？
      RPT   CSR
      NOP
	  nop
send_wordaddr2 
       mov   *ar3,ac1    ;发送第二个7bit地址 wordaddress->ar3
       mov   ac1,port(#I2CDXR)
       mov   #wordaddress,ar3
       rpt   #100
       nop
check_XRDY2      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0
       bcc   check_XRDY2,ac0==#0     
       ;rpt   #10000
       nop
check_ACK2
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK2,ac0!=#0     
      rpt   #100
      nop                 ;测试如上，就是根据E2PROM文档

send_highbyte   
       mov   *ar2,ac2   ;boot->ar2  开始写数据
       sftl  ac2,#-8    ;移除低字节
       and   #00ffh,ac2
       ;mov   #test,ac2
       ;mov   ac2, *ar4+
       mov   ac2,port(#I2CDXR)
       ;rpt   #20000
       nop 
check_XRDY3      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0
       bcc   check_XRDY3,ac0==#0     
       ;rpt   #10000
       nop
check_ACK3       
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK3,ac0!=#0     
      rpt   #100
      nop  
       or    #0800h,port(#I2CMDR);stop signal  手动停止
       rpt   #200
       nop 
**************************************************************************
waitfor1 
	   add   #1,*(#data_counter4)
	   mov   *(#data_counter4),ac0
       sub   #10000,ac0
       bcc   waitfor1,ac0!=#0	  
	   mov   #0,*(#data_counter4)
       ;rpt   #20000
       nop       
**********************************************************************
***********************************word address***********************
       add   #1,*(#pageaddress)   
sendaddr_highbyte   
       mov   *(#pageaddress),ac1
       sftl  ac1,#-8
       and   #00ffh,ac1
       mov   ac1,*ar3+ 
       nop 
       rpt   #20000
       nop
sendaddr_lowbyte   
       mov   *(#pageaddress),ac1
       and   #00ffh,ac1
       mov   ac1,*ar3
       rpt   #20000
       nop
       nop      
	   mov   #wordaddress,ar3

**************************************************************************
****************************************************
write_i2c1  
       mov   #0050h,port(#I2CSAR);i2c slave address      
       rpt   #10 
       nop       
       or    #0680h,port(#I2CMDR);send mode/master mode SCL 100khz
       rpt   #10
       nop
check_busy1  
	   mov   port(#I2CSTR),ac0
       and   #1000h,ac0
       rpt   #10
       nop
       bcc   check_busy1,ac0!=#0 
       bclr  intm       
       rpt   #10
       nop      
*****************************page write1****************************
       or    #2000h,port(#I2CMDR);start signal  
       rpt   #10
       nop
       mov   #0050h,port(#I2CSAR);i2c slave address
       rpt   #10
       nop
check_ACK01       
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK01,ac0!=#0     
      rpt   #100
      nop                 
send_wordaddr4   
       mov   *ar3+,ac1
       mov   ac1,port(#I2CDXR)
       rpt   #100
       nop
check_XRDY4      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0
       bcc   check_XRDY4,ac0==#0     
       ;rpt   #10000
       nop
check_ACK4       
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK4,ac0!=#0     
      rpt   #100
      nop  
     ; mov   #20000,mmap(CSR) 
     ; RPT   CSR
      NOP
	  nop
send_wordaddr5 
       mov   *ar3,ac1
       mov   ac1,port(#I2CDXR)
       mov   #wordaddress,ar3
       rpt   #100
       nop
check_XRDY5      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0
       bcc   check_XRDY5,ac0==#0     
       ;rpt   #10000
       nop
check_ACK5
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK5,ac0!=#0     
      rpt   #100
      nop  
send_lowbyte   
       mov   *ar2+,ac2
       and   #00ffh,ac2
       ;mov   #test,ac2
       ;mov   ac2, *ar4+
       mov   ac2,port(#I2CDXR)

       ;rpt   #20000
       nop
check_XRDY6      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0
       bcc   check_XRDY6,ac0==#0     
       ;rpt   #10000
       nop
check_ACK7       
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK7,ac0!=#0     
      rpt   #100
      nop 
********************************  
check_busystop1  
	   mov   port(#I2CSTR),ac0
       and   #1000h,ac0
       rpt   #10
       nop
       bcc   check_busystop1,ac0==#0 
       bclr  intm       
       rpt   #10
       nop 
       ;rpt   #20000
       nop
       
       or    #0800h,port(#I2CMDR);stop signal
       rpt   #200
       nop 
**************************************************************************
waitfor 
	   add   #1,*(#data_counter4)
	   mov   *(#data_counter4),ac0
       sub   #10000,ac0
       bcc   waitfor,ac0!=#0	  
	   mov   #0,*(#data_counter4)
       ;rpt   #20000
       nop       
**********************************************************************

***********************************word address1***********************
       add   #1,*(#pageaddress)   
sendaddr_highbyte1   
       mov   *(#pageaddress),ac1
       sftl  ac1,#-8
       and   #00ffh,ac1
       mov   ac1,*ar3+ 
       nop 
       rpt   #20000
       nop

sendaddr_lowbyte1   
       mov   *(#pageaddress),ac1
       and   #00ffh,ac1
       mov   ac1,*ar3
       rpt   #20000
       nop
       nop      
	   mov   #wordaddress,ar3
*********************************************************************
       add   #1,*(#data_counter1)

       mov   *(#data_counter1),ac3
       sub   #boot_len,ac3
       bcc   write_i2c,ac3!=#0 

	   mov   #transmit_data1,ar4  
sendover
	   mov   #boot,ar2
	   
       rpt   #200
       nop
       mov   #1,*(#flag1)
       nop 
       ;b finish
*********************************************************************************
;;;;;;;;;;;;;;;;;;;*********************************************************************************
**********************************************
waitfor2 
	   add   #1,*(#data_counter3)
	   mov   *(#data_counter3),ac3 

       sub   #5000,ac3;
       bcc   waitfor2,ac3!=#0
***********************************************
init_i2c1
	   mov   #0,*(#data_counter2)	   	   
	   mov   #0000h,port(#I2CMDR);i2c module reset
	   mov   #0000h,port(#I2CMDR2);i2c module reset	   	   
	   mov   #0000h,port(#I2COAR);i2c own slave address
	   mov   #000fh,port(#I2CPSC);i2c mode_clock 12M
	   mov   #000ah,port(#I2CCLKL);i2c SCL_clock 100k
	   mov   #000ah,port(#I2CCLKH);i2c SCL_clock 100k
	   mov   #0000h,port(#I2CIER);i2c close all interrupts
	   mov   #00ffh,port(#I2CCNT);i2c data count
	   mov   #0ffffh,port(#I2CSTR);i2c interrupts
	   mov   #0ffffh,port(#I2CIER);i2c open XRDY/RRDY interrupt	   
	   mov   #0020h,port(#I2CMDR);i2c module enable	 
**************************************************
       ;i2c read module
read_i2c       
       or    #0680h,port(#I2CMDR);send mode/master mode SCL 100khz
       rpt   #10
       nop
check_busy2  
	   mov   port(#I2CSTR),ac0
       and   #1000h,ac0
       rpt   #10
       nop
       bcc   check_busy2,ac0!=#0 
       bclr  intm       
       rpt   #10
       nop
       rpt   #10
       nop
       or    #2000h,port(#I2CMDR);start signal
       rpt   #10
       nop
	   mov   #0050h,port(#I2CSAR);i2c slave address  
check_ACK8       
       mov   port(#I2CSTR),ac0
       and   #0002h,ac0       
       bcc   check_ACK8,ac0!=#0     
       rpt   #100
       nop  
	   nop
check_XRDY8      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0    
       bcc   check_XRDY8,ac0==#0     
       rpt   #10
       nop
send_wordaddr3   
       ;mov   *ar3+,ac1
       mov   #00h,ac1
       mov   ac1,port(#I2CDXR)
       rpt   #100
       nop
check_ACK9       
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK9,ac0!=#0     
      rpt   #100
      nop
check_XRDY9      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0    
       bcc   check_XRDY9,ac0==#0     
       rpt   #10
       nop
send_wordadd5 
       ;mov   *ar3+,ac1
       mov	 #00h,ac1
       mov   ac1,port(#I2CDXR)
       mov   #wordaddress,ar3
       rpt   #100
       nop
check_ACK10       
      mov   port(#I2CSTR),ac0
      and   #0002h,ac0       
      bcc   check_ACK10,ac0!=#0     
      rpt   #100
      nop 
check_XRDY10      
       mov   port(#I2CSTR),ac0
       and   #0010h,ac0    
       bcc   check_XRDY10,ac0==#0     
       rpt   #10
       nop
       mov   #50000,mmap(CSR);#20000,mmap(CSR) 
       RPT   CSR
       NOP 
	   nop
*****************************************************************
*由写时序转换读时序，可参见AT24C512B.pdf手册。09.05.16       
*****************************************************************
	   rpt   #10
	   nop     
       and   #04a0h,port(#I2CMDR)
	   or   #0008h,port(#I2CIER);i2c open RRDY interrupt
       rpt   #10
       nop      
       bclr  intm   
       rpt   #10
       nop                                                                                                  
       or    #2000h,port(#I2CMDR) ;start signal           
       rpt   #10
       nop
       mov   #0050h,port(#I2CSAR);i2c slave address 
       rpt   #10
       nop
**********************************
********************************
receive1_lowbyte
       mov   port(#I2CDRR),ac1        
       add	 ac2,ac1 
       mov   ac1,*ar1+
       ;and   #00ffh,ar1
       rpt   #50000
       nop
       nop
********************************
check_ACK12       
       mov   port(#I2CSTR),ac0
       and   #0002h,ac0       
       bcc   check_ACK12,ac0!=#0     
       rpt   #100
       nop 
receive_highbyte
       mov   port(#I2CDRR),ac2       
       sftl  ac2,#8
       and   #0ff00h,ac2
       ;mov   ac2,*ar1
       rpt   #10000
       nop
       nop
check_ACK13       
       mov   port(#I2CSTR),ac0
       and   #0002h,ac0       
       bcc   check_ACK13,ac0!=#0     
       rpt   #100
       nop 
receive_lowbyte
       mov   port(#I2CDRR),ac1        
       add	 ac2,ac1       
       mov   ac1,*ar1+       
       
 ;      mov   *ar2+,ac0
;       mov   *ar2+,ac2       
;       sub   ac0,ac1
 ;      bcc   fail,ac1!=#0
       ;mov   ac1,*ar1+
       ;rpt   #50000
check_ACK14       
       mov   port(#I2CSTR),ac0
       and   #0002h,ac0       
       bcc   check_ACK14,ac0!=#0     
       rpt   #100
       nop 
       RPT   CSR
       NOP
	   NOP
check_RRDY2
       mov   port(#I2CSTR),ac0
       and   #0008h,ac0           
       bcc   check_RRDY2,ac0==#0 
       rpt   #10000
       nop
check_ACKSNT
	   mov   port(#I2CSTR),ac0
       and   #2000h,ac0           
       bcc   check_ACKSNT,ac0!=#0 
       NOP
       rpt   #10
       nop

       add   #2,*(#data_counter2)
       mov   *(#data_counter2),ac3
       sub   #read_len,ac3
       bcc   check_ACK12,ac3!=#0        
       
       mov   #receive_data,ar1              
       or    #0800h,port(#I2CMDR)                 
       rpt   #10000
       nop
       RPT   CSR
       NOP
       mov   #1,*(#flag2)
       nop       
***********************************************************************
;       mov   #55aah,ac1        
************
;       mov   #55aah,ac1        
       mov   #0,*(#data_counter4) 
;       add	 ac2,ac1 
 ;      mov   ac1,*ar1+
       mov   *ar1+,ac0
       mov   *ar1+,ac2
       mov   *ar2+,ac2
loop1
       mov   *ar1+,ac0       
       mov   *ar2+,ac1
       sub   ac1,ac0
       bcc   fail,ac0!=#0
       add   #2,*(#data_counter4)
       mov   *(#data_counter4),ac3
       sub   #120,ac3
       bcc   loop1,ac3!=#0   
       b     succeed
       ;and   #00ffh,ar1
fail   
       nop
       b     fail
*********************************************************************
;       b     finish
**********************************************************************
succeed:         
       nop
       b     succeed
*****************************************************

**********************************************************************

*************************************************************************
*****             interrupt service routine                        ******
************************************************************************* 
no_isr:        
       reti   
                  
       .end
