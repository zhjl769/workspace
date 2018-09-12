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
#ifndef __DSPLIB_FILTER_H__
#define __DSPLIB_FILTER_H__

//******************************************************************************
//
//! \addtogroup dsplib_filter_api Filter
//! @{
//!
//! Functions for common digital filters such as Finite Impulse Response (FIR)
//! and Infinite Impulse Response (IIR). The filter functions support real and
//! complex inputs as well as 16-bit and 32-bit data types.
//!
//! \ingroup dsplib_filter_api
//! \defgroup dsplib_filter_real_fir Real FIR
//!
//! Functions for performing real Finite Impulse Response (FIR) filters.
//!
//! \ingroup dsplib_filter_api
//! \defgroup dsplib_filter_complex_fir Complex FIR
//!
//! Functions for performing complex Finite Impulse Response (FIR) filters.
//!
//! \ingroup dsplib_filter_api
//! \defgroup dsplib_filter_real_iir Real IIR
//!
//! Functions for performing real Infinite Impulse Response (IIR) filters.
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

//******************************************************************************
//
//! \ingroup dsplib_filter_real_fir
//!
//! \brief Parameter structure for FIR filter.
//
//******************************************************************************
typedef struct msp_fir_q15_params {
    //! \details
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! \details
    //! Length of FIR coefficients, must be a multiple of two.
    uint16_t tapLength;
    //! \details
    //! Pointer to real FIR filter coefficients.
    const _q15 *coeffs;
} msp_fir_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_filter_real_fir
//!
//! \brief Parameter structure for FIR filter.
//
//******************************************************************************
typedef struct msp_fir_iq31_params {
    //! \details
    //! Length of the output, must be a multiple of two.
    uint16_t length;
    //! \details
    //! Length of FIR coefficients, must be a multiple of two.
    uint16_t tapLength;
    //! \details
    //! Pointer to real FIR filter coefficients.
    const _iq31 *coeffs;
} msp_fir_iq31_params;

//******************************************************************************
//
//! \ingroup dsplib_filter_complex_fir
//!
//! \brief Parameter structure for complex FIR filter.
//
//******************************************************************************
typedef struct msp_cmplx_fir_q15_params{
    //! \details
    //! Length of the output, must be a multiple of two.
    uint16_t length;
    //! \details
    //! Length of FIR coefficients, must be a multiple of two.
    uint16_t tapLength;
    //! \details
    //! Pointer to complex FIR filter coefficients.
    const _q15 *coeffs;
} msp_cmplx_fir_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_filter_complex_fir
//!
//! \brief Parameter structure for complex FIR filter.
//
//******************************************************************************
typedef struct msp_cmplx_fir_iq31_params{
    //! \details
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! \details
    //! Length of FIR coefficients, must be a multiple of two.
    uint16_t tapLength;
    //! \details
    //! Pointer to complex FIR filter coefficients.
    const _iq31 *coeffs;
} msp_cmplx_fir_iq31_params;

//******************************************************************************
//
//! \ingroup dsplib_filter_real_iir
//!
//! \brief Parameter structure for Direct Form 1 biquad filter.
//
//******************************************************************************
typedef struct msp_biquad_df1_q15_params {
    //! \details
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! \details
    //! Pointer to filter coefficients of length 5 in format b2, b1, b0, a2, a1.
    const _q15 *coeffs;
    //! \details
    //! Pointer to an array of length four used to store the state of the biquad
    //! operation. When continuous operation is desired the previous state
    //! needs to be passed to the next biquad operation. This data block must be
    //! allocated in shared RAM when using LEASC.
    _q15 *stateBuffer;
} msp_biquad_df1_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_filter_real_iir
//!
//! \brief Parameter structure for Direct Form 2 biquad filter.
//
//******************************************************************************
typedef struct msp_biquad_df2_q15_params {
    //! \details
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! \details
    //! Pointer to filter coefficient vector of length 7 in format a0, a1, a2,
    //! a3, b1, b2, b3.
    const _q15 *coeffs;
    //! \details
    //! Maximum of the output vector.
    _q15 sumMax;
    //! \details
    //! Minimum of the output vector.
    _q15 sumMin;
    //! \details
    //! DC bias added to input of the biquad operation.
    _q15 bias;
    //! \details
    //! State of the biquad operation. Initialize to zero and pass the previous
    //! state to the next biquad operation when continuous operation is desired.
    _q15 Q1;
    //! \details
    //! State of the biquad operation. Initialize to zero and pass the previous
    //! state to the next biquad operation when continuous operation is desired.
    _q15 Q2;
} msp_biquad_df2_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_filter_real_fir
//!
//! \brief Discrete-time convolution of a source vector with real coefficients
//!        to apply an FIR filter.
//!
//! \par Details
//! FIR filter implementation utilizing discrete-time convolution. Q15
//! multiplication is performed with a 32-bit accumulator to avoid intermediate
//! overflow.
//!
//! \par Pseudo code
//! `dst = conv([stateBuffer, src], taps)(0:1:length)`
//!
//! \param params Pointer to the FIR parameter structure.
//! \param src Pointer to the source vector to filter.
//! \param dst Pointer to the destination vector.
//! \return Status of the operation.
//
//******************************************************************************
extern msp_status msp_fir_q15(const msp_fir_q15_params *params,
                              const _q15 *src,
                              _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_filter_real_fir
//!
//! \brief Discrete-time convolution of a source vector with real coefficients
//!        to apply an FIR filter.
//!
//! \par Details
//! FIR filter implementation utilizing discrete-time convolution. IQ31
//! multiplication is performed with a 32-bit accumulator to avoid intermediate
//! overflow.
//!
//! \par Pseudo code
//! `dst = conv([stateBuffer, src], taps)(0:1:length)`
//!
//! \param params Pointer to the FIR parameter structure.
//! \param src Pointer to the source vector to filter.
//! \param dst Pointer to the destination vector.
//! \return Status of the operation.
//
//******************************************************************************
extern msp_status msp_fir_iq31(const msp_fir_iq31_params *params,
                               const _iq31 *src,
                               _iq31 *dst);

//******************************************************************************
//
//! \ingroup dsplib_filter_complex_fir
//!
//! \brief Discrete-time convolution of a complex source vector with complex
//!        coefficients to apply an FIR filter.
//!
//! \par Details
//! FIR filter implementation utilizing discrete-time convolution. Complex Q15
//! multiplication is performed with a 32-bit accumulator to avoid intermediate
//! overflow.
//!
//! \par Pseudo code
//! `dst = conv([stateBuffer, src], taps)(0:1:length)`
//!
//! \param params Pointer to the complex FIR parameter structure.
//! \param src Pointer to the source vector to filter.
//! \param dst Pointer to the destination vector.
//! \return Status of the operation.
//
//******************************************************************************
extern msp_status msp_cmplx_fir_q15(const msp_cmplx_fir_q15_params *params,
                                    const _q15 *src,
                                    _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_filter_complex_fir
//!
//! \brief Discrete-time convolution of a complex source vector with complex
//!        coefficients to apply an FIR filter.
//!
//! \par Details
//! FIR filter implementation utilizing discrete-time convolution. Complex IQ31
//! multiplication is performed with a 32-bit accumulator to avoid intermediate
//! overflow.
//!
//! \par Pseudo code
//! `dst = conv([stateBuffer, src], taps)(0:1:length)`
//!
//! \param params Pointer to the complex FIR parameter structure.
//! \param src Pointer to the source vector to filter.
//! \param dst Pointer to the destination vector.
//! \return Status of the operation.
//
//******************************************************************************
extern msp_status msp_cmplx_fir_iq31(const msp_cmplx_fir_iq31_params *params,
                                     const _iq31 *src,
                                     _iq31 *dst);

//******************************************************************************
//
//! \ingroup dsplib_filter_real_iir
//!
//! \brief Single order direct form 1 biquad filter.
//!
//! \par Details
//! Single order biquad filter with direct form 1 implementation. Q15
//! multiplication is performed with a 32-bit accumulator to avoid intermediate
//! overflow with saturation to Q15 when storing the result.
//!
//! ![Biquad DF1 structure](images/biquad_df1.png)
//!
//! \par Transfer Function
//! \f$H(z)=\frac{b_0+b_1z^{-1}+b_2z^{-2}}{a_0+a_1z^{-1}+a_2z^{-2}}\f$
//!
//! \par Pseudo code
//! `y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] -  a1 * y[n-1] - a2 * y[n-2]`
//!
//! \param params Pointer to the biquad direct form 1 parameter structure.
//! \param src Pointer to the source data to filter.
//! \param dst Pointer to the destination vector.
//! \return Status of the operation.
//
//******************************************************************************
extern msp_status msp_biquad_df1_q15(const msp_biquad_df1_q15_params *params,
                                     const _q15 *src,
                                     _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_filter_real_iir
//!
//! \brief Single order direct form 2 biquad filter.
//!
//! \par Details
//! Single order direct form 2 biquad filter with bias and minimum and maximum
//! result calculation. Q15 multiplication is performed with a 32-bit
//! accumulator to avoid intermediate overflow with saturation to Q15 when
//! storing the result. The maximum and minimum of the destination vector are
//! returned for post processing.
//!
//! ![Biquad DF2 structure](images/biquad_df2.png)
//!
//! \par Transfer Function
//! \f$H(z)=\frac{b_0+b_1z^{-1}+b_2z^{-2}}{a_0+a_1z^{-1}+a_2z^{-1}+a_3z^{-2}}\f$
//!
//! \par Pseudo code
//! `y[n] = b0 * w[n] + b1 * w[n-1] + b2 * w[n-2]` \n
//! `w[n] = x[n] - a1 * x[n-1] - a2 * x[n-1] - a3 * x[n-2]`
//!
//! \param params Pointer to the biquad direct form 2 parameter structure.
//! \param src Pointer to the source data to filter.
//! \param dst Pointer to the destination vector.
//! \return Status of the operation.
//
//******************************************************************************
extern msp_status msp_biquad_df2_q15(msp_biquad_df2_q15_params *params,
                                     const _q15 *src,
                                     _q15 *dst);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //__DSPLIB_FILTER_H__
