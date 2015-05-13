// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// WARNING: You should *NOT* be using this class directly.  PlatformThread is
// the low-level platform-specific abstraction to the OS's threading interface.
// You should instead be using a message-loop driven Thread, see thread.h.

#ifndef BASE_THREADING_PLATFORM_THREAD_H_
#define BASE_THREADING_PLATFORM_THREAD_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/time.h"
#include "base/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <pthread.h>
#include <unistd.h>
#endif

namespace base {

// PlatformThreadHandle should not be assumed to be a numeric type, since the
// standard intends to allow pthread_t to be a structure.  This means you
// should not initialize it to a value, like 0.  If it's a member variable, the
// constructor can safely "value initialize" using () in the initializer list.
#if defined(OS_WIN)
typedef DWORD PlatformThreadId;
typedef void* PlatformThreadHandle;  // HANDLE
const PlatformThreadHandle kNullThreadHandle = NULL;
#elif defined(OS_POSIX)
typedef pthread_t PlatformThreadHandle;
const PlatformThreadHandle kNullThreadHandle = 0;
typedef pid_t PlatformThreadId;
#endif

const PlatformThreadId kInvalidThreadId = 0;

// Valid values for SetThreadPriority()
enum ThreadPriority{
  kThreadPriority_Normal,
  // Suitable for low-latency, glitch-resistant audio.
  kThreadPriority_RealtimeAudio
};

// A namespace for low-level thread functions.
class BASE_EXPORT PlatformThread {
 public:
  // Gets the current thread id, which may be useful for logging purposes.
  static PlatformThreadId CurrentId();

  // Yield the current thread so another thread can be scheduled.
  static void YieldCurrentThread();

  // Sleeps for the specified duration.
  static void Sleep(base::TimeDelta duration);

  // Sets the priority of the thread specified in |handle| to |priority|.
  // This does not work on Linux, use CreateWithPriority() instead.
  static void SetThreadPriority(PlatformThreadHandle handle,
                                ThreadPriority priority);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(PlatformThread);
};

}  // namespace base

#endif  // BASE_THREADING_PLATFORM_THREAD_H_
