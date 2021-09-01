
#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG(x) Debug::Log(x, __FILENAME__, __LINE__);
#define WARNING(x) Debug::LogWarning(x, __FILENAME__, __LINE__);
#define ERROR(x) Debug::LogError(x, __FILENAME__, __LINE__);

namespace DrageEngine
{
    class Debug
    {
        public:
            template<class T>
            static void Log(const T &message, const std::string &file, const int line)
            {
                std::cout << "ℹ️ " << message << "  [" << file << ":" << line << "]" << std::endl;
                std::cout << Backtrace();
            }
        
            template<class T>
            static void LogWarning(const T &message, const std::string &file, const int line)
            {
                std::cout << "⚠️ " << message << "  [" << file << ":" << line << "]" << std::endl;
                std::cout << Backtrace();
            }

            template<class T>
            static void LogError(const T &message, const std::string &file, const int line)
            {
                std::cout << "🛑 " << message << "  [" << file << ":" << line << "]" << std::endl;
                std::cout << Backtrace();
            }
        
        private:
            static std::string Backtrace();
    };
}

#endif
