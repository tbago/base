#include "base/file_util.h"
#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "base/win/win_util.h"
#include "base/files/file_path.h"
#include "base/utf_string_conversions.h"
#include "base/strings/string_util.h"

#include <windows.h>

namespace file_util {

BASE_EXPORT bool PathOrFileExists( const std::string& path )
{
    std::wstring wstr_path = UTF8ToWide(path.c_str(), path.length());
    return (::GetFileAttributesW(wstr_path.c_str()) != INVALID_FILE_ATTRIBUTES);
}

bool DirectoryExists(const std::string& path)
{
    std::wstring wstr_path = UTF8ToWide(path.c_str(), path.length());
    DWORD fileattr = GetFileAttributesW(wstr_path.c_str());
    if (fileattr != INVALID_FILE_ATTRIBUTES) {
        return (fileattr & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }
    return false;
}

BASE_EXPORT std::string GetExecuteDirectoryWithSeparator()
{
    wchar_t exe_buf[4096] = L"\0";
    DWORD size = ::GetModuleFileNameW(NULL, (LPWSTR)exe_buf, 4095);
    if (size == 0) {
        return "";
    }
    exe_buf[size]=L'\0';

    std::string file_dir = base::FilePath::ExtraFileDirectory(WideToUTF8(exe_buf));
    file_util::EnsureEndsWithSeparator(file_dir);
    return file_dir;
}

//////////////////////////////enum files////////////////////////////////////////////
struct find_data
{
    WIN32_FIND_DATAW    data;
    std::string         path;
    HANDLE              find_hd;

    find_data()
    {
        find_hd = INVALID_HANDLE_VALUE;
    }
    ~find_data()
    {
        if (find_hd != INVALID_HANDLE_VALUE) {
            FindClose(find_hd);
            find_hd = INVALID_HANDLE_VALUE;
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

    std::wstring find_path = UTF8ToWide(pData->path.c_str(), pData->path.length());
    find_path += L"*.*";

    pData->find_hd = FindFirstFileW(find_path.c_str(), &pData->data);
    if (pData->find_hd == INVALID_HANDLE_VALUE) {
        LOG(ERROR)<<"FindFirstFile failed:%d"<<GetLastError();
        delete pData;
        return NULL;
    }
    for (int i=0; i<2; ++i)      //jump the two special file
    {
        std::wstring name = pData->data.cFileName;
        if (name == L"." || name == L"..") {
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
        LOG(WARNING)<<"no file can be found:"<<path.c_str();
        return NULL;
    }
}

bool find_next(find_data* pFindData)
{
    find_data* pData = (find_data*)pFindData;
    if (FindNextFileW(pData->find_hd, &pData->data)) {
        return true;
    }
    else {
        LOG(WARNING)<<"FindNextFile failed:"<<GetLastError();
        return false;
    }
}

bool find_is_folder(find_data* pFindData)
{
    if (pFindData->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }
    else return false;
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

    std::wstring tmp_wstr = pData->data.cFileName;
    std::string name = WideToUTF8(tmp_wstr);

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
            for (uint32 i =0;i < file_ext.size(); i++)     //if it's match the pattern
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

}       //namespace file_util
