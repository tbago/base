// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/threading/platform_thread.h"

namespace base {

// static
PlatformThreadId PlatformThread::CurrentId() {
  return GetCurrentThreadId();
}

// static
void PlatformThread::YieldCurrentThread() {
  ::Sleep(0);
}

// static
void PlatformThread::Sleep(TimeDelta duration) {
  ::Sleep(duration.InMillisecondsRoundedUp());
}

// static
void PlatformThread::SetThreadPriority(PlatformThreadHandle handle,
                                       ThreadPriority priority) {
  switch (priority) {
    case kThreadPriority_Normal:
      ::SetThreadPriority(handle, THREAD_PRIORITY_NORMAL);
      break;
    case kThreadPriority_RealtimeAudio:
      ::SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
      break;
  }
}

}  // namespace base
