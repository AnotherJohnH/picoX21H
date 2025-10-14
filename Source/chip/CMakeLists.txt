#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_midi_vol)
autogen_py(Table_midi_pan)

add_library(chip STATIC
            Table_midi_vol.cpp
            Table_midi_pan.cpp)

target_include_directories(chip
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
   PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
   )
