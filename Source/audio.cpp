//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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

