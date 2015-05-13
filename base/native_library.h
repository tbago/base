// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_NATIVE_LIBRARY_H_
#define BASE_NATIVE_LIBRARY_H_

// This file defines a cross-platform "NativeLibrary" type which represents
// a loadable module.

#include "base/base_export.h"
#include "base/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif  // OS_*

#include <string>

// Macro useful for writing cross-platform function pointers.
#if defined(OS_WIN) && !defined(CDECL)
#define CDECL __cdecl
#else
#define CDECL
#endif

namespace base {
#if defined(OS_WIN)
typedef HMODULE NativeLibrary;
#elif defined(OS_POSIX)
typedef void* NativeLibrary;
#endif  // OS_*

// Loads a native library from disk.  Release it with UnloadNativeLibrary when
// you're done.  Returns NULL on failure.
// If |err| is not NULL, it may be filled in with an error message on
// error.
//library path is utf8 charset
BASE_EXPORT NativeLibrary LoadNativeLibrary(const char* library_path,
                                            std::string& error);

#if defined(OS_WIN)
// Loads a native library from disk.  Release it with UnloadNativeLibrary when
// you're done.
// This function retrieves the LoadLibrary function exported from kernel32.dll
// and calls it instead of directly calling the LoadLibrary function via the
// import table.
BASE_EXPORT NativeLibrary LoadNativeLibraryDynamically(
    const char * library_path);
#endif  // OS_WIN

// Unloads a native library.
BASE_EXPORT void UnloadNativeLibrary(NativeLibrary library);

// Gets a function pointer from a native library.
BASE_EXPORT void* GetFunctionPointerFromNativeLibrary(NativeLibrary library,
                                                      const char* name);

// Returns the full platform specific name for a native library.
// For example:
// "mylib" returns "mylib.dll" on Windows, "libmylib.so" on Linux,
// "mylib.dylib" on Mac.
BASE_EXPORT std::string GetNativeLibraryName(const std::string& name);

}  // namespace base

#endif  // BASE_NATIVE_LIBRARY_H_
