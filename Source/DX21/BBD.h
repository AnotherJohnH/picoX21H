//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// \brief Simulation of a Bucket Brigade Delay 

#pragma once

#include <cstdint>

#include "Table_bbd_filter.h"

template <unsigned LOG2_BUCKETS, typename TYPE = int32_t>
class BBD
{
public:
   BBD() = default;

   void setMod(signed mod_)
   {
      delta_in  = PHASE_MAX + mod_;
      delta_out = PHASE_MAX + mod_;
   }

   //! Send input to BBD and retrieve output
   TYPE sendRecv(TYPE in_)
   {
      bbd_in.push(in_);

      phase_in += delta_in;
      while(phase_in >= PHASE_MAX)
      {
         phase_in -= PHASE_MAX;

         TYPE input  = bbd_in.filter(phase_in);
         TYPE output = buckets.pushPop(input);
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
      void push(TYPE sample_)
      {
         buffer[index] = sample_;
         index = (index + 1) & MASK;
      }

      TYPE pushPop(TYPE sample_)
      {
         TYPE out = buffer[index];
         buffer[index] = sample_;
         index = (index + 1) & MASK;
         return out;
      }

      TYPE filter(unsigned offset_)
      {
         static constexpr unsigned LOG2_TABLE_BBD_FILTER_HALF_SIZE = LOG2_TABLE_BBD_FILTER_SIZE - 1;

         offset_ = offset_ >> (PHASE_BITS - LOG2_TABLE_BBD_FILTER_HALF_SIZE);

         TYPE total = 0;

         for(unsigned i = 0; i < 2; ++i)
         {
            total   += buffer[(index + i) & MASK] * table_bbd_filter[offset_];
            offset_ += 1 << LOG2_TABLE_BBD_FILTER_HALF_SIZE;
         }

         return total >> 16;
      }

   private:
      static const unsigned SIZE = 1 << LOG2_SIZE;
      static const unsigned MASK = SIZE - 1;

      TYPE  buffer[SIZE] = {};
      unsigned index{0};
   };

   static const unsigned PHASE_BITS = 16;
   static const unsigned PHASE_MAX  = 1 << PHASE_BITS;
   static const unsigned PHASE_MASK = PHASE_MAX - 1;

   unsigned             phase_in{0};
   unsigned             delta_in{PHASE_MAX};
   unsigned             phase_out{0};
   unsigned             delta_out{PHASE_MAX};
   Buffer<2>            bbd_in{};
   Buffer<LOG2_BUCKETS> buckets{};
   Buffer<2>            bbd_out{};
};
