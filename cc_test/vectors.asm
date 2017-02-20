		.ref   _c_int00, _myhDma_Isr, _dma0_Isr, _int1_Isr, _int0_Isr,
		 _timer0_Isr, _timer1_Isr, _int2_Isr , _no_isr

		.sect ".vectors"
       
rset	.ivec  	_no_isr
nmi		.ivec	_no_isr
int0	.ivec	_no_isr
int2	.ivec	_no_isr
tint0	.ivec	_timer0_Isr
rint0	.ivec	_no_isr
rint1	.ivec	_no_isr
xint1	.ivec	_no_isr
usb		.ivec	_no_isr
dmac1	.ivec	_no_isr
dspint	.ivec	_no_isr
int3	.ivec	_no_isr
rint2	.ivec   _no_isr
xint2	.ivec	_no_isr
dmac4	.ivec	_no_isr
dmac5	.ivec	_no_isr
int1	.ivec	_no_isr
xint0	.ivec	_no_isr
dmac0	.ivec	_no_isr
int4	.ivec	_no_isr
dmac2	.ivec	_no_isr
dmac3	.ivec	_no_isr
tint1	.ivec	_no_isr
iic		.ivec	_no_isr
berr	.ivec	_no_isr
dlog	.ivec	_no_isr
rtos	.ivec	_no_isr
sint27	.ivec	_no_isr
sint28	.ivec	_no_isr
sint29	.ivec	_no_isr
sint30	.ivec	_no_isr
sint21	.ivec	_no_isr


