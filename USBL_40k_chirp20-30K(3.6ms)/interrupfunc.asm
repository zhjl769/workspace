        .global _Delay_diff_1us, int0_Isr;,timer1Isr ,timer0Isr
	
	
		.bss  _count_timer,1
		.bss  _responder_flag,1
        .noremark 5672,5674,5675,5571
		.global _count_timer
		.global _responder_flag

		;;;;;;;;;;;;;�ж�0�ķ������
int0_Isr:
      
	  mov   #1, *(#_responder_flag)

	  add	#1, *(#_count_timer)
	  mov   *(#_count_timer), ac1
	  sub   #3,ac1
	  bcc   endloop_int0,ac1!=#0
	  and   #0fbefh,port(#1002h)	;�򿪶�ʱ��0 
	  and   #0fffbh,mmap(IER0)	;�ر�int0�ж�

      mov   #0, *(#_count_timer)

endloop_int0:
      reti


;;;;;;;;;;;;;;;;;�ӳٳ���;;;;;;;;;;;;;;;;;;;;
_Delay_diff_1us:
      ;mov dbl(*(#_time_delay2)),ac1
	  mov dbl(*ar0),ac1
	  bcc end_delay_1us,ac1==#0
lop_1us:
      rpt #0b9h     ;�ӳ�1us   c7-6-6=bb -2
	  nop
	  sub #1,ac1
	  bcc end_delay_1us,ac1==#0  ;bccָ���ڼ�ʱռ��5����,subָ��ռ��1����.����ڵȴ�ʱ��ȥ6
	  b   lop_1us         ;bָ��ռ��6����
end_delay_1us:
	  ret


.end