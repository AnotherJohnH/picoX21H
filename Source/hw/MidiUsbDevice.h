//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief USB MIDI Device

#pragma once

#include "STB/MIDIInterface.h"
#include "hw/Config.h"

#if defined(HW_MIDI_USB_DEVICE)
#include "MTL/USBMidiInterface.h"
#endif

namespace hw {

#if defined(HW_MIDI_USB_DEVICE)

//! pico micro USB : MIDI in
class MidiUSBDevice
   : public MIDI::Interface
   , public MTL::USBDevice
{
public:
   MidiUSBDevice(MIDI::Instrument& instrument_,
                 uint16_t          device_id_,
                 const char*       device_name_,
                 bool              debug_ = false)
      : MIDI::Interface(instrument_, debug_)
      , MTL::USBDevice("https://github.com/AnotherJohnH",
                       device_id_, PLT_BCD_VERSION, device_name_,
                       PLT_COMMIT)
   {}

   bool empty() const override { return interface.empty(); }

   uint8_t rx() override { return interface.rx(); }

   void tx(uint8_t byte) override {}

   MTL::USBMidiInterface interface{this};
};

#endif

} // namespace hw
