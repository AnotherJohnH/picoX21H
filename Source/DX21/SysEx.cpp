//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
