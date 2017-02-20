/*
 * msp_mac_int.c
 *
 *  Created on: 2016-6-17
 *      Author: zhjl769
 */

#include "msp430.h"

#include <stdint.h>
#include <stdbool.h>

#include "../../include/DSPLib.h"

/*
 * Perform element wise int multiplication and accumulate (dot product) of two source vectors.
 */
msp_status msp_mac_int(const msp_mac_int_params *params, int *srcA, int *srcB, int *result)
{
    uint16_t length;

    /* Initialize the loop counter with the vector length. */
    length = params->length;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that length parameter is a multiple of two. */
    if (length & 1) {
        return MSP_SIZE_ERROR;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

#if defined(MSP_USE_LEASC)
    msp_status status;
    uint16_t interruptState;
    MSP_LEASC_MAC_PARAMS *leascParams;

    /* Save interrupt state and disable interrupts. */
    interruptState = __get_interrupt_state();
    __disable_interrupt();

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check that the data arrays are aligned and in a valid memory segment. */
    if (!(MSP_LEASC_VALID_ADDRESS(srcA, 4) &
          MSP_LEASC_VALID_ADDRESS(srcB, 4) &
          MSP_LEASC_VALID_ADDRESS(result, 4))) {
        __set_interrupt_state(interruptState);
        return MSP_LEASC_INVALID_ADDRESS;
    }

    /* Return MSP_LEASC_BUSY error if LEA is busy. */
    if (LEASCCNF1 & LEASCBUSY) {
        __set_interrupt_state(interruptState);
        return MSP_LEASC_BUSY;
    }
#endif //DSPLIB_DISABLE_DIAGNOSTICS

    /* Initialize LEASC if it is not enabled. */
    if (!(LEASCPMCTL & LEASCCMDEN)) {
        msp_leasc_init();
    }

    /* Allocate MSP_LEASC_MAC_PARAMS structure on LEASC stack. */
    LEASCCNF2 -= sizeof(MSP_LEASC_MAC_PARAMS)/sizeof(uint32_t);
    leascParams = (MSP_LEASC_MAC_PARAMS *)MSP_LEASC_REVERT_ADDRESS(LEASCCNF2);

    /* Set MSP_LEASC_MAC_PARAMS structure. */
    leascParams->input2 = MSP_LEASC_CONVERT_ADDRESS(srcB);
    leascParams->output = MSP_LEASC_CONVERT_ADDRESS(result);
    leascParams->blkSize = length;

    /* Load source arguments to LEA. */
    LEASCPMS0 = MSP_LEASC_CONVERT_ADDRESS(srcA);
    LEASCPMS1 = MSP_LEASC_CONVERT_ADDRESS(leascParams);

    /* Invoke the LEASCCMD__MAC command with interrupts enabled. */
    LEASCPMCB = LEASCCMD__MAC | LEASCITFLG1;

    /* Clear DSPLib flags, restore interrupts and enter LPM0. */
    msp_leasc_flags = 0;
    __bis_SR_register(GIE+LPM0_bits);

    /* Free MSP_LEASC_MAC_PARAMS from LEASC stack. */
    LEASCCNF2 += sizeof(MSP_LEASC_MAC_PARAMS)/sizeof(uint32_t);

    /* Set status flag. */
    status = MSP_SUCCESS;

#ifndef DSPLIB_DISABLE_DIAGNOSTICS
    /* Check LEASC interrupt flags for any errors. */
    if (msp_leasc_flags & LEASCCOVLIFG) {
        status = MSP_LEASC_COMMAND_OVERFLOW;
    }
    else if (msp_leasc_flags & LEASCOORIFG) {
        status = MSP_LEASC_OUT_OF_RANGE;
    }
    else if (msp_leasc_flags & LEASCSDIIFG) {
        status = MSP_LEASC_SCALAR_INCONSISTENCY;
    }
#endif

    /* Restore interrupt state and return status. */
    __set_interrupt_state(interruptState);

    return status;
#else //MSP_USE_LEASC
#ifdef __MSP430_HAS_MPY32__
    uint16_t *resultPtr = (uint16_t *)result;

    /* If MPY32 is available save control context and set to fractional mode. */
    uint16_t ui16MPYState = MPY32CTL0;
    MPY32CTL0 = MPYDLYWRTEN | MPYSAT;

    /* Reset multiplier context. */
    MPY32CTL0 &= ~MPYC;
    RESHI = 0; RESLO = 0;

    /* Loop through all vector elements. */
    while (length--) {
        /* Multiply and accumulate srcA and srcB. */
        MACS = *srcA++;
        OP2  = *srcB++;
    }

    /* Store result. */
    *resultPtr++ = RESLO;
//    *resultPtr++ = RESHI;

    /* Restore MPY32 control context. */
    MPY32CTL0 = ui16MPYState;
#else //__MSP430_HAS_MPY32__
    /* Initialize the result. */
    *result = 0;

    /* Loop through all vector elements. */
    while (length--) {
        /* Multiply srcA and srcB and accumulate to the result. */
        *result += (int32_t)*srcA++ * (int32_t)*srcB++;
    }

    /* Scale result by 2. */
    *result <<= 1;
#endif //__MSP430_HAS_MPY32__

    return MSP_SUCCESS;
#endif //MSP_USE_LEASC
}



