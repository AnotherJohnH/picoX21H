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

// \brief Audio processing

#if not defined(HW_NATIVE)

#include "MTL/MTL.h"
#include "MTL/Pins.h"

#include "MTL/chip/PioYMDAC.h"
#include "MTL/chip/PioI2S_S16.h"

static MTL::PioYMDAC<MTL::Pio1>   ymdac_in{};
static MTL::PioI2S_S16<MTL::Pio0> i2s_out{};

#include "DX21/DX21Synth.h"

extern DX21::Synth dx21_synth;


static void runDAC()
{
   while(true)
   {
      int16_t left, right;

      ymdac_in.pop(left, right);

      dx21_synth.audio.process(left, right);

      uint32_t packed = (left << 16) | (right & 0xFFFF);
      i2s_out.push(packed);
   }
}

void startAudio(unsigned ym2151_clock_hz_)
{
   ymdac_in.download(ym2151_clock_hz_, /* CLK SD SAM1 */ MTL::PIN_10);
   ymdac_in.start();

   i2s_out.download(ym2151_clock_hz_, /* SD */ MTL::PIN_31, /* LRCLK SCLK */ MTL::PIN_32);
   i2s_out.start();

   MTL_start_core(1, runDAC);
}

#endif

