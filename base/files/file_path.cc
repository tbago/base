// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/files/file_path.h"
#include "base/basictypes.h"

namespace base {

#if defined(FILE_PATH_USES_WIN_SEPARATORS)
const char FilePath::kSeparators[] = "\\/";
#else  // FILE_PATH_USES_WIN_SEPARATORS
const char FilePath::kSeparators[] = "/";
#endif  // FILE_PATH_USES_WIN_SEPARATORS


bool FilePath::IsSeparator( char character )
{
    for (size_t i = 0; i < arraysize(kSeparators) - 1; ++i)
    {
        if (character == kSeparators[i]) {
            return true;
        }
    }
    return false;
}

std::string FilePath::ExtraFileDirectory( const std::string &file_path )
{
    std::string ret_str = file_path;
    std::string::size_type index = file_path.find_last_of(kSeparators, std::string::npos,
                                                        arraysize(kSeparators) - 1);
    if (index != std::string::npos) {
        ret_str = file_path.substr(0, index+1);
    }
    return ret_str;
}

std::string FilePath::ExtraFileNameWithExtersion( const std::string &file_path )
{
    std::string ret_str = file_path;
    std::string::size_type index = file_path.find_last_of(kSeparators, std::string::npos,
        arraysize(kSeparators) - 1);
    if (index != std::string::npos) {
        ret_str = file_path.substr(index+1);
    }
    return ret_str;
}

std::string FilePath::ExtraFileNameWithoutExtersion( const std::string &file_path )
{
    std::string ret_str = ExtraFileNameWithExtersion(file_path);
    std::string::size_type index = ret_str.rfind(".");
    if (index != std::string::npos) {
        ret_str = ret_str.substr(0, index);
    }
    return ret_str;
}

std::string FilePath::ExtraFileExtersion( const std::string &file_path )
{
    std::string ret_str;
    std::string::size_type index = file_path.rfind(".");
    if (index != std::string::npos) {
        ret_str = file_path.substr(index + 1);
    }
    return ret_str;
}

}  // namespace base

