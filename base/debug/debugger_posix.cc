// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/debug/debugger.h"
#include "base/build_config.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#if defined(__GLIBCXX__)
#include <cxxabi.h>
#endif

#if defined(OS_MACOSX)
#include <AvailabilityMacros.h>
#endif

#if defined(OS_MACOSX) || defined(OS_BSD)
#include <sys/sysctl.h>
#endif
#include <ostream>

#include "base/basictypes.h"
#include "base/logging.h"
//#include "base/memory/scoped_ptr.h"
//#include "base/posix/eintr_wrapper.h"
//#include "base/safe_strerror_posix.h"
//#include "base/stringprintf.h"
#include "base/strings/string_piece.h"

namespace base {
namespace debug {

bool SpawnDebuggerOnProcess(unsigned process_id)
{
//  const std::string debug_cmd =
//      StringPrintf("xterm -e 'gdb --pid=%u' &", process_id);
//  LOG(WARNING) << "Starting debugger on pid " << process_id
//               << " with command `" << debug_cmd << "`";
//  int ret = system(debug_cmd.c_str());
//  if (ret == -1)
//    return false;
  return true;
}

// Based on Apple's recommended method as described in
// http://developer.apple.com/qa/qa2004/qa1361.html
bool BeingDebugged() {
  // NOTE: This code MUST be async-signal safe (it's used by in-process
  // stack dumping signal handler). NO malloc or stdio is allowed here.
  //
  // While some code used below may be async-signal unsafe, note how
  // the result is cached (see |is_set| and |being_debugged| static variables
  // right below). If this code is properly warmed-up early
  // in the start-up process, it should be safe to use later.

  // If the process is sandboxed then we can't use the sysctl, so cache the
  // value.
  static bool is_set = false;
  static bool being_debugged = false;

  if (is_set)
    return being_debugged;

  // Initialize mib, which tells sysctl what info we want.  In this case,
  // we're looking for information about a specific process ID.
  int mib[] = {
    CTL_KERN,
    KERN_PROC,
    KERN_PROC_PID,
    getpid()
#if defined(OS_OPENBSD)
    , sizeof(struct kinfo_proc),
    0
#endif
  };

  // Caution: struct kinfo_proc is marked __APPLE_API_UNSTABLE.  The source and
  // binary interfaces may change.
  struct kinfo_proc info;
  size_t info_size = sizeof(info);

#if defined(OS_OPENBSD)
  if (sysctl(mib, arraysize(mib), NULL, &info_size, NULL, 0) < 0)
    return -1;

  mib[5] = (info_size / sizeof(struct kinfo_proc));
#endif

  int sysctl_result = sysctl(mib, arraysize(mib), &info, &info_size, NULL, 0);
  DCHECK_EQ(sysctl_result, 0);
  if (sysctl_result != 0) {
    is_set = true;
    being_debugged = false;
    return being_debugged;
  }

  // This process is being debugged if the P_TRACED flag is set.
  is_set = true;
#if defined(OS_FREEBSD)
  being_debugged = (info.ki_flag & P_TRACED) != 0;
#elif defined(OS_BSD)
  being_debugged = (info.p_flag & P_TRACED) != 0;
#else
  being_debugged = (info.kp_proc.p_flag & P_TRACED) != 0;
#endif
  return being_debugged;
}

// We want to break into the debugger in Debug mode, and cause a crash dump in
// Release mode. Breakpad behaves as follows:
//
// +-------+-----------------+-----------------+
// | OS    | Dump on SIGTRAP | Dump on SIGABRT |
// +-------+-----------------+-----------------+
// | Linux |       N         |        Y        |
// | Mac   |       Y         |        N        |
// +-------+-----------------+-----------------+
//
// Thus we do the following:
// Linux: Debug mode, send SIGTRAP; Release mode, send SIGABRT.
// Mac: Always send SIGTRAP.

#if defined(NDEBUG) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
#define DEBUG_BREAK() abort()
#else
#define DEBUG_BREAK() asm("int3")
#endif

void BreakDebugger() {
  // NOTE: This code MUST be async-signal safe (it's used by in-process
  // stack dumping signal handler). NO malloc or stdio is allowed here.

  DEBUG_BREAK();
}

}  // namespace debug
}  // namespace base
