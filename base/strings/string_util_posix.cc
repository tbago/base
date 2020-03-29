#include "string_util.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

namespace base {

int strncasecmp(const char* string1, const char* string2, size_t count)
{
    return ::strncasecmp(string1, string2, count);
}

static char* strupr(char* ps)
{
  char* p=ps;
  while(*p) {
    if (*p >= 'a' && *p <= 'z') {
       *p = *p - 'a' + 'A';
    }
    ++p;
  }
  return ps;
}

std::string UppercaseString( const std::string& input )
{
    std::string return_str = input;
    strupr((char*)return_str.c_str());
    return return_str;
}

}       //namesapce base
