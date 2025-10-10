//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "DX21/iG10090.h"

#include "STB/Test.h"

TEST(iG10090, basic)
{
   const unsigned SAMPLE_RATE = 56000;

   iG10090 ig10090{8000, /* chorus */ 5, /* tremolo */ 18};

   ig10090.setSampleRate(SAMPLE_RATE);

   for(unsigned i = 0; i < SAMPLE_RATE; ++i)
   {
      int32_t sample = ig10090.sample();

      printf("%d\n", sample);
   }
}
