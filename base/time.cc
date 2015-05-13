// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/time.h"

namespace base {

// TimeDelta ------------------------------------------------------------------

int TimeDelta::InDays() const {
  return static_cast<int>(delta_ / Time::kMicrosecondsPerDay);
}

int TimeDelta::InHours() const {
  return static_cast<int>(delta_ / Time::kMicrosecondsPerHour);
}

int TimeDelta::InMinutes() const {
  return static_cast<int>(delta_ / Time::kMicrosecondsPerMinute);
}

double TimeDelta::InSecondsF() const {
  return static_cast<double>(delta_) / Time::kMicrosecondsPerSecond;
}

int64 TimeDelta::InSeconds() const {
  return delta_ / Time::kMicrosecondsPerSecond;
}

double TimeDelta::InMillisecondsF() const {
  return static_cast<double>(delta_) / Time::kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMilliseconds() const {
  return delta_ / Time::kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMillisecondsRoundedUp() const {
  return (delta_ + Time::kMicrosecondsPerMillisecond - 1) /
      Time::kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMicroseconds() const {
  return delta_;
}

}  // namespace base
