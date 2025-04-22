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
//
// \brief Infinite Impulse Response (IIR) filter

#pragma once

#include <cstdint>
#include <cstdio>

namespace {

constexpr int32_t FP16(double f) { return int32_t(f * (1<<16)); }

}

template <unsigned NUM_POLE,
          unsigned NUM_ZERO,
          typename TYPE = int32_t,
          TYPE SCALE = FP16(1.0)>
class IIRFilter
{
public:
   IIRFilter() = default;

   void setCoef(const TYPE* a_, const TYPE* b_)
   {
      for(unsigned i = 0; i <= NUM_POLE; ++i)
         a[i] = a_[i];

      for(unsigned i = 0; i <= NUM_ZERO; ++i)
         b[i] = b_[i];
   }

   //! Send input to filter and return output
   TYPE sendRecv(TYPE input_)
   {
      x[0] = input_;

      TYPE output = x[0] * b[0];

      for(unsigned i = NUM_ZERO; i > 0; --i)
      {
         output += x[i] * b[i];
         x[i] = x[i - 1];
      }

      for(unsigned i = NUM_POLE; i > 0; --i)
      {
         y[i] = y[i - 1];
         output -= y[i] * a[i];
      }

      output = output / SCALE;

      y[0] = output;

      return output;
   }

private:
   TYPE x[NUM_ZERO + 1] = {};
   TYPE b[NUM_ZERO + 1] = {};
   TYPE y[NUM_POLE + 1] = {};
   TYPE a[NUM_POLE + 1] = {};
};

