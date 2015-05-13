#ifndef BASE_WIN_WIN_UTIL_H_
#define BASE_WIN_WIN_UTIL_H_

#include "base/basictypes.h"
#include "base/base_export.h"
#include <string>

namespace base {
namespace win {

BASE_EXPORT std::string ErrCodeToMessage(uint32 err_code);
}  // namespace win
}  // namespace base

#endif  // BASE_WIN_WIN_UTIL_H_