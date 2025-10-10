//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "DX21/BBD.h"

#include "STB/Test.h"

TEST(BBD, basic)
{
   const unsigned LOG2_BUCKETS = 8;
   const unsigned FILTER_DELAY = 3;
   const unsigned BUCKETS      = 1 << LOG2_BUCKETS;

   BBD<LOG2_BUCKETS> bbd{};
   int32_t           last_out = 0;

   for(unsigned i = 0; i < (BUCKETS * 2); ++i)
   {
      int32_t in  = i;
      int32_t out = bbd.sendRecv(in);

      if (i > (BUCKETS + (FILTER_DELAY * 2)))
      {
         EXPECT_NE(out, 0);
         EXPECT_GT(out, last_out);
      }
      else if (i < BUCKETS)
      {
         EXPECT_EQ(out, 0);
      }

      last_out = out;
   }
}
