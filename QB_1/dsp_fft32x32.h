/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  DSPLIB  DSP Signal Processing Library                                   */
/*                                                                          */
/*      Release:        Revision 1.00a                                      */
/*      CVS Revision:   1.2     Tue Jan 24 13:06:38 2006 (UTC)              */
/*      Snapshot date:  03-Mar-2006                                         */
/*                                                                          */
/*  This library contains proprietary intellectual property of Texas        */
/*  Instruments, Inc.  The library and its source code are protected by     */
/*  various copyrights, and portions may also be protected by patents or    */
/*  other legal protections.                                                */
/*                                                                          */
/*  This software is licensed for use with Texas Instruments TMS320         */
/*  family DSPs.  This license was provided to you prior to installing      */
/*  the software.  You may review this license by consulting the file       */
/*  TI_license.PDF which accompanies the files in this library.             */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2006 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
/* ======================================================================== */
/*  Assembler compatibility shim for assembling 4.30 and later code on      */
/*  tools prior to 4.30.                                                    */
/* ======================================================================== */
/* ======================================================================== */
/*  End of assembler compatibility shim.                                    */
/* ======================================================================== */
/* ======================================================================= */
/* TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                         */
/* NAME                                                                    */
/*     DSP_fft32x32 -- DSP_fft32x32                                        */
/*                                                                         */
/*                                                                         */
/* REVISION DATE                                                           */
/*     06-May-2005                                                         */
/*                                                                         */
/*  USAGE                                                                   */
/*                                                                          */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*      void DSP_fft32x32                                                   */
/*      (                                                                   */
/*          int * w,                                                        */
/*          int nx,                                                         */
/*          int * x,                                                        */
/*          int * y                                                         */
/*      )                                                                   */
/*                                                                          */
/*      w[2*nx]:    Pointer to vector of Q.31 FFT coefficients of size      */
/*                  2*nx elements.                                          */
/*                                                                          */
/*      nx:         Number of complex elements in vector x.                 */
/*                                                                          */
/*      x[2*nx]:    Pointer to input vector of size 2*nx elements.          */
/*                                                                          */
/*      y[2*nx]:    Pointer to output vector of size 2*nx elements.         */
/*                                                                          */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*                                                                          */
/*      This code performs a Radix-4 FFT with digit reversal.  The code     */
/*      uses a special ordering of twiddle factors and memory accesses      */
/*      to improve performance in the presence of cache.  It operates       */
/*      largely in-place, but the final digit-reversed output is written    */
/*      out-of-place.                                                       */
/*                                                                          */
/*      This code requires a special sequence of twiddle factors stored     */
/*      in Q1.31 fixed-point format.  The following C code illustrates      */
/*      one way to generate the desired twiddle-factor array:               */
/*                                                                          */
/*      #include <math.h>                                                   */
/*                                                                          */
/*      #ifndef PI                                                          */
/*      # define PI (3.14159265358979323846)                                */
/*      #endif                                                              */
/*                                                                          */
/*                                                                          */
/*      static int d2i(double d)                                            */
/*      {                                                                   */
/*         if (d >=  2147483647.0) return (int)0x7FFFFFFF;                  */
/*         if (d <= -2147483648.0) return (int)0x80000000;                  */
/*         return (int)d;                                                   */
/*      }                                                                   */
/*                                                                          */
/*                                                                          */
/*      int gen_twiddle_fft32x32(int *w, int n, double scale)               */
/*      {                                                                   */
/*          int i, j, k, s=0, t;                                            */
/*                                                                          */
/*          for (j = 1, k = 0; j < n >> 2; j = j << 2, s++)                 */
/*          {                                                               */
/*              for (i = t=0; i < n >> 2; i += j, t++)                      */
/*              {                                                           */
/*                 w[k +  5] = d2i(scale * cos(6.0 * PI * i / n));          */
/*                 w[k +  4] = d2i(scale * sin(6.0 * PI * i / n));          */
/*                                                                          */
/*                 w[k +  3] = d2i(scale * cos(4.0 * PI * i / n));          */
/*                 w[k +  2] = d2i(scale * sin(4.0 * PI * i / n));          */
/*                                                                          */
/*                 w[k +  1] = d2i(scale * cos(2.0 * PI * i / n));          */
/*                 w[k +  0] = d2i(scale * sin(2.0 * PI * i / n));          */
/*                                                                          */
/*                                                                          */
/*                 k += 6;                                                  */
/*                 }                                                        */
/*             }                                                            */
/*                                                                          */
/*          return k;                                                       */
/*        }                                                                 */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*                                                                          */
/*      The following C code represents an implementation of the Cooley     */
/*      Tukey radix 4 DIF FFT. It accepts the inputs in normal order and    */
/*      produces the outputs in digit reversed order. The natural C code    */
/*      shown in this file on the other hand, accepts the inputs in nor-    */
/*      mal order and produces the outputs in normal order.                 */
/*                                                                          */
/*      Several transformations have been applied to the original Cooley    */
/*      Tukey code to produce the natural C code description shown here.    */
/*      In order to understand these it would first be educational to       */
/*      understand some of the issues involved in the conventional Cooley   */
/*      Tukey FFT code.                                                     */
/*                                                                          */
/*      void radix4(int n, short x[], short wn[])                           */
/*      {                                                                   */
/*          int    n1,  n2,  ie,   ia1,  ia2, ia3;                          */
/*          int    i0,  i1,  i2,    i3,    i, j,     k;                     */
/*          short  co1, co2, co3,  si1,  si2, si3;                          */
/*          short  xt0, yt0, xt1,  yt1,  xt2, yt2;                          */
/*          short  xh0, xh1, xh20, xh21, xl0, xl1,xl20,xl21;                */
/*                                                                          */
/*          n2 = n;                                                         */
/*          ie = 1;                                                         */
/*          for (k = n; k > 1; k >>= 2)                                     */
/*          {                                                               */
/*              n1 = n2;                                                    */
/*              n2 >>= 2;                                                   */
/*              ia1 = 0;                                                    */
/*                                                                          */
/*              for (j = 0; j < n2; j++)                                    */
/*              {                                                           */
/*                   ia2 = ia1 + ia1;                                       */
/*                   ia3 = ia2 + ia1;                                       */
/*                                                                          */
/*                   co1 = wn[2 * ia1    ];                                 */
/*                   si1 = wn[2 * ia1 + 1];                                 */
/*                   co2 = wn[2 * ia2    ];                                 */
/*                   si2 = wn[2 * ia2 + 1];                                 */
/*                   co3 = wn[2 * ia3    ];                                 */
/*                   si3 = wn[2 * ia3 + 1];                                 */
/*                   ia1 = ia1 + ie;                                        */
/*                                                                          */
/*                   for (i0 = j; i0< n; i0 += n1)                          */
/*                   {                                                      */
/*                       i1 = i0 + n2;                                      */
/*                       i2 = i1 + n2;                                      */
/*                       i3 = i2 + n2;                                      */
/*                                                                          */
/*                                                                          */
/*                       xh0  = x[2 * i0    ] + x[2 * i2    ];              */
/*                       xh1  = x[2 * i0 + 1] + x[2 * i2 + 1];              */
/*                       xl0  = x[2 * i0    ] - x[2 * i2    ];              */
/*                       xl1  = x[2 * i0 + 1] - x[2 * i2 + 1];              */
/*                                                                          */
/*                       xh20 = x[2 * i1    ] + x[2 * i3    ];              */
/*                       xh21 = x[2 * i1 + 1] + x[2 * i3 + 1];              */
/*                       xl20 = x[2 * i1    ] - x[2 * i3    ];              */
/*                       xl21 = x[2 * i1 + 1] - x[2 * i3 + 1];              */
/*                                                                          */
/*                       x[2 * i0    ] = xh0 + xh20;                        */
/*                       x[2 * i0 + 1] = xh1 + xh21;                        */
/*                                                                          */
/*                       xt0  = xh0 - xh20;                                 */
/*                       yt0  = xh1 - xh21;                                 */
/*                       xt1  = xl0 + xl21;                                 */
/*                       yt2  = xl1 + xl20;                                 */
/*                       xt2  = xl0 - xl21;                                 */
/*                       yt1  = xl1 - xl20;                                 */
/*                                                                          */
/*                       x[2 * i1    ] = (xt1 * co1 + yt1 * si1) >> 15;     */
/*                       x[2 * i1 + 1] = (yt1 * co1 - xt1 * si1) >> 15;     */
/*                       x[2 * i2    ] = (xt0 * co2 + yt0 * si2) >> 15;     */
/*                       x[2 * i2 + 1] = (yt0 * co2 - xt0 * si2) >> 15;     */
/*                       x[2 * i3    ] = (xt2 * co3 + yt2 * si3) >> 15;     */
/*                       x[2 * i3 + 1] = (yt2 * co3 - xt2 * si3) >> 15;     */
/*                   }                                                      */
/*             }                                                            */
/*                                                                          */
/*             ie <<= 2;                                                    */
/*         }                                                                */
/*     }                                                                    */
/*                                                                          */
/*      The conventional Cooley Tukey FFT, is written using three loops.    */
/*      The outermost loop "k" cycles through the stages. There are log     */
/*      N to the base 4 stages in all. The loop "j" cycles through the      */
/*      groups of butterflies with different twiddle factors, loop "i"      */
/*      reuses the twiddle factors for the different butterflies within     */
/*      a stage. It is interesting to note the following:                   */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*      Stage#     #Groups     # Butterflies with common     #Groups*Bflys  */
/*                               twiddle factors                            */
/* ------------------------------------------------------------------------ */
/*       1         N/4          1                            N/4            */
/*       2         N/16         4                            N/4            */
/*       ..                                                                 */
/*       logN      1            N/4                          N/4            */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/*      The following statements can be made based on above observations:   */
/*                                                                          */
/*      a) Inner loop "i0" iterates a variable number of times. In          */
/*      particular the number of iterations quadruples every time from      */
/*      1..N/4. Hence software pipelining a loop that iterates a variable   */
/*      number of times is not profitable.                                  */
/*                                                                          */
/*      b) Outer loop "j" iterates a variable number of times as well.      */
/*      However the number of iterations is quartered every time from       */
/*      N/4 ..1. Hence the behaviour in (a) and (b) are exactly opposite    */
/*      to each other.                                                      */
/*                                                                          */
/*      c) If the two loops "i" and "j" are colaesced together then they    */
/*      will iterate for a fixed number of times namely N/4. This allows    */
/*      us to combine the "i" and "j" loops into 1 loop. Optimized impl-    */
/*      ementations will make use of this fact.                             */
/*                                                                          */
/*      In addition the Cooley Tukey FFT accesses three twiddle factors     */
/*      per iteration of the inner loop, as the butterflies that re-use     */
/*      twiddle factors are lumped together. This leads to accessing the    */
/*      twiddle factor array at three points each sepearted by "ie". Note   */
/*      that "ie" is initially 1, and is quadrupled with every iteration.   */
/*      Therfore these three twiddle factors are not even contiguous in     */
/*      the array.                                                          */
/*                                                                          */
/*      In order to vectorize the FFT, it is desirable to access twiddle    */
/*      factor array using double word wide loads and fetch the twiddle     */
/*      factors needed. In order to do this a modified twiddle factor       */
/*      array is created, in which the factors WN/4, WN/2, W3N/4 are        */
/*      arranged to be contiguous. This eliminates the seperation between   */
/*      twiddle factors within a butterfly. However this implies that as    */
/*      the loop is traversed from one stage to another, that we maintain   */
/*      a redundant version of the twiddle factor array. Hence the size     */
/*      of the twiddle factor array increases as compared to the normal     */
/*      Cooley Tukey FFT.  The modified twiddle factor array is of size     */
/*      "2 * N" where the conventional Cooley Tukey FFT is of size"3N/4"    */
/*      where N is the number of complex points to be transformed. The      */
/*      routine that generates the modified twiddle factor array was        */
/*      presented earlier. With the above transformation of the FFT,        */
/*      both the input data and the twiddle factor array can be accessed    */
/*      using double-word wide loads to enable packed data processing.      */
/*                                                                          */
/*      The final stage is optimised to remove the multiplication as        */
/*      w0 = 1.  This stage also performs digit reversal on the data,       */
/*      so the final output is in natural order.                            */
/*                                                                          */
/*      The fft() code shown here performs the bulk of the computation      */
/*      in place. However, because digit-reversal cannot be performed       */
/*      in-place, the final result is written to a separate array, y[].     */
/*                                                                          */
/*      There is one slight break in the flow of packed processing that     */
/*      needs to be comprehended. The real part of the complex number is    */
/*      in the lower half, and the imaginary part is in the upper half.     */
/*      The flow breaks in case of "xl0" and "xl1" because in this case     */
/*      the real part needs to be combined with the imaginary part because  */
/*      of the multiplication by "j". This requires a packed quantity like  */
/*      "xl21xl20" to be rotated as "xl20xl21" so that it can be combined   */
/*       using add2's and sub2's. Hence the natural version of C code       */
/*      shown below is transformed using packed data processing as shown:   */
/*                                                                          */
/*                       xl0  = x[2 * i0    ] - x[2 * i2    ];              */
/*                       xl1  = x[2 * i0 + 1] - x[2 * i2 + 1];              */
/*                       xl20 = x[2 * i1    ] - x[2 * i3    ];              */
/*                       xl21 = x[2 * i1 + 1] - x[2 * i3 + 1];              */
/*                                                                          */
/*                       xt1  = xl0 + xl21;                                 */
/*                       yt2  = xl1 + xl20;                                 */
/*                       xt2  = xl0 - xl21;                                 */
/*                       yt1  = xl1 - xl20;                                 */
/*                                                                          */
/*                       xl1_xl0   = _sub2(x21_x20, x21_x20)                */
/*                       xl21_xl20 = _sub2(x32_x22, x23_x22)                */
/*                       xl20_xl21 = _rotl(xl21_xl20, 16)                   */
/*                                                                          */
/*                       yt2_xt1   = _add2(xl1_xl0, xl20_xl21)              */
/*                       yt1_xt2   = _sub2(xl1_xl0, xl20_xl21)              */
/*                                                                          */
/*      Also notice that xt1, yt1 endup on seperate words, these need to    */
/*      be packed together to take advantage of the packed twiddle fact     */
/*      ors that have been loaded. In order for this to be achieved they    */
/*      are re-aligned as follows:                                          */
/*                                                                          */
/*      yt1_xt1 = _packhl2(yt1_xt2, yt2_xt1)                                */
/*      yt2_xt2 = _packhl2(yt2_xt1, yt1_xt2)                                */
/*                                                                          */
/*      The packed words "yt1_xt1" allows the loaded"sc" twiddle factor     */
/*      to be used for the complex multiplies. The real part os the         */
/*      complex multiply is implemented using _dotp2. The imaginary         */
/*      part of the complex multiply is implemented using _dotpn2           */
/*      after the twiddle factors are swizzled within the half word.        */
/*                                                                          */
/*      (X + jY) ( C + j S) = (XC + YS) + j (YC - XS).                      */
/*                                                                          */
/*      The actual twiddle factors for the FFT are cosine, - sine. The      */
/*      twiddle factors stored in the table are csine and sine, hence       */
/*      the sign of the "sine" term is comprehended during multipli-        */
/*      cation as shown above.                                              */
/*                                                                          */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*                                                                          */
/*      The size of the FFT, n, must be a power of 4 and greater than       */
/*      or equal to 16 and less than 32768.                                 */
/*                                                                          */
/*      The arrays 'x[]', 'y[]', and 'w[]' all must be aligned on a         */
/*      double-word boundary for the "optimized" implementations.           */
/*                                                                          */
/*      The input and output data are complex, with the real/imaginary      */
/*      components stored in adjacent locations in the array.  The real     */
/*      components are stored at even array indices, and the imaginary      */
/*      components are stored at odd array indices.                         */
/*                                                                          */
/*  C CODE                                                                  */
/*                                                                          */
/*                                                                          */
/*  NOTES                                                                   */
/*                                                                          */
/*                                                                          */
/*  CYCLES                                                                  */
/*                                                                          */
/*      cycles = [12*N/8+12]*ceil[log4(N)-1]+6*N/4+79                       */
/*      For nx = 512, cycles = 3967                                         */
/*                                                                          */
/*  CODESIZE                                                                */
/*                                                                          */
/*      1056 bytes                                                          */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2006 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
/* ======================================================================= */
/* ======================================================================= */
#ifndef DSP_FFT32X32_H_
#define DSP_FFT32X32_H_ 1

void DSP_fft32x32
(
    int * w,
    int nx,
    int * x,
    int * y
);

#endif
/* ======================================================================== */
/*  End of file:  dsp_fft32x32.h                                            */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2006 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
