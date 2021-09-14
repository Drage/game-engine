
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

namespace DrageEngine
{
    class String
    {
        public:
            static int Find(const std::string& str, const std::string& find, int &length, int from = 0);
            static std::string& Replace(std::string& str, const std::string& find, const std::string& replace);
            static void Split(const std::string& str, const std::string& delimiter, std::vector<std::string>& result);
            static bool IsNullOrEmpty(const std::string& str);
            static bool StartsWith(const std::string& str, const std::string& find);
            static bool EndsWith(const std::string& str, const std::string& find);
            static bool Contains(const std::string& str, const std::string& find);
    };
}

#endif
