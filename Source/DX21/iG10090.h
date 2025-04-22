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
#include "IIRFilter.h"

class iG10090
{
public:
   iG10090(unsigned clock_hz_,
           unsigned chorus_level_,
           unsigned tremolo_level_)
      : clock_hz(clock_hz_)
      , chorus_level(chorus_level_)
      , tremolo_level(tremolo_level_)
   {
      enableTremolo(true);
      enableChorus(true);

      // Coefs for 1st order Butterworth low pass filter fc=200Hz, fs=56000Hz
      static const int32_t a[2] = {FP16(1.0), FP16(-0.97780811)};
      static const int32_t b[2] = {FP16(0.01109594), FP16(0.01109594)};

      filter.setCoef(a, b);
   }

   void enableTremolo(bool enable_)
   {
      tremolo = enable_ ? tremolo_level : 0;
   }

   void enableChorus(bool enable_)
   {
      chorus = enable_ ? chorus_level : 0;
   }

   //! Set rate sample() will be called
   void setSampleRate(unsigned sample_rate_hz_)
   {
      delta = PHASE_MAX * clock_hz / sample_rate_hz_;
   }

   //! Get next modulation sample
   int32_t sample()
   {
      phase += delta;
      if (phase >= PHASE_MAX)
      {
         phase -= PHASE_MAX;

         if (lfo1.tick())
         {
            lfo2.tick();

            modulation = lfo1 * tremolo + lfo2 * chorus;
         }
      }

      return filter.sendRecv(modulation);
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

   static const unsigned PHASE_MAX = 0x10000;

   unsigned clock_hz;
   unsigned chorus_level;
   unsigned tremolo_level;

   unsigned chorus{0};
   unsigned tremolo{0};
   unsigned phase{0};
   unsigned delta{0};

   Lfo</* DIV */ 19> lfo1{};
   Lfo</* DIV */ 16> lfo2{};
   int32_t           modulation{0};
   IIRFilter<2,2>    filter{};
};
