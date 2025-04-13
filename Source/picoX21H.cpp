//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// \brief DX21 hybrid simulation for Raspberry Pi Pico

#include <cstdio>

#if not defined(HW_NATIVE)

#include "MTL/MTL.h"

#endif

#include "DX21/DX21Synth.h"
#include "hw/hw.h"

// -----------------------------------------------------------------------------

static const bool  MIDI_DEBUG      = true;

static DX21::Synth synth{};
static hw::Led     led{};


// --- Physical MIDI -----------------------------------------------------------

static hw::MidiIn  midi_in{synth, MIDI_DEBUG};


// --- USB MIDI ----------------------------------------------------------------

#if defined(HW_MIDI_USB_DEVICE)

static hw::MidiUSBDevice midi_usb{synth, 0x91C0, "picoX21H", MIDI_DEBUG};

extern "C" void IRQ_USBCTRL() { midi_usb.usb.irq(); }

#endif


// -----------------------------------------------------------------------------

extern void startAudio(unsigned);

int main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : picoX21-H (%s)\n", HW_DESCR);
   printf("Author   : Copyright (c) 2025 John D. Haughton\n");
   printf("License  : MIT\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   unsigned ym2151_clock_hz = synth.start();

   startAudio(ym2151_clock_hz);

   while(true)
   {
      midi_in.tick();

#if defined(HW_MIDI_USB_DEVICE)
      midi_usb.tick();
#endif

      led = synth.isAnyVoiceOn();
   }

   return 0;
}

