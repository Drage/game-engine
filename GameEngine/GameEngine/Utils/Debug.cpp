
#include <iostream>
#include <execinfo.h>
#include <sstream>
#include <dlfcn.h>
#include <cxxabi.h>
#include "Debug.h"
#include "StringUtils.h"

using namespace DrageEngine;

void Debug::Log(const std::string &message, const std::string &file, const int line)
{
    std::cout << "ℹ️ " << message << "  [" << file << ":" << line << "]" << std::endl;
    std::cout << Backtrace();
}

void Debug::LogWarning(const std::string &message, const std::string &file, const int line)
{
    std::cout << "⚠️ " << message << "  [" << file << ":" << line << "]" << std::endl;
    std::cout << Backtrace();
}

void Debug::LogError(const std::string &message, const std::string &file, const int line)
{
    std::cout << "🛑 " << message << "  [" << file << ":" << line << "]" << std::endl;
    std::cout << Backtrace();
}

std::string Debug::Backtrace()
{
    void *callstack[32];
    int maxFrames = sizeof(callstack) / sizeof(callstack[0]);
    int frameCount = backtrace(callstack, maxFrames);
    char **symbols = backtrace_symbols(callstack, frameCount);
    
    std::ostringstream ss;
    for (int i = 2; i < frameCount; i++)
    {
        Dl_info info;
        if (dladdr(callstack[i], &info))
        {
            char *demangled = NULL;
            int status;
            demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
            if (status == 0)
            {
                std::string line = std::string(demangled);
                String::Replace(line, "DrageEngine::", "");
                String::Replace(line, "std::__1::", "");
                String::Replace(line, "basic_string<char, char_traits<char>, allocator<char> >", "string");
                String::Replace(line, ", allocator<%> ", "");
                ss << " ↳ " << line << std::endl;
            }
            free(demangled);
        }
        
    }
    free(symbols);
    
    if (frameCount == maxFrames)
        ss << "[truncated]" << std::endl;
    
    ss << std::endl;
    
    return ss.str();
}
