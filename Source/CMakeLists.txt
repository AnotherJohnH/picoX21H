#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

set(python_path ${CMAKE_CURRENT_SOURCE_DIR}/../Platform/python)

add_subdirectory(DX21)
add_subdirectory(hw)

foreach(hw_config ${hw_config_list})

   add_executable(picoX21H_${hw_config}
                  picoX21H.cpp
                  audio.cpp)

   target_compile_definitions(picoX21H_${hw_config} PRIVATE HW_${hw_config})
   target_link_libraries(     picoX21H_${hw_config} PRIVATE DX21 HW)

endforeach()
