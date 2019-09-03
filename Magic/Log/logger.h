#pragma once
#include <list>
#include <string>
#include <memory>
#include "logLevel.h"
#include "../Util/Macro.h"
#include "../Thread/Mutex.h"

namespace Magic{
class LogEvent;
class ILogAppender;
class LogFormatter;

class Logger {
    friend class LoggerManager;
    typedef Mutex MutexType;
public:
    explicit Logger(const std::string& name = "root");
    void addILogAppender(Ptr<ILogAppender>& logAppender);
    void delILogAppender(Ptr<ILogAppender>& logAppender);
    void setFormatter(const std::string& pattern);
    void setLevel(LogLevel::Level);
    LogLevel::Level getLevel() const;
    const std::string& getLogName() const;
    void log(LogLevel::Level level, Ptr<LogEvent>& event);
private:
    MutexType m_Mutex;
    std::string m_LogName;
    std::string m_Formatter;
	LogLevel::Level m_Level;
	std::list<Ptr<ILogAppender>> m_ILogAppenders;
};
}
