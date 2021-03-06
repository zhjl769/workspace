/*
 * Interrupt.h
 *
 *  Created on: 2017-2-24
 *      Author: zhjl769
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_


//Interrupt Service Routine c_int08 : ISR to service EDMAINT.
//vecs_edma1.asm must be modified to include c_int08 entry.
interrupt void  EDMA0_int(void)
{
	//Clear the pending interrupt from the EDMA interrupt pending register
	EDMA_intClear(TCCINTNUM);
	pingpong = 10;
	if (pingpong != 10)
		//Now filling pong so set link to ping
		EDMA_link(hEdma,hEdmaPing);
	else
		//Now filling ping so set link to pong
		EDMA_link(hEdma,hEdmaPong);
}


//Interrupt Service Routine c_int08 : ISR to service Mcbsp0_RX_int.
interrupt void  Mcbsp0_RX_int(void)
{
	if (Buffdata > 1023)
	{
		Buffdata = 0;
		Buffflag = ~Buffflag;
	}
	if(Buffflag == 0)
	{
		while (!MCBSP_rrdy(My_Mcbsp0));
		ping[Buffdata] = MCBSP_read(My_Mcbsp0);
		Buffdata ++;
	}

	else
	{
		while (!MCBSP_rrdy(My_Mcbsp0));
		pong[Buffdata] = MCBSP_read(My_Mcbsp0);
		Buffdata ++;
	}
}


#endif /* INTERRUPT_H_ */
