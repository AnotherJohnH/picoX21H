#!/usr/bin/env python3
#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import table
import sys

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
