//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief 16x2 Alphanumeric LCD interface

#pragma once

#include "hw/Config.h"

#if defined(HW_LCD_I2C)
#include "MTL/AlphaNumLcd_I2C.h"

#elif defined(HW_LCD_I2C_BRIDGE)
#include "MTL/AlphaNumLcd_I2CBridge.h"

#else
#define HW_LCD_NONE
#endif

namespace hw {

#if defined(HW_LCD_I2C)

// pico pin 24    : SDA
// pico pin 25    : SCL

class Lcd : public MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
   }
};

#elif defined(HW_LCD_I2C_BRIDGE)

// pico pin 24    : SDA
// pico pin 25    : SCL

class Lcd : public MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
   }
};

#endif

} // namespace hw
