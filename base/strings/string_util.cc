#include "string_util.h"
#include "base/basictypes.h"

namespace base {

std::vector<std::string> StringSplit( const std::string& input, const std::string& separet_chars )
{
    std::vector<std::string> r;
    const char* p = input.c_str();
    const char* pc = NULL;
    const char* q = NULL;
    uint32 len = 0;
    while(*p != NULL)
    {
        pc = separet_chars.c_str();
        while(*pc != NULL)
        {
            if (*pc == *p) {
                break;
            }
            ++pc;
        }
        if (*pc != NULL)         //need delete char
        {
            if (len != 0) {
                std::string tmp;
                tmp.append(q, len);
                r.push_back(tmp);
            }
            ++p;
            len = 0;
        }
        else         //not need delete
        {
            if (len == 0) {
                q = p;
            }
            len++;
            ++p;
        }
    }
    if (len != 0)
    {
        std::string tmp;
        tmp.append(q, len);
        r.push_back(tmp);
    }
    return r;
}

}       //namespace base
