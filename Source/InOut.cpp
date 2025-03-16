//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"

#include "YM2151.h"

#include "MTL/Gpio.h"

inline void wait(volatile uint32_t n)
{
   while(n--);
}

int main()
{
   MTL::Gpio::InOut<1, MTL::PIN_14> pin_io;
   MTL::Gpio::Out<1,   MTL::PIN_15> pin_status;

   unsigned state = 0;

   while(true)
   {
      switch(state)
      {
      case 0:
         pin_io.setOut();
         pin_io = true;
         break;

      case 1:
         pin_io.setOut();
         pin_io = false;
         break;

      case 2:
         pin_io.setOut();
         pin_io = true;
         break;

      case 3:
         pin_io.setIn();
         break;

      case 4:
         pin_status = pin_io;
         break;
      }

      state = (state + 1) % 5;
      wait(10000);
   }

   return 0;
}
