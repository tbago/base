// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Time represents an absolute point in time, internally represented as
// microseconds (s/1,000,000) since the Windows epoch (1601-01-01 00:00:00 UTC)
// (See http://crbug.com/14734).  System-dependent clock interface routines are
// defined in time_PLATFORM.cc.
//
// TimeDelta represents a duration of time, internally represented in
// microseconds.
//
// TimeTicks represents an abstract time that is most of the time incrementing
// for use in measuring time durations. It is internally represented in
// microseconds.  It can not be converted to a human-readable time, but is
// guaranteed not to decrease (if the user changes the computer clock,
// Time::Now() may actually decrease or jump).  But note that TimeTicks may
// "stand still", for example if the computer suspended.
//
// These classes are represented as only a 64-bit value, so they can be
// efficiently passed by value.

#ifndef BASE_TIME_UTIL_H_
#define BASE_TIME_UTIL_H_

#include <time.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {

class Time;

// TimeDelta ------------------------------------------------------------------

class BASE_EXPORT TimeDelta {
 public:
  TimeDelta() : delta_(0) {
  }

  // Converts units of time to TimeDeltas.
  static TimeDelta FromDays(int64 days);
  static TimeDelta FromHours(int64 hours);
  static TimeDelta FromMinutes(int64 minutes);
  static TimeDelta FromSeconds(int64 secs);
  static TimeDelta FromMilliseconds(int64 ms);
  static TimeDelta FromMicroseconds(int64 us);

  // Converts an integer value representing TimeDelta to a class. This is used
  // when deserializing a |TimeDelta| structure, using a value known to be
  // compatible. It is not provided as a constructor because the integer type
  // may be unclear from the perspective of a caller.
  static TimeDelta FromInternalValue(int64 delta) {
    return TimeDelta(delta);
  }

  // Returns the internal numeric value of the TimeDelta object. Please don't
  // use this and do arithmetic on it, as it is more error prone than using the
  // provided operators.
  // For serializing, use FromInternalValue to reconstitute.
  int64 ToInternalValue() const {
    return delta_;
  }

#if defined(OS_POSIX)
  struct timespec ToTimeSpec() const;
#endif

  // Returns the time delta in some unit. The F versions return a floating
  // point value, the "regular" versions return a rounded-down value.
  //
  // InMillisecondsRoundedUp() instead returns an integer that is rounded up
  // to the next full millisecond.
  int InDays() const;
  int InHours() const;
  int InMinutes() const;
  double InSecondsF() const;
  int64 InSeconds() const;
  double InMillisecondsF() const;
  int64 InMilliseconds() const;
  int64 InMillisecondsRoundedUp() const;
  int64 InMicroseconds() const;

  TimeDelta& operator=(TimeDelta other) {
    delta_ = other.delta_;
    return *this;
  }

  // Computations with other deltas.
  TimeDelta operator+(TimeDelta other) const {
    return TimeDelta(delta_ + other.delta_);
  }
  TimeDelta operator-(TimeDelta other) const {
    return TimeDelta(delta_ - other.delta_);
  }

  TimeDelta& operator+=(TimeDelta other) {
    delta_ += other.delta_;
    return *this;
  }
  TimeDelta& operator-=(TimeDelta other) {
    delta_ -= other.delta_;
    return *this;
  }
  TimeDelta operator-() const {
    return TimeDelta(-delta_);
  }

  // Computations with ints, note that we only allow multiplicative operations
  // with ints, and additive operations with other deltas.
  TimeDelta operator*(int64 a) const {
    return TimeDelta(delta_ * a);
  }
  TimeDelta operator/(int64 a) const {
    return TimeDelta(delta_ / a);
  }
  TimeDelta& operator*=(int64 a) {
    delta_ *= a;
    return *this;
  }
  TimeDelta& operator/=(int64 a) {
    delta_ /= a;
    return *this;
  }
  int64 operator/(TimeDelta a) const {
    return delta_ / a.delta_;
  }

  // Comparison operators.
  bool operator==(TimeDelta other) const {
    return delta_ == other.delta_;
  }
  bool operator!=(TimeDelta other) const {
    return delta_ != other.delta_;
  }
  bool operator<(TimeDelta other) const {
    return delta_ < other.delta_;
  }
  bool operator<=(TimeDelta other) const {
    return delta_ <= other.delta_;
  }
  bool operator>(TimeDelta other) const {
    return delta_ > other.delta_;
  }
  bool operator>=(TimeDelta other) const {
    return delta_ >= other.delta_;
  }

 private:
  friend class Time;
  friend TimeDelta operator*(int64 a, TimeDelta td);

  // Constructs a delta given the duration in microseconds. This is private
  // to avoid confusion by callers with an integer constructor. Use
  // FromSeconds, FromMilliseconds, etc. instead.
  explicit TimeDelta(int64 delta_us) : delta_(delta_us) {
  }

  // Delta in microseconds.
  int64 delta_;
};

// Time -----------------------------------------------------------------------

// Represents a wall clock time.
class BASE_EXPORT Time {
 public:
  static const int64 kMillisecondsPerSecond = 1000;
  static const int64 kMicrosecondsPerMillisecond = 1000;
  static const int64 kMicrosecondsPerSecond = kMicrosecondsPerMillisecond *
                                              kMillisecondsPerSecond;
  static const int64 kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
  static const int64 kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
  static const int64 kMicrosecondsPerDay = kMicrosecondsPerHour * 24;
  static const int64 kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
  static const int64 kNanosecondsPerMicrosecond = 1000;
  static const int64 kNanosecondsPerSecond = kNanosecondsPerMicrosecond *
                                             kMicrosecondsPerSecond;
};

// static
inline TimeDelta TimeDelta::FromDays(int64 days) {
    return TimeDelta(days * Time::kMicrosecondsPerDay);
}

// static
inline TimeDelta TimeDelta::FromHours(int64 hours) {
    return TimeDelta(hours * Time::kMicrosecondsPerHour);
}

// static
inline TimeDelta TimeDelta::FromMinutes(int64 minutes) {
    return TimeDelta(minutes * Time::kMicrosecondsPerMinute);
}

// static
inline TimeDelta TimeDelta::FromSeconds(int64 secs) {
    return TimeDelta(secs * Time::kMicrosecondsPerSecond);
}

// static
inline TimeDelta TimeDelta::FromMilliseconds(int64 ms) {
    return TimeDelta(ms * Time::kMicrosecondsPerMillisecond);
}

// static
inline TimeDelta TimeDelta::FromMicroseconds(int64 us) {
    return TimeDelta(us);
}

inline TimeDelta operator*(int64 a, TimeDelta td) {
    return TimeDelta(a * td.delta_);
}

}  // namespace base

#endif  // BASE_TIME_UTIL_H_
