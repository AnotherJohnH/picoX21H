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

#if not defined(HW_NATIVE)

#include "MTL/MTL.h"
#include "YM2151.h"

#endif

#include "hw/hw.h"

// ------------------------------------------------------------------------------------

static hw::Led led{};

static YM2151::Interface<MTL::PIN_11,
                         MTL::PIN_5,
                         MTL::PIN_4,
                         /* swap_data_bits */ true> ym2151{};


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

   // Config operator C2 for channel 0
   ym2151.setOp<YM2151::AR>( 0, YM2151::OP_C2, 0x3F);
   ym2151.setOp<YM2151::D1R>(0, YM2151::OP_C2, 0);
   ym2151.setOp<YM2151::D1L>(0, YM2151::OP_C2, 0);
   ym2151.setOp<YM2151::D2R>(0, YM2151::OP_C2, 0);
   ym2151.setOp<YM2151::RR>( 0, YM2151::OP_C2, 0x3F);
   ym2151.setOp<YM2151::TL>( 0, YM2151::OP_C2, 0x3F);

   // Config channel 0
   ym2151.setCh<YM2151::CONECT>(0, 0);
   ym2151.setCh<YM2151::FB>(    0, 0);
   ym2151.setCh<YM2151::RL>(    0, 0b11);
   ym2151.setCh<YM2151::KC>(    0, 0x4A);
   ym2151.setCh<YM2151::KF>(    0, 0);
   ym2151.setCh<YM2151::AMS>(   0, 0);
   ym2151.setCh<YM2151::PMS>(   0, 0);

   ym2151.noteOn(0, YM2151::OP_C2);

   printf("DONE\n");

   led = true;

   return 0;
}
