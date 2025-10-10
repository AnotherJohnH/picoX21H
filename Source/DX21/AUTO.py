#!/usr/bin/env python3
#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import math
import table
import sys

def sine64(index_6, x):
   """ Sine lookup table for iG10090 BBD modulator LFO """
   phase = ((index_6 + 0.5) * math.pi) / 32
   return int(math.sin(phase) * 32)


def linear_interp_filter(index, x):
   """ Re-construction filter for BBD simulation """
   if x <= 0.5:
      y = 2 * x
   else:
      y = 2 * (1 - x)
   return int(y * 0xFFFF)


filename = sys.argv[1]

data = []

with open(filename, 'rb') as file:
   while True:
      byte = file.read(1)
      if byte == b'':
         break
      data.append(int.from_bytes(byte, byteorder='big', signed=False))

table.gen("dx21_rom",
          func      = lambda i,x : data[i],
          typename  = "uint8_t",
          size      = 0x24A0,
          fmt       = '02x')

table.gen("iG10090_sine", func = sine64, typename = "int8_t", log2_size = 6)

table.gen("bbd_filter",
          func = linear_interp_filter,
          typename = "uint16_t",
          log2_size = 8,
          fmt = "04x")
