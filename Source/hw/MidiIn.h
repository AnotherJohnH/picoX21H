//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief MIDI-in configuration

#pragma once

#include "STB/MIDIInterface.h"
#include "hw/Config.h"

#if defined(HW_MIDI_IN_UART1)
#include "MTL/chip/Uart.h"

#elif defined(HW_MIDI_IN_NATIVE)
#include "PLT/MIDIInterface.h"

#endif

namespace hw {

#if defined(HW_MIDI_IN_UART1)

//! Physical MIDI in on pico pin 27
class MidiIn : public MIDI::Interface
{
public:
   MidiIn(MIDI::Instrument& instrument_, bool debug_)
      : MIDI::Interface(instrument_, debug_)
   {}

private:
   bool empty() const override { return uart.empty(); }

   uint8_t rx() override { return uart.rx(); }

   void tx(uint8_t byte) override {}

   MTL::Uart1_P26_P27 uart{/* baud */      31250,
                           /* bits */      8,
                           /* parity */    MTL::UART::NONE,
                           /* stop bits */ 1,
                           /* pull up */   true};
};

#elif defined(HW_MIDI_IN_FAKE)

//! Fake MIDI in with hard coded messages
class MidiIn : public MIDI::Interface
{
public:
   MidiIn(MIDI::InstrumentBase& instrument_, bool debug_)
      : MIDI::Interface(instrument_, debug_)
   {}

private:
   bool empty() const override { return n == sizeof(data); }

   uint8_t rx() override { return data[n++]; }

   void tx(uint8_t byte) override {}

   unsigned n {0};

   uint8_t  data[7] =
   {
      0x90,        // NOTE ON
      0x3C, 0x7F,  // C vel=127
      0x40, 0x7F,  // E vel=127
      0x43, 0x7F   // G vel=127
   };
};

#elif defined(HW_MIDI_IN_NATIVE)

using MidiIn = PLT::MIDI::Interface;

#endif

} // namespace hw
