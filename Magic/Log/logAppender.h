#pragma once
#include <memory>
#include <fstream>
#include "logLevel.h"
#include "logFormatter.h"
#include "../Thread/Mutex.h"
namespace Magic{
class LogEvent;
class LogFormatter;

class ILogAppender{
    friend class Logger;
public:
    typedef Mutex MutexType;

    virtual ~ILogAppender();
    virtual void log(LogLevel::Level level,std::unique_ptr<LogEvent>& event) =0;
protected:
    MutexType m_Mutex;
    std::unique_ptr<LogFormatter> m_Formatter;
};

class StdOutLogAppender : public ILogAppender{
public:
    void log(LogLevel::Level level, std::unique_ptr<LogEvent>& event) override;
};

class FileLogAppender : public ILogAppender{
public:
    explicit FileLogAppender(const std::string &path);
    void log(LogLevel::Level level, std::unique_ptr<LogEvent>& event) override;
    bool reOpen();
private:
    std::string m_Path;
    std::ofstream m_FileStream;
};

}
