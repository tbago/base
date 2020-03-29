#ifndef BASE_FILE_UTIL_H_
#define BASE_FILE_UTIL_H_

#include "base/build_config.h"
#include "base/base_export.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <string>
#include <vector>

namespace file_util {

//-----------------------------------------------------------------------------
// Functions that operate purely on a path string w/o touching the filesystem:

// Returns true if the given path ends with a path separator character.
BASE_EXPORT bool EndsWithSeparator(std::string& path);

// Makes sure that |path| ends with a separator IFF path is a directory that
// exists. Returns true if |path| is an existing directory, false otherwise.
BASE_EXPORT bool EnsureEndsWithSeparator(std::string& path);

// Returns true if the given path or file exists on the local filesystem,
// false otherwise.
BASE_EXPORT bool PathOrFileExists(const std::string& path);

// Returns true if the given path exists and is a directory, false otherwise.
BASE_EXPORT bool DirectoryExists(const std::string& path);

//Returns the execute directory end with separator.
BASE_EXPORT std::string GetExecuteDirectoryWithSeparator();

//Enum files from directory,support recursion enum, support file extersion filter.
BASE_EXPORT void EnumFilesFromDirectory( std::string &directory,
                                        bool recursion,
                                        std::vector<std::string> &file_ext,
                                        std::vector<std::string> &ret_list);
}  // namespace file_util

#endif  // BASE_FILE_UTIL_H_
