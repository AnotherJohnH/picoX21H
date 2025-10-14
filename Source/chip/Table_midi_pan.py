#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

MAX_LEVEL = 0x7fff

def midi_pan(midi_pan_7, x):
   """ Convert MIDI pan to a normalized gain """
   # Equal power pan
   p = midi_pan_7 / 127
   return int(math.sin((math.pi / 2) * p) * MAX_LEVEL)

table.gen("midi_pan",
          func = midi_pan,
          typename = "int16_t",
          log2_size = 7,
          fmt = '04x')
