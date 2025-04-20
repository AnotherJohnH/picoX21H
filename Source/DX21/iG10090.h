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

// \brief iG10090 BBD Modulator simulation

#pragma once

#include <cstdint>

#include "Table_iG10090_sine.h"

class iG10090
{
public:
   iG10090() = default;

   int32_t tick()
   {
      if (lfo1.tick())
      {
         lfo2.tick();

         modulation = lfo1 * 4 + lfo2 * 4;
      }

      return modulation;
   }

private:
   template <unsigned DIV>
   class Lfo
   {
   public:
      Lfo()
      {
         out = table_iG10090_sine[phase];
      }

      operator int8_t() const { return out; }

      bool tick()
      {
         if (div-- == 0)
         {
            div   = DIV;
            phase = (phase + 1) & TABLE_IG10090_SINE_MASK;
            out   = table_iG10090_sine[phase];

            return true;
         }

         return false;
      }

   private:
      uint8_t div{DIV};
      uint8_t phase{0};
      int8_t  out;
   };

   Lfo</* DIV */ 19> lfo1{};
   Lfo</* DIV */ 16> lfo2{};

   int32_t modulation;
};
