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

// \brief Interface for Yamaha YM2151

#pragma once

#include <unistd.h>
#include <cstring>

#include "MTL/Gpio.h"
#include "MTL/chip/PioClock.h"

#define DBG if (0) printf

namespace YM2151 {

enum Param
{
   // Params
   TEST, NFRQ, NE, CLKA, CLKB, LOAD, F_RESET, IRQ_EN, CSM, LFRQ, AMD, PMD, W, CT,

   // Channel params
   CONECT, FB, RL, KC, KF, AMS, PMS,

   // Op params
   MUL, DT1, TL, AR, KS, D1R, AMS_EN, D2R, DT2, RR, D1L
};

static const uint8_t OP_M1  = 0b0001;
static const uint8_t OP_C1  = 0b0010;
static const uint8_t OP_M2  = 0b0100;
static const uint8_t OP_C2  = 0b1000;
static const uint8_t OP_ALL = OP_M1 | OP_C1 | OP_M2 | OP_C2;


template <typename PIO_TYPE,
          unsigned PIN_CLK_M,  // Pin for CLK_M
          unsigned PIN_CTRL4,  // First pin for _IC, A0, _WR and _RD
          unsigned PIN_DATA8,  // First pin for D0-D7
          bool     REV_DATA = false>
class Interface
{
public:
   Interface() = default;

   signed download(unsigned clock_freq_)
   {
      sd = clock.download(pio, clock_freq_, PIN_CLK_M);
      return sd;
   }

   void start()
   {
      pio.start(1 << sd);

      hardReset();
   }

   //! Initialize bus signals and YM2151 registers uses IC pin
   void hardReset()
   {
      data8.setHiZ();

      a0  = A0_ADDR;
      _cs = _rd = _wr = true;
      wait_ns(T_AH);

      _ic = false;
      usleep(T_INIT);
      _ic = true;

      memset(shadow, 0, sizeof(shadow));
   }

   //! Initialize registers to stop all sounds and timer activity
   void softReset()
   {
      set<NE>(0);     // Disable noise

      set<LOAD>(0);   // Stop timers
      set<IRQ_EN>(0); // Disable interrupts
      set<CSM>(0);    // Clear CSM

      for(unsigned ch = 0; ch < 8; ch++)
      {
         for(unsigned op = 0; op < 4; ch++)
         {
            set<D1L>(ch, op, 0xF);
            set<RR>( ch, op, 0x8);
         }
      }

      for(unsigned ch = 0; ch < 8; ch++)
      {
         noteOff(ch);
      }
   }

   void noteOn(unsigned channel_, uint8_t op_mask_ = OP_ALL)
   {
      writeReg(0x08, (op_mask_ << 3) | channel_);
   }

   void noteOff(unsigned channel_, uint8_t op_mask_ = OP_ALL)
   {
      writeReg(0x08, ((~op_mask_) << 3) | channel_);
   }

   template <YM2151::Param PARAM>
   void set(unsigned value_)
   {
      switch(PARAM)
      {
      case TEST:    writeReg(  0x01,       value_); break;

      case NFRQ:    writeField(0x0F, 9, 5, value_); break;
      case NE:      writeField(0x0F, 7, 1, value_); break;

      case CLKA:    writeReg(  0x10,       value_ >> 2);
                    writeField(0x11, 0, 2, value_); break;

      case CLKB:    writeReg(  0x12,       value_); break;

      case LOAD:    writeReg(  0x14, 0, 2, value_); break;
      case F_RESET: writeReg(  0x14, 4, 2, value_); break;
      case IRQ_EN:  writeReg(  0x14, 4, 2, value_); break;
      case CSM:     writeReg(  0x14, 7, 1, value_); break;

      case LFRQ:    writeReg(  0x18,       value_); break;
      case AMD:     writeReg(  0x19,       value_ & 0x7F); break;
      case PMD:     writeReg(  0x19,       value_ | 0x80); break;
      case W:       writeField(0x1B, 0, 2, value_); break;
      case CT:      writeField(0x1B, 6, 2, value_); break;

      default: break;
      }
   }

   //! Set channel parameter
   template <YM2151::Param PARAM>
   void setCh(unsigned channel_, unsigned value_)
   {
      switch(PARAM)
      {
      case CONECT: writeField(0x20 + channel_, 0, 3, value_); break;
      case FB:     writeField(0x20 + channel_, 3, 3, value_); break;
      case RL:     writeField(0x20 + channel_, 6, 2, value_); break;
      case KC:     writeField(0x28 + channel_, 0, 7, value_); break;
      case KF:     writeField(0x30 + channel_, 1, 7, value_); break;
      case AMS:    writeField(0x38 + channel_, 0, 2, value_); break;
      case PMS:    writeField(0x38 + channel_, 4, 3, value_); break;

      default: break;
      }
   }

   //! Set operator parameter
   template <YM2151::Param PARAM>
   void setOp(unsigned channel_, uint8_t op_, unsigned value_)
   {
      unsigned offset;

      switch(op_)
      {
      case OP_M1: offset =      channel_; break;
      case OP_M2: offset =  8 + channel_; break;
      case OP_C1: offset = 16 + channel_; break;
      case OP_C2: offset = 24 + channel_; break;
      }

      switch(PARAM)
      {
      case MUL:    writeField(0x40 + offset, 0, 4, value_); break;
      case DT1:    writeField(0x40 + offset, 4, 3, value_); break;
      case TL:     writeField(0x60 + offset, 0, 7, value_); break;
      case AR:     writeField(0x80 + offset, 0, 5, value_); break;
      case KS:     writeField(0x80 + offset, 6, 2, value_); break;
      case D1R:    writeField(0xA0 + offset, 0, 5, value_); break;
      case AMS_EN: writeField(0xA0 + offset, 7, 1, value_); break;
      case D2R:    writeField(0xC0 + offset, 0, 5, value_); break;
      case DT2:    writeField(0xC0 + offset, 6, 2, value_); break;
      case RR:     writeField(0xE0 + offset, 0, 4, value_); break;
      case D1L:    writeField(0xE0 + offset, 4, 4, value_); break;

      default: break;
      }
   }

   //! Write a field in a register
   void writeField(uint8_t  addr_,
                   unsigned ls_bit_,
                   unsigned bits_,
                   unsigned data_)
   {
      uint8_t data_mask = (1 << bits_) - 1;
      uint8_t reg_mask  = ~data_mask << ls_bit_;

      uint8_t data = (shadow[addr_] & reg_mask) |
                     (data_ & data_mask) << ls_bit_;

      writeReg(addr_, data);
   }

   //! Write a register
   void writeReg(uint8_t addr_, uint8_t data_)
   {
      if ((addr_ >= 0x20) && (data_ == shadow[addr_]))
         return;

      writeBus(A0_ADDR, addr_);
      writeBus(A0_DATA, data_);

      unsigned count = 0;

      while((readStatus() & (1<<7)) != 0)
      {
         ++count;
      }

      shadow[addr_] = data_;

      DBG("WR %02X => %02X [%u]\n", data_, addr_, count);
   }

   uint8_t readStatus()
   {
      return readBus(A0_DATA);
   }

   //! Read a register (expected value not actual)
   uint8_t readReg(uint8_t addr_) const
   {
      return shadow[addr_];
   }

private:
   static uint8_t revBits(uint8_t value_)
   {
      value_ = ((value_ & 0xF0) >> 4) | ((value_ & 0x0F) << 4);
      value_ = ((value_ & 0xCC) >> 2) | ((value_ & 0x33) << 2);
      value_ = ((value_ & 0xAA) >> 1) | ((value_ & 0x55) << 1);

      return value_;
   }

   //! Wait for at least the given nano-seconds
   void wait_ns(unsigned nano_seconds_)
   {
      // TODO nano sleep for improved performance
      usleep((nano_seconds_ / 1000) + 1);
   }

   //! Write a byte to the YM2151 bus
   void writeBus(bool a0_, uint8_t value_)
   {
      data8.setOut();

      a0 = a0_;
      wait_ns(T_AS);

      _cs = _wr = false;
      wait_ns(T_CW - T_DS);

      if (REV_DATA)
         data8 = revBits(value_);
      else
         data8 = value_;

      wait_ns(T_DS);

      _cs = _wr = true;
      wait_ns(T_DH);

      data8.setHiZ();
   }

   //! Read a byte from the YM2151 bus
   uint8_t readBus(bool a0_)
   {
      data8.setIn();

      a0 = a0_;
      wait_ns(T_AS);

      _cs = _rd = false;
      wait_ns(T_ACC);

      uint8_t value;
      if (REV_DATA)
         value = revBits(data8);
      else
         value = data8;

      _cs = _rd = true;
      wait_ns(T_DH);

      data8.setHiZ();

      return value;
   }

   static constexpr unsigned T_AS   = 10;    //!< Address setup (ns)
   static constexpr unsigned T_AH   = 10;    //!< Address hold (ns)
   static constexpr unsigned T_CW   = 100;   //!< Chip slecet wait (ns)
   static constexpr unsigned T_DS   = 50;    //!< Data write setup (ns)
   static constexpr unsigned T_DH   = 10;    //!< Data read/write hold (ns)
   static constexpr unsigned T_ACC  = 180;   //!< Read data access (ns)
   static constexpr unsigned T_INIT = 25000; //!< Chip initialisation (ns)

   static constexpr bool A0_ADDR = false;
   static constexpr bool A0_DATA = true;

   //!< Bi-directional data bus
   MTL::Gpio::InOut<8, PIN_DATA8> data8;

   //!< Control signals
   MTL::Gpio::Out<1, PIN_CTRL4+0> _ic; //!< Initial clear
   MTL::Gpio::Out<1, PIN_CTRL4+1> a0;  //!< 0=>address, 1=>data
   MTL::Gpio::Out<1, PIN_CTRL4+2> _wr; //!< Write
   MTL::Gpio::Out<1, PIN_CTRL4+3> _rd; //!< Read

   // MTL::Gpio::Out<1, PIN_CTRL5+4> _cs; //!< Chip select
   bool                           _cs; //!< dummy chip select

   uint8_t shadow[256];

   MTL::PioClock clock{};
   PIO_TYPE      pio{};
   int           sd{-1};
};

} // namespace YM2151
