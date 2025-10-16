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
      MTL::config.gpio(_IC, ">YM2151_3 /IC");
      MTL::config.gpio(A0,  ">YM2151_4  A0");
      MTL::config.gpio(_WR, ">YM2151_5 /WR");
      MTL::config.gpio(_RD, ">YM2151_6 /RD");

      MTL::config.gpio(HW_YM2151_CLK, ">YM2151_24 CLK");

      MTL::config.gpio(HW_YM2151_DAC_IN + 0, "<YM2151_21 DAC SD  ");
      MTL::config.gpio(HW_YM2151_DAC_IN + 1, "<YM2151_23 DAC CLK ");
      MTL::config.gpio(HW_YM2151_DAC_IN + 2, "<YM2151_20 DAC SAM1");

      MTL::config.gpio(HW_YM2151_DATA8 + 0, "=YM2151_18 D7");
      MTL::config.gpio(HW_YM2151_DATA8 + 1, "=YM2151_17 D6");
      MTL::config.gpio(HW_YM2151_DATA8 + 2, "=YM2151_16 D5");
      MTL::config.gpio(HW_YM2151_DATA8 + 3, "=YM2151_15 D4");
      MTL::config.gpio(HW_YM2151_DATA8 + 4, "=YM2151_14 D3");
      MTL::config.gpio(HW_YM2151_DATA8 + 5, "=YM2151_13 D2");
      MTL::config.gpio(HW_YM2151_DATA8 + 6, "=YM2151_12 D1");
      MTL::config.gpio(HW_YM2151_DATA8 + 7, "=YM2151_11 D0");
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
