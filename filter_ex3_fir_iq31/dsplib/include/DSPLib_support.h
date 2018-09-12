/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#ifndef __DSPLIB_SUPPORT_H__
#define __DSPLIB_SUPPORT_H__

//******************************************************************************
//
//! \addtogroup dsplib_support Support
//! @{
//!
//! Definitions and abstraction for MSP devices.
//!
//! \ingroup dsplib_support
//! \defgroup dsplib_support_device Common
//!
//! This file provides common definitions, useful conversion macros and 
//! abstracted functions to support the core DSPLib functions. While not all
//! macros and functions in this file are useful outside of DSPLib the main
//! application can use any of them as needed.
//!
//! @}
//
//******************************************************************************

//******************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//******************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Offset used to store table size.
//
//******************************************************************************
#define DSPLIB_TABLE_OFFSET     2

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Increment size for complex data.
//
//******************************************************************************
#define CMPLX_INCREMENT         (2)

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Access the real portion of complex data.
//
//******************************************************************************
#define CMPLX_REAL(ptr)         ((ptr)[0])

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Access the imaginary portion of complex data.
//
//******************************************************************************
#define CMPLX_IMAG(ptr)         ((ptr)[1])

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Convert number to Q15 fixed point.
//
//******************************************************************************
#ifndef __QMATHLIB_H__ // Define _Q15 if not defined by QmathLib header.
#define _Q15(A)                 ((_q15)((A) * ((uint32_t)1 << 15)))
#endif  //__QMATHLIB_H__

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Convert number to IQ31 fixed point.
//
//******************************************************************************
#define _IQ31(A)                ((_iq31)((A) * ((uint32_t)1 << 31)))

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Saturate input to minimum or maximum value and return value.
//
//******************************************************************************
#define __saturate(x, min, max) (((x)>(max))?(max):(((x)<(min))?(min):(x)))

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Place data with alignment, when LEASC is used data is placed into the
//!        shared RAM section.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__)
#define _PRAGMA(x) _Pragma (#x)
#if defined(MSP_USE_LEASC)
#define DSPLIB_DATA(var,align)  _PRAGMA(DATA_SECTION(var,".leaRAM"))\
                                _PRAGMA(DATA_ALIGN(var,(align)))
#else
#define DSPLIB_DATA(var,align)  _PRAGMA(DATA_ALIGN(var,(align)))
#endif
#elif defined(__IAR_SYSTEMS_ICC__)
#define _PRAGMA(x) _Pragma (#x)
#if defined(MSP_USE_LEASC)
#define DSPLIB_DATA(var,align)  _PRAGMA(location="LEARAM")\
                                _PRAGMA(data_alignment=align)
#else
#define DSPLIB_DATA(var,align)  _PRAGMA(data_alignment=align)
#endif
#elif defined(__GNUC__)
#if defined(MSP_USE_LEASC)
#define DSPLIB_DATA(var,align)  __attribute__((section(".leaRAM")))\
                                __attribute__((aligned(align)))
#else
#define DSPLIB_DATA(var,align)  __attribute__((aligned(align)))
#endif
#else
#define DSPLIB_DATA(var,align)
#endif

// Benchmark register support
#define HWREG16(x)          (*((volatile uint16_t *)((uint16_t)x)))
#define BENCHMARK_TAxR      (0x0010)
#define BENCHMARK_TAxEX0    (0x0020)

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Start benchmark using Timer_A with SMCLK as the clock source.
//!
//! \param baseAddress Base address of Timer_A peripheral to use.
//! \param resolution Resolution of the benchmark, valid values are:
//!                   - 1 (16-bit result)
//!                   - 2 (17-bit result)
//!                   - 4 (18-bit result)
//!                   - 8 (19-bit result)
//!                   - 16 (20-bit result)
//!                   - 32 (21-bit result)
//!                   - 64 (22-bit result)
//!
//! \return None
//
//******************************************************************************
static inline void msp_benchmarkStart(uint16_t baseAddress, uint8_t res)
{
    static const uint8_t TAx_ID[8] = {
        0x00, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0xC0
    };
    HWREG16(baseAddress) = 0x0004;
    HWREG16(baseAddress+BENCHMARK_TAxEX0) = res < 7 ? 0 : ((0x07&(res >> 3))-1);
    HWREG16(baseAddress) = 0x0220 + TAx_ID[res > 7 ? 7 : ((res-1)&7)];
}

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Stop timer A benchmark and return cycle counts.
//!
//! \param baseAddress Base address of Timer_A peripheral to use.
//!
//! \return Benchmark cycle counts.
//
//******************************************************************************
static inline uint32_t msp_benchmarkStop(uint16_t baseAddress)
{
    static const uint8_t TAx_IDEX_SCALE[8] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
    };
    uint16_t cycles = HWREG16(baseAddress+BENCHMARK_TAxR);
    uint16_t shift = (HWREG16(baseAddress) & 0x00C0) >> 6;
    uint16_t scale = TAx_IDEX_SCALE[HWREG16(baseAddress+BENCHMARK_TAxEX0) & 7];
    HWREG16(baseAddress) = 0;
    return (scale*((uint32_t)cycles << shift));
}

#if defined(__MSP430_HAS_MPY32__)

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Real Q15 multiply with result returned.
//
//******************************************************************************
static inline int16_t __q15mpy(int16_t a, int16_t b) 
{
    MPYS = a;
    OP2  = b;
    return RESHI;
}

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Real Q31 multiply with result returned.
//
//******************************************************************************
static inline int32_t __q31mpy(int32_t a, int32_t b) 
{
    uint32_t result;
    MPYS32L = a & 0xFFFF;
    MPYS32H = a >> 16;
    OP2L    = b & 0xFFFF;
    OP2H    = b >> 16;
    result  = RES2;
    result |= ((uint32_t)RES3 << 16);
    return (int32_t)result;
}

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Complex Q15 multiply with result stored back to a.
//
//******************************************************************************
static inline void __q15cmpy(int16_t *aR,
                             int16_t *aI,
                             const int16_t *bR,
                             const int16_t *bI)
{
    MPYS = *aI;
    OP2  = *bR;
    MACS = *aR;
    OP2  = *bI;
    MPYS = -*aI;
    *aI  = RESHI;
    OP2  = *bI;
    MACS = *aR;
    OP2  = *bR;
    *aR  = RESHI;
}

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Complex Q15 by IQ31 multiply with result stored back to a.
//
//******************************************************************************
static inline void __q15iq31cmpy(int32_t *aR,
                                 int32_t *aI,
                                 const int32_t *bR,
                                 const int32_t *bI)
{
    MPYS = *aI;
    OP2L = (uint16_t)*bR;
    OP2H = (uint16_t)(*bR >> 16);
    MACS = *aR;
    OP2L = (uint16_t)*bI;
    OP2H = (uint16_t)(*bI >> 16);
    MPYS = -*aI;
    *aI  = RES1;
    *aI |= ((uint32_t)RES2 << 16);
    OP2L = (uint16_t)*bI;
    OP2H = (uint16_t)(*bI >> 16);
    MACS = *aR;
    OP2L = (uint16_t)*bR;
    OP2H = (uint16_t)(*bR >> 16);
    *aR  = RES1;
    *aR |= ((uint32_t)RES2 << 16);
}

#else //__MSP430_HAS_MPY32__

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Real Q15 multiply with result returned.
//
//******************************************************************************
#define __q15mpy(A, B)  ((int16_t)(((int32_t)(A) * (int32_t)(B)) >> 15))

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Real IQ31 multiply with result returned.
//
//******************************************************************************
#define __q31mpy(A, B)  ((int32_t)(((int64_t)(A) * (int64_t)(B)) >> 31))

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Real Q15 by IQ31 multiply with result returned
//
//******************************************************************************
#define __q15iq31mpy(A, B)  ((int32_t)(((int64_t)(A) * (int64_t)(B)) >> 15))

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Complex Q15 multiply with result stored back to A.
//
//******************************************************************************
static inline void __q15cmpy(int16_t *aR,
                             int16_t *aI,
                             const int16_t *bR,
                             const int16_t *bI)
{
    int16_t resR =  __q15mpy(*aR, *bR) - __q15mpy(*aI, *bI);
    int16_t resI =  __q15mpy(*aR, *bI) + __q15mpy(*aI, *bR);
    *aR = resR;
    *aI = resI;
}

//******************************************************************************
//
//! \ingroup dsplib_support_device
//!
//! \brief Complex IQ31 by Q15 multiply with result stored back to a.
//
//******************************************************************************
static inline void __q15iq31cmpy(int32_t *aR,
                                 int32_t *aI,
                                 const int32_t *bR,
                                 const int32_t *bI)
{
    int32_t resR =  __q15iq31mpy(*aR, *bR) - __q15iq31mpy(*aI, *bI);
    int32_t resI =  __q15iq31mpy(*aR, *bI) + __q15iq31mpy(*aI, *bR);
    *aR = resR;
    *aI = resI;
}

#endif //__MSP430_HAS_MPY32__

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //__DSPLIB_SUPPORT_H__
