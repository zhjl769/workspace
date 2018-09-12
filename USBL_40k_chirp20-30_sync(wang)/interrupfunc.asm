        .global _Delay_diff_1us, int0_Isr;,timer1Isr ,timer0Isr
	
	
		.bss  _count_timer,1
		.bss  _responder_flag,1
        .noremark 5672,5674,5675,5571
		.global _count_timer
		.global _responder_flag

		;;;;;;;;;;;;;中断0的服务程序
int0_Isr:
      
	  mov   #1, *(#_responder_flag)

	  add	#1, *(#_count_timer)
	  mov   *(#_count_timer), ac1
	  sub   #3,ac1
	  bcc   endloop_int0,ac1!=#0
	  and   #0fbefh,port(#1002h)	;打开定时器0 
	  and   #0fffbh,mmap(IER0)	;关闭int0中断

      mov   #0, *(#_count_timer)

endloop_int0:
      reti


;;;;;;;;;;;;;;;;;延迟程序;;;;;;;;;;;;;;;;;;;;
_Delay_diff_1us:
      ;mov dbl(*(#_time_delay2)),ac1
	  mov dbl(*ar0),ac1
	  bcc end_delay_1us,ac1==#0
lop_1us:
      rpt #0b9h     ;延迟1us   c7-6-6=bb -2
	  nop
	  sub #1,ac1
	  bcc end_delay_1us,ac1==#0  ;bcc指令在假时占用5周期,sub指令占用1周期.因此在等待时减去6
	  b   lop_1us         ;b指令占用6周期
end_delay_1us:
	  ret


.end