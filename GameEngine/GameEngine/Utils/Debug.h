
#ifndef DEBUG_H
#define DEBUG_H

#include <string>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG(x) Debug::Log(x, __FILENAME__, __LINE__);
#define WARNING(x) Debug::LogWarning(x, __FILENAME__, __LINE__);
#define ERROR(x) Debug::LogError(x, __FILENAME__, __LINE__);

namespace DrageEngine
{
    class Debug
    {
        public:
            static void Log(const std::string &message, const std::string &file, const int line);
            static void LogWarning(const std::string &message, const std::string &file, const int line);
            static void LogError(const std::string &message, const std::string &file, const int line);
        
        private:
            static std::string Backtrace();
    };
}

#endif
