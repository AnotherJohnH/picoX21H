#!/usr/bin/env python3
#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

def sine64(index_6, x):
   """ Sine lookup table for iG10090 BBD modulator LFO """
   phase = ((index_6 + 0.5) * math.pi) / 32
   return int(math.sin(phase) * 32)

table.gen("iG10090_sine",
          func = sine64,
          typename = "int8_t",
          log2_size = 6)
