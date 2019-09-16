// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/utf_string_conversions.h"
#include "base/logging.h"
#ifdef OS_WIN
#include <windows.h>

BASE_EXPORT bool WideToUTF8( const wchar_t* src, size_t src_len, std::string* output )
{
    UNREFERENCED_PARAMETER(src_len);
    if (NULL == output) {
        LOG(WARNING)<<"not output found";
        return false;
    }
    int char_len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)src, -1, NULL, 0, NULL, NULL);
    if (char_len == 0) {
        *output = "";
        LOG(FATAL)<<"cannot convert to utf8 char:"<<GetLastError();
        return false;
    }
    char* char_txt = new char[char_len+1];
    if (WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)src, -1, char_txt, char_len, NULL, NULL) == 0)
    {
        delete []char_txt;
        *output = "";
        LOG(FATAL)<<"cannot convert to utf8 char:"<<GetLastError();
        return false;
    }
    char_txt[char_len] = '\0';
    *output = char_txt;
    delete []char_txt;
    return true;
}

BASE_EXPORT std::string WideToUTF8( const std::wstring& wide )
{
    std::string ret;
    // Ignore the success flag of this call, it will do the best it can for
    // invalid input, which is what we want here.
    WideToUTF8(wide.data(), wide.length(), &ret);
    return ret;
}

BASE_EXPORT bool UTF8ToWide( const char* src, size_t src_len, std::wstring* output )
{
    UNREFERENCED_PARAMETER(src_len);
    if (NULL == output) {
        LOG(WARNING)<<"not output found";
        return false;
    }
    int  wchar_len = ::MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );
    if (wchar_len == 0) {
        LOG(FATAL)<<"cannot converter to wide char:"<<GetLastError();
        *output=L"";
        return false;
    }
    wchar_t* wchar_txt = NULL;
    wchar_txt = new  wchar_t[wchar_len+1];
    memset(wchar_txt,0,(wchar_len+1)*sizeof(wchar_t));
    if (::MultiByteToWideChar( CP_UTF8, 0, src, -1, (LPWSTR)wchar_txt, wchar_len) == 0)
    {
        delete []wchar_txt;
        *output=L"";
        LOG(FATAL)<<"cannot convert to wide char:"<<GetLastError();
        return false;
    }
    *output = wchar_txt;
    delete []wchar_txt;
    return true;
}

BASE_EXPORT std::wstring UTF8ToWide( const char* src, size_t src_len )
{
    std::wstring ret;
    UTF8ToWide(src, src_len, &ret);
    return ret;
}

BASE_EXPORT std::wstring ASCIIToWide( const char * src, size_t src_len )
{
    UNREFERENCED_PARAMETER(src_len);
    std::wstring ret_wstr = L"";
    int  wchar_len = ::MultiByteToWideChar( CP_ACP, 0, src, -1, NULL, 0 );
    if (wchar_len == 0) {
        LOG(FATAL)<<"cannot converter to wide char:"<<GetLastError();
        return ret_wstr;
    }
    wchar_t* wchar_txt = NULL;
    wchar_txt = new  wchar_t[wchar_len+1];
    memset(wchar_txt,0,(wchar_len+1)*sizeof(wchar_t));
    if (::MultiByteToWideChar( CP_ACP, 0, src, -1, (LPWSTR)wchar_txt, wchar_len) == 0)
    {
        delete []wchar_txt;
        LOG(FATAL)<<"cannot convert to wide char:"<<GetLastError();
        return ret_wstr;
    }
    ret_wstr = wchar_txt;
    delete []wchar_txt;
    return ret_wstr;
}

BASE_EXPORT std::string WideToASCII( const wchar_t* src, size_t src_len )
{
    UNREFERENCED_PARAMETER(src_len);
    std::string ret_str = "";
    int char_len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)src, -1, NULL, 0, NULL, NULL);
    if (char_len == 0) {
        LOG(FATAL)<<"cannot converter to wide char:"<<GetLastError();
        return "";
    }
    char* char_txt = new char[char_len+1];
    if (WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)src, -1, char_txt, char_len, NULL, NULL) == 0)
    {
        delete []char_txt;
        assert(false);
        return "";
    }
    char_txt[char_len] = '\0';
    ret_str = char_txt;
    delete []char_txt;
    return ret_str;
}

#elif defined(OS_POSIX)
BASE_EXPORT bool WideToUTF8( const wchar_t* src, size_t src_len, std::string* output )
{
    LOG(FATAL)<<"not implement";
    return false;
}

BASE_EXPORT std::string WideToUTF8( const std::wstring& wide )
{
    std::string ret;
    // Ignore the success flag of this call, it will do the best it can for
    // invalid input, which is what we want here.
    WideToUTF8(wide.data(), wide.length(), &ret);
    return ret;
}

BASE_EXPORT bool UTF8ToWide( const char* src, size_t src_len, std::wstring* output )
{
    LOG(FATAL)<<"not implement";
    return false;
}

BASE_EXPORT std::wstring UTF8ToWide( const char* src, size_t src_len )
{
    std::wstring ret;
    UTF8ToWide(src, src_len, &ret);
    return ret;
}

BASE_EXPORT std::wstring ASCIIToWide( const char * src, size_t src_len )
{
    LOG(FATAL)<<"not implement";
    return L"";
}

BASE_EXPORT std::string WideToASCII( const wchar_t* src, size_t src_len )
{
    LOG(FATAL)<<"not implement";
    return "";
}

#endif
