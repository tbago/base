// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/native_library.h"
#include "base/utf_string_conversions.h"
#include "base/win/win_util.h"

namespace base {

typedef HMODULE (WINAPI* LoadLibraryFunction)(const wchar_t* file_name);

NativeLibrary LoadNativeLibraryHelper(const char * library_path,
                                      LoadLibraryFunction load_library_api)
{
  // LoadLibrary() opens the file off disk.
  std::wstring wstr_library_path = UTF8ToWide(library_path, strlen(library_path));
  HMODULE module = (*load_library_api)(wstr_library_path.c_str());
  return module;
}

// static
NativeLibrary LoadNativeLibrary(const char * library_path,
                                std::string& error) {
  NativeLibrary library_handle = LoadNativeLibraryHelper(library_path, LoadLibraryW);
  if (NULL == library_handle) {
      error = base::win::ErrCodeToMessage(GetLastError());
  }
  return library_handle;
}

NativeLibrary LoadNativeLibraryDynamically(const char * library_path) {
  typedef HMODULE (WINAPI* LoadLibraryFunction)(const wchar_t* file_name);

  LoadLibraryFunction load_library;
  load_library = reinterpret_cast<LoadLibraryFunction>(
      GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"));

  return LoadNativeLibraryHelper(library_path, load_library);
}

// static
void UnloadNativeLibrary(NativeLibrary library) {
  FreeLibrary(library);
}

// static
void* GetFunctionPointerFromNativeLibrary(NativeLibrary library,
                                          const char* name) {
  return GetProcAddress(library, name);
}

// static
std::string GetNativeLibraryName(const std::string& name) {
  return name + ".dll";
}

}  // namespace base
