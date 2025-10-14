//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Hardware Interface for Yamaha YM2151

#pragma once

#include "YM2151/Interface.h"

#include "MTL/Gpio.h"
#include "MTL/chip/PioClock.h"
#include "MTL/chip/PioYMDAC.h"

namespace YM2151 {

template <typename PIO_SYNTH,
          typename PIO_DAC,
          unsigned PIN_CTRL4,       // First pin for _IC, A0, _WR and _RD
          unsigned PIN_DATA8,       // First pin for D0-D7
          bool     REV_DATA>        // true => rev data bits
class Hardware : public Interface
{
public:
   Hardware() = default;

   void init(unsigned clock_freq_hz_,
             unsigned pin_clk_m_,
             unsigned pin_clk_sd_sam1_)
   {
      int sd = clock.download(clock_freq_hz_, pin_clk_m_);
      clock.start();

      hardReset();

      dac_in.download(clock_freq_hz_, pin_clk_sd_sam1_);
      dac_in.start();
   }

   bool setClock(unsigned clock_freq_hz_) override
   {
      clock.setClock(clock_freq_hz_);
      dac_in.setClock(clock_freq_hz_);

      return Chip::setClock(clock_freq_hz_);
   }

   //! Initialize bus signals and YM2151 registers uses IC pin
   void hardReset() override
   {
      data8.setHiZ();

      a0  = A0_ADDR;
      _cs = _rd = _wr = true;
      wait_ns(T_AH);

      _ic = false;
      usleep(T_INIT);
      _ic = true;

      Interface::hardReset();
   }

   void mixOut(Sample& mix_)
   {
      if (mute) return;

      int16_t left, right;
      dac_in.pop(left, right);

      Sample out{left, right};
      mixer(out, mix_);
   }

   static constexpr unsigned _IC = PIN_CTRL4 + 0;
   static constexpr unsigned A0  = PIN_CTRL4 + 1;
   static constexpr unsigned _WR = PIN_CTRL4 + 2;
   static constexpr unsigned _RD = PIN_CTRL4 + 3;

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
   void writeBus(bool a0_, uint8_t value_) override
   {
      data8.setOut();

      a0 = a0_;
      wait_ns(T_AS);

      _cs = _wr = false;
      wait_ns(T_CSW - T_DS);

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

   void waitForReady() override
   {
      while((readBus(A0_DATA) & (1<<7)) != 0);
   }

   static constexpr unsigned T_AS   = 10;    //!< Address setup (ns)
   static constexpr unsigned T_AH   = 10;    //!< Address hold (ns)
   static constexpr unsigned T_CSW  = 100;   //!< Chip slecet width (ns)
   static constexpr unsigned T_DS   = 50;    //!< Data write setup (ns)
   static constexpr unsigned T_DH   = 10;    //!< Data read/write hold (ns)
   static constexpr unsigned T_ACC  = 180;   //!< Read data access (ns)
   static constexpr unsigned T_INIT = 25000; //!< Chip initialisation (ns)

   //!< Bi-directional data bus
   MTL::Gpio::InOut<8, PIN_DATA8> data8;

   //!< Control signals
   MTL::Gpio::Out<1, _IC> _ic; //!< Initial clear
   MTL::Gpio::Out<1, A0>  a0;  //!< 0=>address, 1=>data
   MTL::Gpio::Out<1, _WR> _wr; //!< Write
   MTL::Gpio::Out<1, _RD> _rd; //!< Read

   // MTL::Gpio::Out<1, PIN_CTRL5+4> _cs; //!< Chip select
   bool _cs; //!< dummy chip select

   MTL::PioClock<PIO_SYNTH> clock{};  //! Clock out to YM2151
   MTL::PioYMDAC<PIO_DAC>   dac_in{};
};

} // namespace YM2151
