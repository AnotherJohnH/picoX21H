#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(test_DX21
      testMain.cpp
      testBBD.cpp
      testig10090.cpp
      testIIRFilter.cpp
       )

   target_link_libraries(test_DX21
      PRIVATE DX21 STB)

   add_test(NAME test_DX21 COMMAND test_DX21)

endif()
