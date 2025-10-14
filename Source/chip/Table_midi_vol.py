#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

MAX_ATTEN_DB = 60
MAX_LEVEL    = 0x7fff

def midi_vol(midi_vol_7, x):
   """ Convert MIDI volume to a normalized gain """
   # Hard code zero => infinite attenuation 
   if midi_vol_7 == 0:
      return 0
   atten_db = (127 - midi_vol_7) * MAX_ATTEN_DB / 126.0
   return int(math.pow(10, -atten_db / 20.0) * MAX_LEVEL)

table.gen("midi_vol",
          func = midi_vol,
          typename = "int16_t",
          log2_size = 7,
          fmt = '04x')
