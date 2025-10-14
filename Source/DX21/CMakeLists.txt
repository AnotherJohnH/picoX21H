#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_dx21_rom ${CMAKE_CURRENT_SOURCE_DIR}/rom/dx21_rom_v1_5.bin)
autogen_py(Table_iG10090_sine)
autogen_py(Table_bbd_filter)
autogen_py(Table_midi_vol)
autogen_py(Table_midi_pan)

add_library(DX21 STATIC
   Table_dx21_rom.cpp
   Table_iG10090_sine.cpp
   Table_bbd_filter.cpp
   SysEx.cpp
   )

target_include_directories(DX21
   PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
   )

target_link_libraries(DX21
   INTERFACE chip 
   )

add_subdirectory(test)
