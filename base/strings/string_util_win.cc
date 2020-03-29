#include "string_util.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>


namespace base {

int strncasecmp(const char* s1, const char* s2, size_t count)
{
    return _strnicmp(s1, s2, count);
}

std::string UppercaseString( const std::string& input )
{
    std::string return_str = input;
    strupr((char*)return_str.c_str());
    return return_str;
}

}       //namespace base
