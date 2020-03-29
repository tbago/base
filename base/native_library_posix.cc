// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/native_library.h"

#include <dlfcn.h>
#include "base/logging.h"

namespace base {

// static
NativeLibrary LoadNativeLibrary(const char * library_path,
                                std::string& error) {

  // We deliberately do not use RTLD_DEEPBIND.  For the history why, please
  // refer to the bug tracker.  Some useful bug reports to read include:
  // http://crbug.com/17943, http://crbug.com/17557, http://crbug.com/36892,
  // and http://crbug.com/40794.
  void* dl = dlopen(library_path, RTLD_LAZY);
  if (NULL == dl) {
    error = dlerror();
  }
  return dl;
}

// static
void UnloadNativeLibrary(NativeLibrary library) {
  int ret = dlclose(library);
  if (ret < 0) {
    DLOG(ERROR) << "dlclose failed: " << dlerror();
    NOTREACHED();
  }
}

// static
void* GetFunctionPointerFromNativeLibrary(NativeLibrary library,
                                          const char* name) {
  return dlsym(library, name);
}

// static
std::string GetNativeLibraryName(const std::string& name) {
  return name + ".dylib";
}

}  // namespace base
