//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <cstring>

namespace SysEx {

static const unsigned NUM_OP   = 4;
static const unsigned NAME_LEN = 10;

enum LfoWave : uint8_t
{
   TRIANGLE        = 0,
   SAW_DOWN        = 1,
   SAW_UP          = 2,
   SQUARE          = 3,
   SINE            = 4,
   SAMPLE_AND_HOLD = 5
};

//! Envelope generator parameters
struct AmpEG
{
   uint8_t ar{};  //!< Attack rate
   uint8_t d1r{}; //!< 1st decay rate
   uint8_t d2r{}; //!< 2nd decay rate
   uint8_t rr{};  //!< Release rate
   uint8_t d1l{}; //!< 1st decay level
};


struct PitchEG
{
   uint8_t r[3];
   uint8_t l[3];
};

//! Packed voice parameters
struct Packed
{
   Packed() = default;

   struct Op
   {
      AmpEG   eg{};
      uint8_t ks{};                             //!< Keyboard sclaing level
      uint8_t amp_mod__eg_bias__sens__key_vel{};
      uint8_t out_level{};                      //!< Output level
      uint8_t freq_stuff{};
      uint8_t ksr__dt1{};

   } op[NUM_OP];

   union
   {
      struct
      {
         uint8_t alg      : 3;
         uint8_t fb       : 3;
         uint8_t pad      : 1;
         uint8_t lfo_sync : 1;
      };

      uint8_t alg_bits;
   };

   uint8_t lfo_speed;
   uint8_t lfo_delay;
   uint8_t pmd;
   uint8_t amd;
   uint8_t pms_amp__mod_sens__lfo_wave;
   uint8_t transpose;
   uint8_t pitch_bend_range;
   uint8_t chorus_switch;
   uint8_t porta_time;
   uint8_t foot_volume;
   uint8_t mod_wh_p;
   uint8_t mod_wh_a;
   uint8_t breath_p;
   uint8_t breath_a;
   uint8_t breath_pb;
   uint8_t breath_eg;
   char    name[NAME_LEN];        // ASCII
   PitchEG pitch_eg;
};


struct Voice
{
   Voice() = default;

   //! Construct from a patch in a packed table
   Voice(const uint8_t* packed_patch_table_, unsigned index_ = 0)
   {
      *this = (const SysEx::Packed&)packed_patch_table_[index_ * sizeof(SysEx::Packed)];
   }

   void print(unsigned ) const;

   //! Copy in a packed SysEx voice
   void operator=(const Packed& packed)
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         op[i] = packed.op[i];
      }

      alg = packed.alg;
      fb  = packed.fb;

      memcpy(name, packed.name, NAME_LEN);

      pitch_eg = packed.pitch_eg;
   }

   struct Op
   {
      Op() = default;

      void print(unsigned n) const;

      void operator=(const Packed::Op& packed)
      {
         eg        = packed.eg;
         out_level = packed.out_level;
         freq      = packed.freq_stuff >> 2;
      }

      AmpEG   eg{};
      uint8_t level_scale{};
      uint8_t rate_scale{};
      uint8_t eg_bias_sens{};
      uint8_t a_mod_sens{};
      uint8_t key_vel{};
      uint8_t out_level{};
      uint8_t freq{};
      uint8_t detune{};

   } op[NUM_OP];

   uint8_t alg{};
   uint8_t fb{};
   uint8_t lfo_speed;
   uint8_t lfo_delay;
   uint8_t lfo_pmd;
   uint8_t lfo_amd;
   uint8_t lfo_sync;
   uint8_t lfo_wave;

   uint8_t p_mod_sens;
   uint8_t a_mod_sens;
   uint8_t transpose;
   uint8_t poly_mode;
   uint8_t porta_time;
   uint8_t foot_volume;
   uint8_t foot_sustain;
   uint8_t foot_porta;
   uint8_t bc_pitch;
   uint8_t bc_amp;
   uint8_t bc_pitch_bias;
   uint8_t bc_eg_bias;
   char    name[NAME_LEN];        // ASCII
   PitchEG pitch_eg;
};

} // namespace SysEx
