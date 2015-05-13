#include "base/file_util.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"

namespace file_util
{

BASE_EXPORT bool EndsWithSeparator( std::string &path )
{
    if (0 == path.length()) {
        return false;
    }
    return base::FilePath::IsSeparator(path[path.length() - 1]);
}

BASE_EXPORT bool EnsureEndsWithSeparator( std::string &path )
{
    if (!DirectoryExists(path))
        return false;

    if (EndsWithSeparator(path))
        return true;

    path.append(&base::FilePath::kSeparators[0], 1);
    return true;
}

}       //namespace file_util
