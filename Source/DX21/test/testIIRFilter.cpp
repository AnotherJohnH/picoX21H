//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include "DX21/IIRFilter.h"

#include "STB/Test.h"

TEST(IIRFIlter, low_pass_order1)
{
   IIRFilter<1, 1, double, 1.0> filter{};

   // Coefs for 1st order Butterworth low pass filter fc=200Hz, fs=56000Hz
   static const double a[2] = {1.0, -0.97780811};
   static const double b[2] = {0.01109594, 0.01109594};

   filter.setCoef(a, b);

   for(unsigned i = 0; i < 2000; ++i)
   {
      double in  = ((i / 200) % 2) - 0.5;
      double out = filter.sendRecv(in);
      // printf("%u,%g,%g\n", i, in, out);
   }
}

TEST(IIRFIlter, low_pass_order2)
{
   IIRFilter<2, 2, double, 1.0> filter{};

   // Coefs for 2nd order Butterworth low pass filter fc=200Hz, fs=56000Hz
   static const double a[3] = {1.0,        -1.96826772, +0.96876339};
   static const double b[3] = {0.00012392, +0.00024783, +0.00012392};

   filter.setCoef(a, b);

   for(unsigned i = 0; i < 2000; ++i)
   {
      double in  = ((i / 200) % 2) - 0.5;
      double out = filter.sendRecv(in);
      // printf("%g,%g\n", in, out);
   }
}

TEST(IIRFIlter, low_pass_order1_int)
{
   IIRFilter<1, 1> filter{};

   // Coefs for 1st order Butterworth low pass filter fc=200Hz, fs=56000Hz
   static const int32_t a[2] = {FP16(1.0), FP16(-0.97780811)};
   static const int32_t b[2] = {FP16(0.01109594), FP16(0.01109594)};

   filter.setCoef(a, b);

   for(unsigned i = 0; i < 2000; ++i)
   {
      int32_t in  = ((i / 200) % 2) * FP16(1.0) - FP16(0.5);
      int32_t out = filter.sendRecv(in);
      // printf("%u,0x%04x,0x%04x\n", i, in, out);
   }
}
