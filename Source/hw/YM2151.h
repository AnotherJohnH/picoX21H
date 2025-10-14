//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Yamaha YM2151

#pragma once

#include "hw/Config.h"

#if defined(HW_YM2151)
#include "MTL/MTL.h"
#include "MTL/Pins.h"
#include "YM2151/Hardware.h"
#else
#include "YM2151/Emulator.h"
#endif

namespace hw {

#if defined(HW_YM2151)

class YM2151 : public ::YM2151::Hardware<MTL::Pio0,
                                         MTL::Pio1,
                                         HW_YM2151_CTRL4,
                                         HW_YM2151_DATA8,
                                         /* REV_DATA */ true>
{
public:
   YM2151()
   {
      MTL::config.gpio(_IC, "YM2151 /IC");
      MTL::config.gpio(A0,  "YM2151 A0");
      MTL::config.gpio(_WR, "YM2151 /WR");
      MTL::config.gpio(_RD, "YM2151 /RD");

      MTL::config.gpio(HW_YM2151_CLK, "YM2151 CLK");

      MTL::config.gpio(HW_YM2151_DAC_IN + 0, "YM2151 DAC SD");
      MTL::config.gpio(HW_YM2151_DAC_IN + 1, "YM2151 DAC CLK");
      MTL::config.gpio(HW_YM2151_DAC_IN + 2, "YM2151 DAC SAM1");

      MTL::config.gpio(HW_YM2151_DATA8 + 0, "YM2151 D7");
      MTL::config.gpio(HW_YM2151_DATA8 + 1, "YM2151 D6");
      MTL::config.gpio(HW_YM2151_DATA8 + 2, "YM2151 D5");
      MTL::config.gpio(HW_YM2151_DATA8 + 3, "YM2151 D4");
      MTL::config.gpio(HW_YM2151_DATA8 + 4, "YM2151 D3");
      MTL::config.gpio(HW_YM2151_DATA8 + 5, "YM2151 D2");
      MTL::config.gpio(HW_YM2151_DATA8 + 6, "YM2151 D1");
      MTL::config.gpio(HW_YM2151_DATA8 + 7, "YM2151 D0");
   }

   void start(unsigned clock_hz_)
   {
      init(clock_hz_,
           HW_YM2151_CLK,
           HW_YM2151_DAC_IN);
   }
};

#else

using YM2151 = YM2151::Emulator;

#endif

} // namespace hw
