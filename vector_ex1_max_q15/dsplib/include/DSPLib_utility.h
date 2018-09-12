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
#ifndef __DSPLIB_UTILITY_H__
#define __DSPLIB_UTILITY_H__

//******************************************************************************
//
//! \addtogroup dsplib_utility_api Utility
//! @{
//!
//! This file provides utility functions for operations such as data copy,
//! array fill and deinterleaving of channel data.
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
//! \ingroup dsplib_utility_api
//!
//! \brief Parameters for Q15 vector copy function.
//
//******************************************************************************
typedef struct msp_copy_q15_params {
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
} msp_copy_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Parameters for IQ31 vector copy function.
//
//******************************************************************************
typedef struct msp_copy_iq31_params {
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
} msp_copy_iq31_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//!  \brief Parameters for Q15 real vector fill
//
//******************************************************************************
typedef struct msp_fill_q15_params {
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! Scalar constant to fill the destination vector with.
    _q15 value;
} msp_fill_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//!  \brief Parameters for IQ31 real vector fill
//
//******************************************************************************
typedef struct msp_fill_iq31_params {
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! Scalar constant to fill the destination vector with.
    _iq31 value;
} msp_fill_iq31_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//!  \brief Parameters for Q15 complex vector fill
//
//******************************************************************************
typedef struct msp_cmplx_fill_q15_params {
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! Real constant value to fill real indices with.
    _q15 realValue;
    //! Imaginary constant value to fill imaginary indices with.
    _q15 imagValue;
} msp_cmplx_fill_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//!  \brief Parameters for IQ31 complex vector fill
//
//******************************************************************************
typedef struct msp_cmplx_fill_iq31_params {
    //! Length of the source data, must be a multiple of two.
    uint16_t length;
    //! Real constant value to fill real indices with.
    _iq31 realValue;
    //! Imaginary constant value to fill imaginary indices with.
    _iq31 imagValue;
} msp_cmplx_fill_iq31_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//!  \brief Parameters for Q15 vector deinterleave
//
//******************************************************************************
typedef struct msp_deinterleave_q15_params {
    //! Length of each channel, must be a multiple of two.
    uint16_t length;
    //! Specific zero-indexed channel to pull from source.
    uint16_t channel;
    //! Number of channels being separated.
    uint16_t numChannels;
} msp_deinterleave_q15_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//!  \brief Parameters for IQ31 vector deinterleave
//
//******************************************************************************
typedef struct msp_deinterleave_iq31_params {
    //! Length of each channel, must be a multiple of two.
    uint16_t length;
    //! Specific zero-indexed channel to pull from source.
    uint16_t channel;
    //! Number of channels being separated.
    uint16_t numChannels;
} msp_deinterleave_iq31_params;

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Real Q15 vector copy.
//!
//! \par Details
//! Copy of real Q15 source vector to destination vector.
//!
//! \param params Pointer to the vector copy parameter structure.
//! \param src Pointer to the source data to copy.
//! \param dst Pointer to the destination vector.
//! \return none.
//
//******************************************************************************
extern msp_status msp_copy_q15(const msp_copy_q15_params *params,
                               const _q15 *src,
                               _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Real IQ31 vector copy.
//!
//! \par Details
//! Copy of real IQ31 source vector to destination vector.
//!
//! \param params Pointer to the vector copy parameter structure.
//! \param src Pointer to the source data to copy.
//! \param dst Pointer to the destination vector.
//! \return none.
//
//******************************************************************************
extern msp_status msp_copy_iq31(const msp_copy_iq31_params *params,
                                const _iq31 *src,
                                _iq31 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Real Q15 vector fill with constant.
//!
//! \par Details
//! Fill of real Q15 destination vector with constant.
//!
//! \param params Pointer to the vector copy parameter structure.
//! \param dst Pointer to the destination vector.
//! \return none.
//
//******************************************************************************
extern msp_status msp_fill_q15(const msp_fill_q15_params *params, _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Real IQ31 vector fill with constant.
//!
//! \par Details
//! Fill of real IQ31 destination vector with constant.
//!
//! \param params Pointer to the vector copy parameter structure.
//! \param dst Pointer to the destination vector.
//! \return none.
//
//******************************************************************************
extern msp_status msp_fill_iq31(const msp_fill_iq31_params *params, _iq31 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Complex Q15 vector fill with constant.
//!
//! \par Details
//! Fill of complex Q15 destination vector with constant real and imaginary
//! components.
//!
//! \param params Pointer to the complex fill parameter structure.
//! \param dst Pointer to the destination vector.
//! \return none.
//
//******************************************************************************
extern msp_status msp_cmplx_fill_q15(const msp_cmplx_fill_q15_params *params,
                                     _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Complex IQ31 vector fill with constant.
//!
//! \par Details
//! Fill of complex IQ31 destination vector with constant real and imaginary
//! components.
//!
//! \param params Pointer to the complex fill parameter structure.
//! \param dst Pointer to the destination vector.
//! \return none.
//
//******************************************************************************
extern msp_status msp_cmplx_fill_iq31(const msp_cmplx_fill_iq31_params *params,
                                      _iq31 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Extract a single channel from multiple-channel source.
//!
//! \par Details
//! A single Q15 vector is extracted from multiple interleaved vectors. For
//! example, a deinterleave operation to extract channel one from a four channel
//! source vector x[] with channel length 4 will extract the following data:
//!
//!     x[1], x[5], x[9], x[13]
//!
//! \param params Pointer to the deinterleave parameter structure.
//! \param src Pointer to the source data to filter.
//! \return none.
//
//******************************************************************************
extern msp_status msp_deinterleave_q15(
                                    const msp_deinterleave_q15_params *params,
                                    _q15 *src,
                                    _q15 *dst);

//******************************************************************************
//
//! \ingroup dsplib_utility_api
//!
//! \brief Extract a single channel from multiple-channel source.
//!
//! \par Details
//! A single IQ31 vector is extracted from multiple interleaved vectors. For
//! example, a deinterleave operation to extract channel one from a four channel
//! source vector x[] with channel length 4 will extract the following data:
//!
//!     x[1], x[5], x[9], x[13]
//!
//! \param params Pointer to the deinterleave parameter structure.
//! \param src Pointer to the source data to filter.
//! \return none.
//
//******************************************************************************
extern msp_status msp_deinterleave_iq31(
                                    const msp_deinterleave_iq31_params *params,
                                    _iq31 *src,
                                    _iq31 *dst);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //__DSPLIB_UTILITY_H__
