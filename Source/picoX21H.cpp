//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief DX21 hybrid simulation for Raspberry Pi Pico

#include <cstdio>

#if not defined(HW_NATIVE)
#include "MTL/MTL.h"
#endif

#include "DX21/DX21Synth.h"
#include "hw/hw.h"

#include "SynthIO.h"

// -----------------------------------------------------------------------------

static const bool  MIDI_DEBUG      = false;

static SynthIO     synth_io{};
static hw::Led     led{};

DX21::Synth dx21_synth{synth_io};


// --- Physical MIDI -----------------------------------------------------------

static hw::MidiIn  midi_in{dx21_synth, MIDI_DEBUG};


// --- USB MIDI ----------------------------------------------------------------

#if defined(HW_MIDI_USB_DEVICE)

static hw::MidiUSBDevice midi_usb{dx21_synth, 0x91C0, "picoX21H", MIDI_DEBUG};

extern "C" void IRQ_USBCTRL() { midi_usb.irq(); }

#endif


// --- 16x2 LCD display --------------------------------------------------------

#if not defined(HW_LCD_NONE)

static hw::Lcd lcd{};

#endif

void SynthIO::displayLCD(unsigned row, const char* text)
{
#if not defined(HW_LCD_NONE)
   lcd.move(0, row);
   lcd.print(text);
#endif
}


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

#if not defined(HW_NATIVE)
   unsigned ym2151_clock_hz = dx21_synth.start();

   startAudio(ym2151_clock_hz);
#endif

   while(true)
   {
      midi_in.tick();

#if defined(HW_MIDI_USB_DEVICE)
      midi_usb.tick();
#endif

      led = dx21_synth.isAnyVoiceOn();
   }

   return 0;
}

