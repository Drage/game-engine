
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include "Debug.h"
#include "StringUtils.h"

using namespace DrageEngine;

Debug::LogHistory Debug::history;
unsigned Debug::nextLogId = 0;

std::string Debug::Backtrace()
{
    void *callstack[32];
    int maxFrames = sizeof(callstack) / sizeof(callstack[0]);
    int frameCount = backtrace(callstack, maxFrames);
    char **symbols = backtrace_symbols(callstack, frameCount);
    
    std::ostringstream ss;
    for (int i = 3; i < frameCount; i++)
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
                String::ReplaceAll(line, "DrageEngine::", "");
                String::ReplaceAll(line, "std::__1::", "");
                String::ReplaceAll(line, "basic_string<char, char_traits<char>, allocator<char> >", "string");
                String::ReplaceAll(line, ", allocator<%> ", "");
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

const std::deque<Debug::LogMessage>& Debug::GetLogHistory()
{
    return history;
}

int Debug::GetLogHistoryCount()
{
    return nextLogId;
}

void Debug::ClearLogHistory()
{
    history.clear();
}
