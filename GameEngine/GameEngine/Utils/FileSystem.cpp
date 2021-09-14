
#include <filesystem>
#include <fstream>
#include "FileSystem.h"
#include "StringUtils.h"
#include "Debug.h"

#if defined(_WIN32)
  #include <windows.h>
  #include <direct.h>
  #define getcwd _getcwd
#elif defined(__APPLE__)
  #include <dirent.h>
  #include <sys/types.h>
  #include <unistd.h>
#endif

using namespace DrageEngine;

std::string DrageEngine::RelativeToAbsolutePath(const std::string &relativePath)
{
    std::string path(relativePath);
    std::string root(getcwd(NULL, 0));
    while (String::StartsWith(path, "../"))
    {
        path = path.substr(3, path.length() - 3);
        int lastSlash = (int)root.rfind('/');
        root = root.substr(0, lastSlash);
    }
    return root + "/" + path;
}

#if defined(_WIN32)
bool DrageEngine::GetFilesInDirectory(const std::string &directory, std::vector<std::string> &files, bool recursiveSearch)
{
    // Search for all files - append wildcard
    std::string searchDir = directory + "*";

    // Get first file in directory
    HANDLE handle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA data;
    handle = FindFirstFile(searchDir.c_str(), &data);

    if (handle == INVALID_HANDLE_VALUE)
    {
        ERROR("Could not open directory: " + directory);
        return false;
    }

    do
    {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (recursiveSearch)
            {
                std::string subDir = directory + data.cFileName + "/";

                if (subDir.at(subDir.length() - 2) != '.')
                    GetFilesInDirectory(subDir, files, true);
            }
        }
        else
        {
            files.push_back(directory + data.cFileName);
        }
    }
    while (FindNextFile(handle, &data) != 0);

    FindClose(handle);
    return true;
}
#endif

#if defined(__APPLE__)
bool DrageEngine::GetFilesInDirectory(const std::string &directory, std::vector<std::string> &files, bool recursiveSearch)
{
    std::string absolutePathToDirectory = RelativeToAbsolutePath(directory);
    
    DIR *dir = opendir(absolutePathToDirectory.c_str());
    if (dir == NULL)
    {
        ERROR("Could not open directory: " + directory);
        return false;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (recursiveSearch)
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                
                std::string subDir = directory + entry->d_name + "/";
                GetFilesInDirectory(subDir, files, true);
            }
        }
        else
        {
            files.push_back(directory + entry->d_name);
        }
    }
    
    closedir(dir);
    return true;
}
#endif

#if defined(_WIN32)
std::string DrageEngine::FindFilePath(const std::string &directory, const std::string &filename)
{
    ERROR("Not implemented");
    return NULL;
}
#endif

#if defined(__APPLE__)
std::string DrageEngine::FindFilePath(const std::string &directory, const std::string &filename)
{
    DIR *dir = opendir(directory.c_str());
    if (dir == NULL)
    {
        ERROR("Could not open directory: " + directory);
        return NULL;
    }
    
    std::string path = "";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            
            std::string subDir = directory + entry->d_name + "/";
            path = FindFilePath(subDir, filename);
        }
        else
        {
            if (entry->d_name == filename)
            {
                path = directory + entry->d_name;
                break;
            }
        }
    }
    
    closedir(dir);
    return path;
}
#endif

std::string DrageEngine::ReadFileContents(const std::string &path)
{
    std::string data;
    std::ifstream stream(path, std::ios::in);
    if (stream.is_open())
    {
        std::stringstream sstr;
        sstr << stream.rdbuf();
        data = sstr.str();
        stream.close();
        return data;
    }
    else
    {
        ERROR("Could not open file: " + path);
        return 0;
    }
}

std::string DrageEngine::GetFileExtension(const std::string &path)
{
    long split = path.rfind('.');
    return path.substr(split + 1, path.length() - split - 1);
}

std::string DrageEngine::GetFileName(const std::string &path)
{
    long start;
    if (path.rfind('/') != std::string::npos)
        start = path.rfind('/') + 1;
    else
        start = 0;
    
    long length = path.length() - start;
    return path.substr(start, length);
}

std::string DrageEngine::GetFileNameWithoutExtension(const std::string &path)
{
    long start;
    if (path.rfind('/') != std::string::npos)
        start = path.rfind('/') + 1;
    else
        start = 0;
    
    long end = path.rfind('.');
    long length = end - start;
    return path.substr(start, length);
}
