#pragma once
#include <list>
#include <string>
#include <memory>
#include "logLevel.h"

namespace Magic{
class LogEvent;
class ILogAppender;
class LogFormatter;
class Logger : public std::enable_shared_from_this<Logger>{
public:
    Logger(const std::string& name = "root",const std::string& formatPattern = "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    void addILogAppender(std::shared_ptr<ILogAppender> logAppender);
    void delILogAppender(std::shared_ptr<ILogAppender> logAppender);
    const std::string& getLogName() const;
    void log(LogLevel::Level level, std::shared_ptr<LogEvent> event);


private:

    std::string m_LogName;
	LogLevel::Level m_Level;
    std::shared_ptr<Logger> m_Root;
    std::shared_ptr<LogFormatter> m_Formatter;
    std::list<std::shared_ptr<ILogAppender>> m_ILogAppenders;
};
}