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
#ifndef __DSPLIB_LEASC_H__
#define __DSPLIB_LEASC_H__

#include <stdint.h>
#include "DSPLib_types.h"

//******************************************************************************
//
//! \addtogroup dsplib_support Support
//! @{
//!
//! \ingroup dsplib_support
//! \defgroup dsplib_support_leasc LEASC
//!
//! Definitions required to support the use of LEASC.
//!
//! @}
//
//******************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__MSP430_HAS_LEA_SC__) && !defined(MSP_DISABLE_LEASC)

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Enable the LEASC accelerator for DSPLib functions. The default is
//! to use LEASC when available, to disable LEASC define MSP_DISABLE_LEASC in
//! the compiler settings.
//!
//! It is always recommended to use LEASC for lowest power and best performance.
//! Disabling LEASC is only intended to be used for benchmarking LEASC
//! performance against the CPU and hardware multiplier or by users who would
//! like to dedicate the LEASC peripheral to a specific function and run the
//! remainder on the CPU.
//
//******************************************************************************
#define MSP_USE_LEASC                   1

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Convert MSP430 address to internal LEASC address mapping.
//
//******************************************************************************
#define MSP_LEASC_CONVERT_ADDRESS(x)    ((uint16_t)(((uintptr_t)(x) >> 2) & 0xffff))

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Convert LEASC address to MSP430 address mapping.
//
//******************************************************************************
#define MSP_LEASC_REVERT_ADDRESS(x)     ((uintptr_t)(((uint32_t)(x) << 2)))

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Check that the address is aligned correctly.
//
//******************************************************************************
#define MSP_LEASC_RANGE(x)              (((uintptr_t)(x) >= LEASCMB) && ((uintptr_t)(x) < LEASCMT))

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Check that the address is within the LEASC RAM.
//
//******************************************************************************
#define MSP_LEASC_ALIGNED(x,n)          (!((uint32_t)(x) & ((n)-1)))

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Check that the address aligment is valid and within the LEASC RAM.
//
//******************************************************************************
#define MSP_LEASC_VALID_ADDRESS(x,n)    (MSP_LEASC_RANGE(x) & MSP_LEASC_ALIGNED(x,n))

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief LEASC constant memory address for 0x0000 coefficients.
//
//******************************************************************************
#define MSP_LEASC_CONST_ZERO            0xA000

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief LEASC constant memory address for 0x7fff coefficients.
//
//******************************************************************************
#define MSP_LEASC_CONST_ONE             0xAC00

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief LEASC constant memory address for 0x8000 coefficients.
//
//******************************************************************************
#define MSP_LEASC_Q15_CONST_NEG_ONE     0xB000

 //******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief LEASC constant memory address for 0x80000000 coefficients.
//
//******************************************************************************
#define MSP_LEASC_IQ31_CONST_NEG_ONE    0xE000

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__ADDMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_ADDMATRIX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCMD__ADDLONGMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_ADDMATRIXLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__SUBMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_SUBMATRIX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__SUBMATRIXLONG command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_SUBMATRIXLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MPYMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_MPYMATRIX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MPYLONGMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_MPYLONGMATRIX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MAC command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t reserved;
} MSP_LEASC_MAC_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MACLONGMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t reserved;
} MSP_LEASC_MACLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__POLYNOMIAL command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t output;
    uint16_t rCoeff;
    uint16_t m;
    int32_t scaleFactor;
} MSP_LEASC_POLY_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MAX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
} MSP_LEASC_MAX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MAXLONGMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
    uint16_t offset;
    uint16_t reserved;
} MSP_LEASC_MAXLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MAXUNSIGNED command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
} MSP_LEASC_MAXUNSIGNED_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MAXUNSIGNEDMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
    uint16_t inputOffset;
    uint16_t reserved;
} MSP_LEASC_MAXLONGUNSIGNED_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MIN command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
} MSP_LEASC_MIN_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MINLONGMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
    uint16_t inputOffset;
    uint16_t reserved;
} MSP_LEASC_MINLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MINUNSIGNEDMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
    uint16_t inputOffset;
    uint16_t reserved;
} MSP_LEASC_MINUNSIGNED_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MINLONGUNSIGNED command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t output;
    uint16_t inputOffset;
    uint16_t reserved;
} MSP_LEASC_MINLONGUNSIGNED_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MPYCOMPLEXMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t outputOffset;
} MSP_LEASC_MPYCOMPLEXMATRIX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MACCOMPLEXMATRIX command.
//
//******************************************************************************
typedef struct {
    uint16_t blkSize;
    uint16_t input2;
    uint16_t output;
    uint16_t input1Offset;
    uint16_t input2Offset;
    uint16_t reserved;
} MSP_LEASC_MACCOMPLEXMATRIX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__MPYMATRIXROW command.
//
//******************************************************************************
typedef struct {
    uint16_t rowSize;
    uint16_t colSize;
    uint16_t colVector;
    uint16_t output;
} MSP_LEASC_MPYMATRIXROW_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FIR command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t coeffs;
    uint16_t output;
    uint16_t tapLength;
    uint16_t bufferMask;
    uint16_t reserved;
} MSP_LEASC_FIR_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FIRLONG command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t coeffs;
    uint16_t output;
    uint16_t tapLength;
    uint16_t bufferMask;
    uint16_t reserved;
} MSP_LEASC_FIRLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FIRCOMPLEX command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t coeffs;
    uint16_t output;
    uint16_t tapLength;
    uint16_t bufferMask;
    uint16_t reserved;
} MSP_LEASC_FIRCOMPLEX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FIRLONGCOMPLEX command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t coeffs;
    uint16_t output;
    uint16_t tapLength;
    uint16_t bufferMask;
    uint16_t reserved;
} MSP_LEASC_FIRLONGCOMPLEX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__IIR command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t output;
    uint16_t state;
    uint16_t coeffs;
    uint16_t direction;
    uint16_t reserved;
} MSP_LEASC_IIR_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__BIQUADWITHBIASANDMINMAX command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t output;
    _q15 Q1;
    _q15 Q2;
    _q15 bias;
    _q15 a0;
    _q15 a1;
    _q15 a2;
    _q15 a3;
    _q15 b0;
    _q15 b1;
    _q15 b2;
    _q15 reserved1;
    _q15 sumMin;
    _q15 reserved2;
    _q15 sumMax;
} MSP_LEASC_IIR_DF2_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__GOERTZEL command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t output;
    uint16_t coeffCos;
    uint16_t coeffSin;
} MSP_LEASC_GOERTZEL_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__BITREVERSECOMPLEXEVEN or LEASCCMD__BITREVERSECOMPLEXODD commands.
//
//******************************************************************************
typedef struct {
    uint16_t sqrtVectorSize;
    uint16_t reserved;
} MSP_LEASC_BITREVERSECOMPLEX_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__BITREVERSECOMPLEXLONGEVEN or LEASCCMD__BITREVERSECOMPLEXLONGODD commands.
//
//******************************************************************************
typedef struct {
    uint16_t sqrtVectorSize;
    uint16_t reserved;
} MSP_LEASC_BITREVERSECOMPLEXLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FFTCOMPLEXAUTOSCALING command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSizeBy2;
    uint16_t log2Size;
} MSP_LEASC_FFTCOMPLEXAUTOSCALING_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FFTCOMPLEXFIXEDSCALING command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSizeBy2;
    uint16_t log2Size;
} MSP_LEASC_FFTCOMPLEXFIXEDSCALING_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FFTCOMPLEXLONG command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t log2Size;
} MSP_LEASC_FFTCOMPLEXLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FFT command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t log2Size;
} MSP_LEASC_FFT_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__FFTLONG command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t log2Size;
} MSP_LEASC_FFTLONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__DEINTERLEAVEEVENEVEN,
//! LEASCCMD__DEINTERLEAVEEVENODD, LEASCCMD__DEINTERLEAVEODDEVEN and
//! LEASCCMD__DEINTERLEAVEODDODD commands.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t interleaveDepth;
    uint16_t output;
    uint16_t reserved;
} MSP_LEASC_DEINTERLEAVE_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Parameter structure for LEASCCMD__DEINTERLEAVELONG command.
//
//******************************************************************************
typedef struct {
    uint16_t vectorSize;
    uint16_t interleaveDepth;
    uint16_t output;
    uint16_t reserved;
} MSP_LEASC_DEINTERLEAVELONG_PARAMS;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief DSPLib interrupt flags.
//
//******************************************************************************
extern volatile uint32_t msp_leasc_flags;

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief DSPLib initialization routine for LEASC.
//
//******************************************************************************
extern void msp_leasc_init(void);

//******************************************************************************
//
//! \ingroup dsplib_support_leasc
//!
//! \brief Check that the address aliment is valid and within the LEASC RAM.
//
//******************************************************************************
extern __interrupt void msp_leasc_isr(void);

#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __DSPLIB_LEASC_H__
