#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_custom_command(
   COMMENT
      "Auto-generate for DX21"
   COMMAND
      ${CMAKE_COMMAND}
         -E env PYTHONPATH=${python_path}
         ${CMAKE_CURRENT_SOURCE_DIR}/AUTO.py
             ${CMAKE_CURRENT_SOURCE_DIR}/rom/dx21_rom_v1_5.bin
   DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/AUTO.py
      ${CMAKE_CURRENT_SOURCE_DIR}/rom/dx21_rom_v1_5.bin
   OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx21_rom.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx21_rom.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_iG10090_sine.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_iG10090_sine.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_bbd_filter.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_bbd_filter.cpp
   )

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

add_subdirectory(test)
