#include "base/file_util.h"
#include <unistd.h>
#include <mach-o/dyld.h>
#include <sys/types.h>
#include <dirent.h>
#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/strings/string_util.h"

namespace file_util {

BASE_EXPORT bool PathOrFileExists( const std::string& path )
{
    return access(path.c_str(), F_OK) == 0;
}

BASE_EXPORT bool DirectoryExists(const std::string& path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0) {
		return false;
	}
	if (!S_ISDIR(info.st_mode)) {
		return false;
	}
	return true;
}

BASE_EXPORT std::string GetExecuteDirectoryWithSeparator()
{
    char buf[4*PATH_MAX]={0};
    std::string ret_str;
    typedef int (*NSGetExecutablePathProcPtr)(char *buf, size_t *bufsize);
    NSGetExecutablePathProcPtr NSGetExecutablePath = NULL;
    NSGetExecutablePath = (NSGetExecutablePathProcPtr) NSAddressOfSymbol(NSLookupAndBindSymbol("__NSGetExecutablePath"));
    if (NSGetExecutablePath != NULL)
    {
        size_t pathLength = 4*PATH_MAX;
        (*NSGetExecutablePath)(buf, &pathLength);
        buf[4*PATH_MAX-1] = 0;
    }
    ret_str = buf;
    ret_str = base::FilePath::ExtraFileDirectory(ret_str);
    file_util::EnsureEndsWithSeparator(ret_str);
    return ret_str;
}

struct find_data
{
    struct dirent * data;
    std::string path;
    DIR* find_hd;
    find_data()
    {
        find_hd = NULL;
    }
    ~find_data()
    {
        if (find_hd != NULL) {
            closedir(find_hd);
            find_hd = NULL;
        }
    }
};

bool find_next(find_data* pFindData);
std::string find_get_path(find_data* pFindData);

find_data* find_first(std::string &path)
{
    find_data* pData = new find_data();

    pData->path = path;

    bool bOK = true;

    std::string err_str = "No file can be found, path = ";
    err_str += pData->path;

    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        //LOG(ERROR)<<"opendir failed"<<strerror(errno)).c_str();
        delete pData;
        return NULL;
    }
    pData->data = NULL;
    pData->find_hd = dir;
    if (!find_next(pData)) { //find the first file
        delete pData;
        //LOG(WARNING)<<"no file can be found";
        return NULL;
    }
    for (int i=0; i < 2; ++i) //jump the two special file
    {
        std::string name = pData->data->d_name;
        if (name == "." || name == "..")
        {
            if (!find_next(pData)) {
                bOK = false;
                break;
            }
        } else break;
    }
    if (bOK) {
        return pData;
    }
    else {
        delete pData;
        //LOG(WARNING)<<"no file can be found";
        return NULL;
    }
}

bool find_next(find_data* pFindData)
{
    find_data* pData = (find_data*)pFindData;
    do {
        pData->data = readdir(pData->find_hd);
        if (pData->data == NULL) {
            //LOG(ERROR)<<"read dir failed";
            return false;
        }
        else {
            break;
        }
    } while (true);
    return true;
}

bool find_is_folder(find_data* pFindData)
{
    return (pFindData->data->d_type & DT_DIR);
}

void find_close(find_data* pFindData)
{
    if (pFindData != NULL) {
        delete pFindData;
    }
}

std::string find_get_path(find_data* pFindData)
{
    find_data* pData = (find_data*)pFindData;
    std::string name = pData->data->d_name;
    return pData->path + name;
}

BASE_EXPORT void EnumFilesFromDirectory( std::string &directory,
                                        bool recursion,
                                        std::vector<std::string> &file_ext,
                                        std::vector<std::string> &ret_list )
{
    find_data *pfind_data = find_first(directory);
    do
    {
        if (NULL == pfind_data) {
            LOG(WARNING)<<"no file found in path:"<<directory.c_str();
            return;
        }
        std::string full_path = find_get_path(pfind_data);
        if (!find_is_folder(pfind_data))
        {
            std::string src_ext = base::FilePath::ExtraFileExtersion(full_path);
            for (int i =0;i < file_ext.size(); i++)     //if it's match the pattern
            {
                std::string tmp_ext = file_ext[i];
                if (0 == base::strncasecmp(src_ext.c_str() ,tmp_ext.c_str(), tmp_ext.length())) {
                    ret_list.push_back(full_path);
                    break;
                }
            }
        }
        else if (recursion)
        {
            EnsureEndsWithSeparator(full_path);
            EnumFilesFromDirectory(full_path, recursion, file_ext, ret_list);
        }
    } while(find_next(pfind_data));
    find_close(pfind_data);
}

}   //namespace file_util