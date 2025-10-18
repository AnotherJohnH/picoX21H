#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(picoX21H
        LANGUAGES CXX C ASM
        VERSION 0.03)

include(CTest)

set(HARDWARE picoX21H)

add_subdirectory(Platform)
add_subdirectory(Hardware)
add_subdirectory(Source)

if(${PLT_TARGET} STREQUAL rpipico OR ${PLT_TARGET} STREQUAL rpipico2)
   math(EXPR plt_bcd_version "${CMAKE_MAJOR_VERSION} * 0x100 + ${CMAKE_MINOR_VERSION}"
        OUTPUT_FORMAT HEXADECIMAL)
   target_compile_definitions(MTL PUBLIC PLT_BCD_VERSION=${plt_bcd_version})
endif()
