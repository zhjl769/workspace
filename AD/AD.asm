;程序说明：AD7663测试程序与DSP用McBSP1连接
;		   测试采样率为100kHz
                .title   "AD.asm"
                .include "regs.asm"
                .include "vector.asm"
                .global main

stack_len                   .set 100
sysstack_len                .set 100
stack                       .usect "stack",stack_len
sysstack                    .usect "stack",sysstack_len

f_flag1          .usect  "variable",1
content1         .usect  "variable",1
length1          .usect  "variable",1
_Data1          .usect  "data1",4096
                
                    .text
main:
         bset  c54cm
        .c54cm_on
    
        bset  intm         
        bclr  M40  
        amov  #(stack+stack_len),xsp               
		mov   #(sysstack+sysstack_len),ssp 		
		
		mov   #2c92h,port(#CLKMD)     ;2712配置时钟倍频25倍，200MHz
		mov   #002ch,mmap(IVPH)       
		mov   #002ch,mmap(IVPD)   
		mov   #0ffffh,mmap(IFR0)        
		mov   #0ffffh,mmap(IFR1)
		mov   #0020h,mmap(IER0)
		mov   #0000h,mmap(IER1)
		
			
        mov   #0,*(#f_flag1)               
        mov   #0,*(#content1)       
        mov   #0,*(#length1)
     
        mov   #_Data1,AR5 
        mov   #1000h,*(#length1)
        call  initmcbsp	            	         
        bclr  intm                 
        bclr  SXMD 
        ;bset FRCT                  		
		nop
		
wait:  
 
        mov   #00c1h,port(#SPCR2_0);00c1

		mov   #0001h,port(#SPCR1_0)	         
      
        ;b     wait
recieve:
              
        mov  *(#f_flag1),AC0 
        nop
        nop                  
        bcc recieve,AC0==#0
        nop
        nop 
        mov  #0,*(#f_flag1)
               
go1:    mov	  port(#DRR1_0),AC0		
		nop
		nop	       


        mov   AC0,*AR5+               
        mov   *(#content1),AC1	   
		sub   *(#length1),AC1	
		bcc   here,AC1==#0        
		nop	
		nop
	    b    recieve
	     
	    

              
here:   
        mov   #0000h,port(#SPCR2_0)             
        mov   #0000h,port(#SPCR1_0)
        mov   #0000h,*(#content1)       
        mov   #_Data1,AR5
           
        nop
        b     wait
        
initmcbsp:
mcbsp1_init:
        mov   #0000h,port(#SPCR2_0)
        mov   #0000h,port(#SPCR1_0)
        mov   #0005h,port(#RCR2_0);04h忽略不期望的帧同步信号;;RDATDLY=00;0-bit数据延迟
        mov   #0040h,port(#RCR1_0);字长为16位RWDLEN1=010

        mov   #0000h,port(#XCR2_0);
        mov   #0040h,port(#XCR1_0);A0
        mov   #3027h,port(#SRGR2_0);3013h帧同步周期为20个CLKG;;;GSYNC=0;CLKSP=0;CLKSM=1;FSGM=1;FPER=19
        mov   #0231h,port(#SRGR1_0);363h帧同步脉冲宽度为4个CLKG周期，CLKG分频系数为100;;FWID=3;CLKGDV=99;;fs=200MHz/(99+1)/(3+1+16)=100kHz;FWID=2;CLKGDV=69;;fs=100kHz
        mov   #0b0dh,port(#PCR0)   ;IDLEEN=0;XIOEN=0;RIOEN=0;FSXM=1;FSRM=0;CLKXM=1;CLKRM=0;SCLKME=0;CLKSSTAT=0;DXSTAT=0;DRSTAT=0;FSXP=1;FSRP=0;CLKXP=0;CLKRP=0

        mov   #0001h,port(#SPCR2_0)
     	mov   #0000h,port(#SPCR1_0)
;        rpt   #2bch
;    	nop
        mov   #0001h,port(#SPCR2_0)
		mov   #0000h,port(#SPCR1_0)
		nop
		nop
        ret
        
rint0_isr:       
        mov   #0001,*(#f_flag1)
		add   #1,*(#content1)
        nop       
        nop                                
        reti                	            	    
no_isr: nop
        reti
        .end        

        


		
		
		
		
       
		
		       
		              
		                     
		                            
		                                   
		                                          
		                                                 
		                                                        
		                                                               
		                                                                      
		                                                                             
		                                                                                    
		                                                                                                  
 
