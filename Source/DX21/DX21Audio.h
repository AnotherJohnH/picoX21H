//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "BBD.h"
#include "iG10090.h"

namespace DX21 {

class Audio
{
public:
   Audio() = default;

   void setSampleRate(unsigned sample_rate_)
   {
      modulator.setSampleRate(sample_rate_);
   }

   //! Simulation of DX21 audio processing
   void process(int16_t& left, int16_t& right)
   {
      int32_t dry_l = (left * (128 - balance)) / 64;
      int32_t dry_r = (right * balance) / 64;

      int32_t mix_l;
      int32_t mix_r;

      if (chorus)
      {
         bbd.setMod(modulator.sample());

         int32_t wet = bbd.sendRecv((dry_l + dry_r) / 2);

         mix_l = (dry_l + wet) / 2;
         mix_r = (dry_r - wet) / 2;
      }
      else
      {
         mix_l = dry_l;
         mix_r = dry_r;
      }

      // TODO LOG volume
      left  = (mix_l * volume) / 128;
      right = (mix_r * volume) / 128;
   }

   volatile uint8_t balance{64};
   volatile uint8_t volume{127};
   volatile bool    chorus{false};

   BBD</* LOG2_SIZE */ 8> bbd{};
   iG10090                modulator{/* clock */ 8000, /* chorus */ 5, /* tremolo */ 18};
};

} // namespace DX21
