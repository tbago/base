#include "win_util.h"
#include <windows.h>

namespace base {
namespace win {

BASE_EXPORT std::string ErrCodeToMessage(uint32 err_code)
{
    std::string str_err_ret;
    //Buffer that gets the error message string
    HLOCAL hlocal = NULL;
    DWORD dwErr = err_code;

    DWORD format_ret = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                       NULL, dwErr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR)&hlocal,0,NULL);
    if (hlocal != NULL && format_ret > 0)
    {
        str_err_ret = (const char *)LocalLock(hlocal);
        LocalFree(hlocal);
    }
    return str_err_ret;
}

}       //namespace win
}       //namespace base
