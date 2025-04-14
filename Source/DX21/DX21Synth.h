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

#pragma once

#include <cstdio>

#include "STB/MIDIInstrument.h"

#include "YM2151.h"
#include "SysEx.h"
#include "Table_dx21_rom.h"
#include "../SynthIO.h"

namespace DX21 {

class Synth : public MIDI::Instrument</* N */ 8>
{
public:
   Synth(SynthIO& synth_io_)
      : io(synth_io_)
   {
   }

   unsigned start(unsigned ym2151_clock_hz_ = YM2151_CLOCK_HZ)
   {
      ym2151.download(ym2151_clock_hz_);
      ym2151.start();

      for(unsigned i = 0; i < NUM_VOICES; ++i)
      {
         voiceProgram(i, 0);
      }

      io.displayLCD(0, "*   picoX21H   *");
      io.displayLCD(1, "*  SYNTHESIZER *");

      return ym2151_clock_hz_;
   }

private:
   // MIDI::Instrument implementation
   void voiceProgram(unsigned index_, uint8_t number_) override
   {
      SysEx::Voice voice{table_dx21_rom, number_};

      if (index_ == 0)
         voice.print(number_);

      for(unsigned i = 0; i < SysEx::NUM_OP; ++i)
      {
         uint8_t op;
         switch(i)
         {
         case 0: op = YM2151::OP_M1; break;
         case 1: op = YM2151::OP_C1; break;
         case 2: op = YM2151::OP_M2; break;
         case 3: op = YM2151::OP_C2; break;
         }

         ym2151.setOp<YM2151::AR>( index_, op, voice.op[i].eg.ar);
         ym2151.setOp<YM2151::D1R>(index_, op, voice.op[i].eg.d1r);
         ym2151.setOp<YM2151::D1L>(index_, op, voice.op[i].eg.d1l);
         ym2151.setOp<YM2151::D2R>(index_, op, voice.op[i].eg.d2r);
         ym2151.setOp<YM2151::RR>( index_, op, voice.op[i].eg.rr);
         ym2151.setOp<YM2151::MUL>(index_, op, voice.op[i].freq);
         ym2151.setOp<YM2151::TL>( index_, op, (99 - voice.op[i].out_level) * 128 / 99);
      }

      // Config channel
      ym2151.setCh<YM2151::CONECT>(index_, voice.alg);
      ym2151.setCh<YM2151::FB>(    index_, voice.fb);
      ym2151.setCh<YM2151::RL>(    index_, 0b11);

      ym2151.setCh<YM2151::KF>(    index_, 0);
      ym2151.setCh<YM2151::AMS>(   index_, 0);
      ym2151.setCh<YM2151::PMS>(   index_, 0);
   }

   void voiceMute(unsigned index_) override
   {
   }

   void voiceOn(unsigned index_, uint8_t midi_note_, uint8_t velocity_) override
   {
      static const unsigned table[12] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14};

      unsigned octave = midi_note_ / 12;
      unsigned note   = table[midi_note_ % 12];

      ym2151.setCh<YM2151::KC>(index_, (octave << 4) | note);

      ym2151.noteOn(index_);
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      ym2151.noteOff(index_);
   }

   void voicePressure(unsigned index_, uint8_t level_) override
   {
   }

   void voiceControl(unsigned index_, uint8_t control_, uint8_t value_) override
   {
      if (control_ == 119)
      {
         // Hack to allow program selection via CC119 for DAWs that
         // charge extra for easy MIDI program selection
         this->voiceProgram(index_, value_);
         return;
      }
   }

   void voicePitchBend(unsigned index_, int16_t value_) override
   {
   }

   static const unsigned YM2151_CLOCK_HZ = 3579545; //!< 3.579545 MHz
   static const uint8_t  ID_YAMAHA       = 67;

   SynthIO& io;

   YM2151::Interface<MTL::Pio0,
                     /* CTRL4    */ MTL::PIN_4,
                     /* CLK_M    */ MTL::PIN_9,
                     /* DATA8    */ MTL::PIN_14,
                     /* REV_DATA */ true> ym2151{};
};

} // namespace DX21
