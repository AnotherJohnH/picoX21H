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
