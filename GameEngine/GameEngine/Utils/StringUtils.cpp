
#include <sstream>
#include <algorithm>
#include "StringUtils.h"

using namespace DrageEngine;

int String::Find(const std::string& str, const std::string& find, int &length, int from)
{
    std::vector<std::string> tokens;
    Split(find, "%", tokens);
    
    int start = from;
    int pos = start;
    for (int i = 0; i < tokens.size(); i++)
    {
        pos = (int)str.find(tokens[i], pos);
        
        if (pos == std::string::npos)
            return -1;
        
        if (i == 0)
            start = pos;
        
        pos += tokens[i].length();
    }
    
    length = pos - start;
    return start;
}

std::string& String::ReplaceAll(std::string& str, const std::string& find, const std::string& replace)
{
    int pos = 0, length;
    while ((pos = Find(str, find, length, pos)) != std::string::npos)
    {
        str.replace(pos, length, replace);
        pos += replace.length();
    }
    return str;
}

std::string& String::Replace(std::string& str, const std::string& find, const std::string& replace)
{
    int pos = 0, length;
    if ((pos = Find(str, find, length, pos)) != std::string::npos)
        str.replace(pos, length, replace);
    return str;
}

void String::Split(const std::string& str, const std::string& delimiter, std::vector<std::string>& result)
{
    int last = 0, next;
    std::string token;
    while ((next = (int)str.find(delimiter, last)) != std::string::npos)
    {
        token = str.substr(last, next - last);
        result.push_back(token);
        last = next + (int)delimiter.length();
    }
    token = str.substr(last, str.length());
    result.push_back(token);
}

bool String::IsNullOrEmpty(const std::string& str)
{
    for (std::string::const_iterator i = str.begin(); i != str.end(); i++)
    {
        if (*i != ' ' && *i != '\n')
            return false;
    }
    return true;
}

bool String::StartsWith(const std::string& str, const std::string& find)
{
    if (str.length() >= find.length())
        return str.compare(0, find.length(), find) == 0;
    else
        return false;
}

bool String::EndsWith(const std::string& str, const std::string& find)
{
    if (str.length() >= find.length())
        return str.compare(str.length() - find.length(), find.length(), find) == 0;
    else
        return false;
}

bool String::Contains(const std::string& str, const std::string& find)
{
    return str.find(find) != std::string::npos;
}

bool String::ContainsIgnoreCase(const std::string& str, const std::string& find)
{
    auto it = std::search(
        str.begin(), str.end(),
        find.begin(), find.end(),
        [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
    );
    return (it != str.end());
}

std::string String::Join(std::initializer_list<std::string> list)
{
    std::stringstream ss;
    for (auto elem : list)
        ss << elem;
    return ss.str();
}
