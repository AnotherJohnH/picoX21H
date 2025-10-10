//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
