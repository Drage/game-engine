
#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>
#include <deque>
#include <sstream>
#include "GameTime.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG(x) Debug::Log(x, __FILENAME__, __LINE__);
#define WARNING(x) Debug::LogWarning(x, __FILENAME__, __LINE__);
#define ERROR(x) Debug::LogError(x, __FILENAME__, __LINE__);

namespace DrageEngine
{
    class Debug
    {
        public:
            enum LogType
            {
                INFO = 0,
                WARNING = 1,
                ERROR = 2
            } LogType;
        
            typedef struct LogMessage
            {
                unsigned id;
                enum LogType type;
                std::string message;
                std::string timestamp;
                std::string backtrace;
                std::string originFile;
                int originLine;
            } LogMessage;
        
            typedef std::deque<LogMessage> LogHistory;
    
            template<class T>
            static void Log(const T &message, const std::string &file, const int line)
            {
                LogToConsole(LogType::INFO, message, file, line);
            }
        
            template<class T>
            static void LogWarning(const T &message, const std::string &file, const int line)
            {
                LogToConsole(LogType::WARNING, message, file, line);
            }

            template<class T>
            static void LogError(const T &message, const std::string &file, const int line)
            {
                LogToConsole(LogType::ERROR, message, file, line);
            }
        
            static const std::deque<LogMessage>& GetLogHistory();
            static int GetLogHistoryCount();
            static void ClearLogHistory();
        
        private:
            static const int MAX_HISTORY = 1000;
            static LogHistory history;
            static unsigned nextLogId;
        
            template<class T>
            static void LogToConsole(enum LogType type, const T &message, const std::string &file, const int line)
            {
                LogMessage log;
                log.id = nextLogId++;
                log.type = type;
                log.timestamp = Time::Now();
                
                std::stringstream ss;
                ss << message;
                log.message = ss.str();
                
                log.originFile = file;
                log.originLine = line;
                
                log.backtrace = Backtrace();
                
                history.push_back(log);
                if (history.size() > MAX_HISTORY)
                    history.pop_front();
                
                std::string icon;
                switch (type)
                {
                    case LogType::INFO: icon = "ℹ️"; break;
                    case LogType::WARNING: icon = "⚠️"; break;
                    case LogType::ERROR: icon = "🛑"; break;
                }
                
                std::cout << icon << " " << log.message << " " << "[" << file << ":" << line << "]" << std::endl;
                std::cout << log.backtrace;
            }
        
            static std::string Backtrace();
    };
}

#endif
