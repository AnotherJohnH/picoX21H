//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Target hardware configurations

#pragma once

#if defined(HW_I2S_DAC)

#define HW_DESCR "I2S DAC"

#define HW_DAC_I2S_ANY
#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_LED
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#elif defined(HW_PWM_DAC)

#define HW_DESCR "PWM DAC"

#define HW_DAC_PWM
#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_LED
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#elif defined(HW_NATIVE)

#define HW_DESCR "native"

#define HW_DAC_NATIVE
#define HW_MIDI_IN_NATIVE

#else

#error "Target hardware not specified"

#endif
