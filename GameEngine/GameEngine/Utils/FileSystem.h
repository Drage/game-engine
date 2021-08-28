
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <string>
#include "ParamList.h"

namespace DrageEngine
{
    bool GetFilesInDirectory(const std::string &directory, std::vector<std::string> &files, bool recursiveSearch = false);
    std::string FindFilePath(const std::string &directory, const std::string &filename);
    std::string ReadFileContents(const std::string &path);
    std::string GetFileExtension(const std::string &path);
    std::string GetFileName(const std::string &path);
    std::string GetFileNameWithoutExtension(const std::string &path);
}

#endif
