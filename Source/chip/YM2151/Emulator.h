//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Emulation of Yamaha YM2151

#pragma once

#include "YM2151/Interface.h"

namespace YM2151 {

class Emulator : public Interface
{
public:
   Emulator() = default;

   void start(unsigned clock_freq_)
   {
   }

   void mixOut(Sample& mix_)
   {
      if (mute) return;

      int16_t left{0};
      int16_t right{0};

      Sample out{left, right};
      mixer(out, mix_);
   }

private:
   //! Write a byte to the YM2151 bus
   void writeBus(bool a0_, uint8_t value_) override
   {
   }

   //! 
   void waitForReady() override
   {
   }
};

} // namespace YM2151
