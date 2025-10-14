#!/usr/bin/env python3
#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table

def linear_interp_filter(index, x):
   """ Re-construction filter for BBD simulation """
   if x <= 0.5:
      y = 2 * x
   else:
      y = 2 * (1 - x)
   return int(y * 0xFFFF)

table.gen("bbd_filter",
          func = linear_interp_filter,
          typename = "uint16_t",
          log2_size = 8,
          fmt = "04x")
