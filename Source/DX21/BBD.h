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

#pragma once

#include <cstdint>

#include "Table_bbd_filter.h"

template <unsigned LOG2_LENGTH>
class BBD
{
public:
   BBD() = default;

   void setRate(unsigned rate)
   {
   }

   //! Send input to BBD and retrieve output
   int32_t sendRecv(int32_t in_)
   {
      bbd_in.push(in_);

      phase_in += delta_in;
      while(phase_in >= PHASE_MAX)
      {
         phase_in -= PHASE_MAX;

         int32_t input  = bbd_in.filter(phase_in);
         int32_t output = buckets.pushPop(input);
         bbd_out.push(output);
      }

      phase_out += delta_out;
      while(phase_out >= PHASE_MAX)
      {
         phase_out -= PHASE_MAX;
      }

      return bbd_out.filter(phase_out);
   }

private:
   template <unsigned LOG2_SIZE>
   class Buffer
   {
   public:
      void push(int32_t sample_)
      {
         buffer[index++] = sample_;
         index = (index + 1) & MASK;
      }

      int32_t pushPop(int32_t sample_)
      {
         int32_t out = buffer[index];
         buffer[index++] = sample_;
         index = (index + 1) & MASK;
         return out;
      }

      int32_t filter(unsigned offset_)
      {
         static constexpr unsigned LOG2_TABLE_BBD_FILTER_HALF_SIZE = LOG2_TABLE_BBD_FILTER_SIZE - 1;

         offset_ = offset_ >> (PHASE_BITS - LOG2_TABLE_BBD_FILTER_HALF_SIZE);

         int32_t total;

         for(unsigned i = 0; i < 2; ++i)
         {
            total   += buffer[(index + i) & MASK] * table_bbd_filter[offset_];
            offset_ += LOG2_TABLE_BBD_FILTER_HALF_SIZE;
         }

         return total >> 16;
      }

   private:
      static const unsigned SIZE = 1 << LOG2_SIZE;
      static const unsigned MASK = SIZE - 1;

      int32_t  buffer[SIZE] = {};
      unsigned index{0};
   };

   static const unsigned PHASE_BITS = 16;
   static const unsigned PHASE_MAX  = 1 << PHASE_BITS;
   static const unsigned PHASE_MASK = PHASE_MAX - 1;

   unsigned            phase_in{0};
   unsigned            delta_in{PHASE_MAX};
   unsigned            phase_out{0};
   unsigned            delta_out{PHASE_MAX};
   Buffer<2>           bbd_in{};
   Buffer<LOG2_LENGTH> buckets{};
   Buffer<2>           bbd_out{};
};
