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

#include <cstdio>
#include <cstdarg>

#include "SysEx.h"

using namespace SysEx;

// Some helpers for pretty printing
static const char* FG_GREEN   = "\e[32m";
static const char* FG_DEFAULT = "\e[39m";
static void fgGreen()   { printf("%s", FG_GREEN); }
static void fgDefault() { printf("%s", FG_DEFAULT); }

static void fmtf(const char* fix, const char* format = "", ...)
{
   printf("%s%s%s ", FG_GREEN, fix, FG_DEFAULT);

   va_list ap;
   va_start(ap, format);
   vprintf(format, ap);
   va_end(ap);
}

void Voice::Op::print(unsigned n) const
{
   if (n == 0)
      fmtf("OP AR D1R D1L D2R RR  OUT FRQ", "\n");

   fgGreen();
   printf("%u  ", 4 - n);
   fgDefault();

   printf("%2u  %2u %2u  %2u  %2u  %2u  %2u",
          eg.ar, eg.d1r, eg.d1l, eg.d2r, eg.rr, out_level, freq);

   printf("\n");
}

void Voice::print(unsigned n) const
{  
   fmtf("#", "%03u", n);
   fmtf(" NAME");
   for(unsigned i = 0; i < NAME_LEN; i++)
      putchar(name[i]);
   fmtf(" ALG", "%u", alg + 1);
   fmtf(" FBK", "%u", fb);
   printf("\n");

   for(unsigned i = 0; i < NUM_OP; ++i)
   {
      op[i].print(i);
   }
}
