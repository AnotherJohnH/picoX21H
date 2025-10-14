//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Stereo sample pair

#pragma once

#include <cstdint>

struct Sample
{
   Sample() = default;

   Sample(int16_t left_, int16_t right_)
      : left(left_)
      , right(right_)
   {
   }

   int16_t operator=(int32_t value_)
   {
      return left = right = value_;
   }

   void operator=(const Sample& that)
   {
      left  = that.left;
      right = that.right;
   }

   void operator+=(const Sample& that)
   {
      left  += that.left;
      right += that.right;
   }

   Sample operator*(const int32_t scalar_) volatile
   {
      return Sample((left * scalar_) >> 15, (right * scalar_) >> 15);
   }

   Sample operator*(const Sample& that) volatile
   {
      return Sample((left * that.left) >> 15, (right * that.right) >> 15);
   }

   int32_t left{};
   int32_t right{};
};
