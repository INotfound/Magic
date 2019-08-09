#pragma once
#include <list>
#include <string>
#include <memory>
#include "logLevel.h"

namespace Magic{
class LogEvent;
class ILogAppender;
class LogFormatter;

class Logger {
    friend class LoggerManager;
public:
    explicit Logger(const std::string& name = "root");
    void addILogAppender(std::unique_ptr<ILogAppender>& logAppender);
    void delILogAppender(std::unique_ptr<ILogAppender>& logAppender);
    void setLevel(LogLevel::Level);
    LogLevel::Level getLevel() const;
    const std::string& getLogName() const;
    void log(LogLevel::Level level, std::unique_ptr<LogEvent>& event);
private:
    std::string m_LogName;
    std::string m_Formatter;
	LogLevel::Level m_Level;
    std::list<std::unique_ptr<ILogAppender>> m_ILogAppenders;
};
}