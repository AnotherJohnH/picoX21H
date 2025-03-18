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

// \brief DX21 hybrid simulation for Raspberry Pi Pico

#include <cstdio>
#include <unistd.h>

#if not defined(HW_NATIVE)

#include "MTL/MTL.h"
#include "MTL/Gpio.h"

#include "YM2151.h"
#include "PioYMDAC.h"
#include "PioI2S_S16.h"

#endif

#include "hw/hw.h"

// ------------------------------------------------------------------------------------

static const unsigned YM2151_CLOCK_HZ = 3579545;

static YM2151::Interface<MTL::Pio0,
                         /* CTRL5    */ MTL::PIN_5,
                         /* DATA8    */ MTL::PIN_11,
                         /* REV_DATA */ true> ym2151{};

static hw::Led led{};


void playNote(unsigned ch_)
{
   // Config operator C2
   ym2151.setOp<YM2151::MUL>(ch_, YM2151::OP_C2, 0x01);
   ym2151.setOp<YM2151::AR>( ch_, YM2151::OP_C2, 0x3F);
   ym2151.setOp<YM2151::D1R>(ch_, YM2151::OP_C2, 0);
   ym2151.setOp<YM2151::D1L>(ch_, YM2151::OP_C2, 0);
   ym2151.setOp<YM2151::D2R>(ch_, YM2151::OP_C2, 0);
   ym2151.setOp<YM2151::RR>( ch_, YM2151::OP_C2, 0x3F);
   ym2151.setOp<YM2151::TL>( ch_, YM2151::OP_C2, 0x00);

   // Config channel
   ym2151.setCh<YM2151::CONECT>(ch_, 0);
   ym2151.setCh<YM2151::FB>(    ch_, 0);
   ym2151.setCh<YM2151::RL>(    ch_, 0b11);
   ym2151.setCh<YM2151::KC>(    ch_, 0x4A);
   ym2151.setCh<YM2151::KF>(    ch_, 0);
   ym2151.setCh<YM2151::AMS>(   ch_, 0);
   ym2151.setCh<YM2151::PMS>(   ch_, 0);

   ym2151.noteOn(ch_, YM2151::OP_C2);
}


extern void startAudio(unsigned);

int main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : picoX21-H (%s)\n", HW_DESCR);
   printf("Author   : Copyright (c) 2025 John D. Haughton\n");
   printf("License  : MIT\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   ym2151.download(YM2151_CLOCK_HZ, /* CLK */ MTL::PIN_4);
   ym2151.start();

   startAudio(YM2151_CLOCK_HZ);

   playNote(0);

   while(true)
   {
      led = not led;

      usleep(500000);
   }

   return 0;
}

