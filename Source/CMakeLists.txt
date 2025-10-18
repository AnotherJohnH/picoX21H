#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_subdirectory(chip)
add_subdirectory(DX21)

foreach(hw_config ${hw_picoX21H_config_list})

   add_executable(picoX21H_${hw_config}
                  picoX21H.cpp)

   target_include_directories(picoX21H_${hw_config}
       PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

   target_compile_definitions(picoX21H_${hw_config} PRIVATE HW_${hw_config})

   target_link_libraries(picoX21H_${hw_config} PRIVATE DX21 HW)

   set_target_properties(picoX21H_${hw_config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
