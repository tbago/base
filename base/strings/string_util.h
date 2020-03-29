// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file defines utility functions for working with strings.

#ifndef BASE_STRINGS_STRING_UTIL_H_
#define BASE_STRINGS_STRING_UTIL_H_


#include <string>
#include <vector>
#include "base/base_export.h"

namespace base {

// Compares up to count characters of s1 and s2 without regard to case using
// the current locale; returns 0 if they are equal, 1 if s1 > s2, and -1 if
// s2 > s1 according to a lexicographic comparison.
BASE_EXPORT
int strncasecmp(const char* s1, const char* s2, size_t count);

BASE_EXPORT
std::vector<std::string> StringSplit(const std::string& input, const std::string& separet_chars);

///将字符串转换成大写字符串
BASE_EXPORT
std::string UppercaseString(const std::string& input);

}       //namespace base
#endif      //BASE_STRINGS_STRING_UTIL_H_
