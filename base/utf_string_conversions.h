// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_UTF_STRING_CONVERSIONS_H_
#define BASE_UTF_STRING_CONVERSIONS_H_

#include <string>

#include "base/base_export.h"

// These convert between UTF-8, -16, and -32 strings. They are potentially slow,
// so avoid unnecessary conversions. The low-level versions return a boolean
// indicating whether the conversion was 100% valid. In this case, it will still
// do the best it can and put the result in the output buffer. The versions that
// return strings ignore this error and just return the best conversion
// possible.
BASE_EXPORT bool WideToUTF8(const wchar_t* src, size_t src_len,
                            std::string* output);
BASE_EXPORT std::string WideToUTF8(const std::wstring& wide);
BASE_EXPORT bool UTF8ToWide(const char* src, size_t src_len,
                            std::wstring* output);
BASE_EXPORT std::wstring UTF8ToWide(const char* src, size_t src_len);

// These convert an ASCII string, typically a hardcoded constant, to a
//Wide string.
BASE_EXPORT std::wstring ASCIIToWide(const char * src, size_t src_len);
BASE_EXPORT std::string WideToASCII(const wchar_t* src, size_t src_len);

#endif  // BASE_UTF_STRING_CONVERSIONS_H_
