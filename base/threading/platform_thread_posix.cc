// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/threading/platform_thread.h"

#include <errno.h>
#include <sched.h>

#if defined(OS_MACOSX)
#include <sys/resource.h>
#include <algorithm>
#endif


namespace base {

// static
PlatformThreadId PlatformThread::CurrentId() {
  // Pthreads doesn't have the concept of a thread ID, so we have to reach down
  // into the kernel.
#if defined(OS_MACOSX)
  return pthread_mach_thread_np(pthread_self());
#elif defined(OS_LINUX)
  return syscall(__NR_gettid);
#elif defined(OS_ANDROID)
  return gettid();
#elif defined(OS_SOLARIS)
  return pthread_self();
#elif defined(OS_NACL) && defined(__GLIBC__)
  return pthread_self();
#elif defined(OS_NACL) && !defined(__GLIBC__)
  // Pointers are 32-bits in NaCl.
  return reinterpret_cast<int32>(pthread_self());
#elif defined(OS_POSIX)
  return reinterpret_cast<int64>(pthread_self());
#endif
}

// static
void PlatformThread::YieldCurrentThread() {
  sched_yield();
}

// static
void PlatformThread::Sleep(TimeDelta duration) {
  struct timespec sleep_time, remaining;

  // Break the duration into seconds and nanoseconds.
  // NOTE: TimeDelta's microseconds are int64s while timespec's
  // nanoseconds are longs, so this unpacking must prevent overflow.
  sleep_time.tv_sec = duration.InSeconds();
  duration -= TimeDelta::FromSeconds(sleep_time.tv_sec);
  sleep_time.tv_nsec = duration.InMicroseconds() * 1000;  // nanoseconds

  while (nanosleep(&sleep_time, &remaining) == -1 && errno == EINTR)
    sleep_time = remaining;
}
}  // namespace base
