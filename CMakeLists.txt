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

add_subdirectory(Platform/Hardware)
add_subdirectory(Source)
