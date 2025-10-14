//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Interface for output DAC

#pragma once

#include "Sample.h"

namespace chip {

class Dac
{     
public:
   Dac() = default;
   
   virtual void setSampleRate(unsigned sample_rate_hz_) = 0;
}; 

template <typename TYPE>
class DacImpl : public Dac
{
public:
   DacImpl(unsigned sample_rate_hz_)
      : dac(sample_rate_hz_)
   {
   }

   void setSampleRate(unsigned sample_rate_hz_) override
   {
      dac.setSampleRate(sample_rate_hz_);
   }

   void push(const Sample& sample_)
   {
      dac.push(sample_.left, sample_.right);
   }

private:
   TYPE dac;
};

}
