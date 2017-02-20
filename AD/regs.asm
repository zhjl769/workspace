	.title "regs.asm"

* EMIF REGISTER *
EGCR	.set	800H
EMI_RST	.set	801H
EMI_BE	.set 	802H
CE0_1	.set	803H
CE0_2	.set	804H
CE0_3	.set	805H
CE1_1	.set	806H
CE1_2	.set	807H
CE1_3	.set	808H
CE2_1	.set	809H
CE2_2	.set	80AH
CE2_3	.set	80BH
CE3_1	.set	80CH
CE3_2	.set	80DH
CE3_3	.set	80EH
SDC1	.set	80FH
SDPER	.set	810H
SDCNT	.set	811H
INIT	.set	812H
SDC2	.set	813H

* CLOCK MODE REGISTER *
CLKMD   .set    1C00H
*McBSP0 REGISTERS*
DRR2_0     .set    2800H 
DRR1_0     .set    2801H
DXR2_0     .set    2802H
DXR1_0     .set    2803H
SPCR2_0    .set    2804H
SPCR1_0    .set    2805H
RCR2_0     .set    2806H
RCR1_0     .set    2807H
XCR2_0     .set    2808H
XCR1_0     .set    2809H
SRGR2_0    .set    280AH
SRGR1_0    .set    280BH
MCR2_0     .set    280CH
MCR1_0     .set    280DH
RCERA_0    .set    280EH
RCERB_0    .set    280FH
XCERA_0    .set    2810H
XCERB_0    .set    2811H
PCR0       .set    2812H
RCERC_0    .set    2813H
RCERD_0    .set    2814H
XCERC_0    .set    2815H
XCERD_0    .set    2816H
RCERE_0    .set    2817H
RCERF_0    .set    2818H
XCERE_0    .set    2819H
XCERF_0    .set    281AH
RCERG_0    .set    281BH
RCERH_0    .set    281CH
XCERG_0    .set    281DH
XCERH_0    .set    281EH


*McBSP1 REGISTERS*
DRR2_1     .set    2C00H 
DRR1_1     .set    2C01H
DXR2_1     .set    2C02H
DXR1_1     .set    2C03H
SPCR2_1    .set    2C04H
SPCR1_1    .set    2C05H
RCR2_1     .set    2C06H
RCR1_1     .set    2C07H
XCR2_1     .set    2C08H
XCR1_1     .set    2C09H
SRGR2_1    .set    2C0AH
SRGR1_1    .set    2C0BH
MCR2_1     .set    2C0CH
MCR1_1     .set    2C0DH
RCERA_1    .set    2C0EH
RCERB_1    .set    2C0FH
XCERA_1    .set    2C10H
XCERB_1    .set    2C11H
PCR1       .set    2C12H
RCERC_1    .set    2C13H
RCERD_1    .set    2C14H
XCERC_1    .set    2C15H
XCERD_1    .set    2C16H
RCERE_1    .set    2C17H
RCERF_1    .set    2C18H
XCERE_1    .set    2C19H
XCERF_1    .set    2C1AH
RCERG_1    .set    2C1BH
RCERH_1    .set    2C1CH
XCERG_1    .set    2C1DH
XCERH_1    .set    2C1EH

*McBSP2 REGISTERS*
DRR2_2     .set    3000H 
DRR1_2     .set    3001H
DXR2_2     .set    3002H
DXR1_2     .set    3003H
SPCR2_2    .set    3004H
SPCR1_2    .set    3005H
RCR2_2     .set    3006H
RCR1_2     .set    3007H
XCR2_2     .set    3008H
XCR1_2     .set    3009H
SRGR2_2    .set    300AH
SRGR1_2    .set    300BH
MCR2_2     .set    300CH
MCR1_2     .set    300DH
RCERA_2    .set    300EH
RCERB_2    .set    300FH
XCERA_2    .set    3010H
XCERB_2    .set    3011H
PCR2       .set    3012H
RCERC_2    .set    3013H
RCERD_2    .set    3014H
XCERC_2    .set    3015H
XCERD_2    .set    3016H
RCERE_2    .set    3017H
RCERF_2    .set    3018H
XCERE_2    .set    3019H
XCERF_2    .set    301AH
RCERG_2    .set    301BH
RCERH_2    .set    301CH
XCERG_2    .set    301DH
XCERH_2    .set    301EH
*DMA  REGISTERS*
DMA_GCR    .set    0E00H
DMA_GSCR   .set    0E02H
DMA_GTCR   .set    0E03H
*通道0寄存器*
DMA_CSDP0  .set    0C00H
DMA_CCR0   .set    0C01H
DMA_CICR0  .set    0C02H
DMA_CSR0   .set    0C03H
DMA_CSSA_L0 .set   0C04H
DMA_CSSA_U0 .set   0C05H
DMA_CDSA_L0 .set   0C06H
DMA_CDSA_U0 .set   0C07H
DMA_CEN0    .set   0C08H
DMA_CFN0   .set    0C09H
*通道1寄存器*
DMA_CSDP1  .set    0C20H
DMA_CCR1   .set    0C21H
DMA_CICR1  .set    0C22H
DMA_CSR1   .set    0C23H
DMA_CSSA_L1 .set   0C24H
DMA_CSSA_U1 .set   0C25H
DMA_CDSA_L1 .set   0C26H
DMA_CDSA_U1 .set   0C27H
DMA_CEN1    .set   0C28H
DMA_CFN1    .set   0C29H
*通道2寄存器*
DMA_CSDP2  .set    0C40H
DMA_CCR2   .set    0C41H
DMA_CICR2  .set    0C42H
DMA_CSR2   .set    0C43H
DMA_CSSA_L2 .set   0C44H
DMA_CSSA_U2 .set   0C45H
DMA_CDSA_L2 .set   0C46H
DMA_CDSA_U2 .set   0C47H
DMA_CEN2    .set   0C48H
DMA_CFN2    .set   0C49H
*通道3寄存器*
DMA_CSDP3  .set    0C60H
DMA_CCR3   .set    0C61H
DMA_CICR3  .set    0C62H
DMA_CSR3   .set    0C63H
DMA_CSSA_L3 .set   0C64H
DMA_CSSA_U3 .set   0C65H
DMA_CDSA_L3 .set   0C66H
DMA_CDSA_U3 .set   0C67H
DMA_CEN3    .set   0C68H
DMA_CFN3    .set   0C69H
*通道4寄存器*
DMA_CSDP4  .set    0C80H
DMA_CCR4   .set    0C81H
DMA_CICR4  .set    0C82H
DMA_CSR4   .set    0C83H
DMA_CSSA_L4 .set   0C84H
DMA_CSSA_U4 .set   0C85H
DMA_CDSA_L4 .set   0C86H
DMA_CDSA_U4 .set   0C87H
DMA_CEN4    .set   0C88H
DMA_CFN4    .set   0C89H
*通道5寄存器*
DMA_CSDP5  .set    0CA0H
DMA_CCR5   .set    0CA1H
DMA_CICR5  .set    0CA2H
DMA_CSR5   .set    0CA3H
DMA_CSSA_L5 .set   0CA4H
DMA_CSSA_U5 .set   0CA5H
DMA_CDSA_L5 .set   0CA6H
DMA_CDSA_U5 .set   0CA7H
DMA_CEN5    .set   0CA8H
DMA_CFN5    .set   0CA9H

* EXTERNAL BUS SELECTION REGISTER *
EBSR   	.set    6C00H

* TIMERS *
TIM0	.set	1000H
PRD0	.set	1001H
TCR0	.set	1002H
PRSC0	.set	1003H
TIM1	.set	2400H
PRD1	.set	2401H
TCR1	.set	2402H
PRSC1	.set	2403H

* GPIO *
IODIR	.set	3400H
IODATA	.set	3401H
AGPIOEN   .set   4400h
AGPIODIR  .set   4401h
AGPIODATA  .SET  4402H


