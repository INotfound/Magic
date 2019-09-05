#pragma once
#include <memory>
#include <fstream>
#include "logLevel.h"
#include "logFormatter.h"
#include "../Util/Define.h"
#include "../Thread/Mutex.h"
namespace Magic{
class LogEvent;
class LogFormatter;

class ILogAppender{
    friend class Logger;
public:
    virtual ~ILogAppender();
    virtual void log(LogLevel::Level level,Ptr<LogEvent>& event) =0;
protected:
    typedef Mutex MutexType;
    MutexType m_Mutex;
    Ptr<LogFormatter> m_Formatter;
};

class StdOutLogAppender : public ILogAppender{
public:
    void log(LogLevel::Level level, Ptr<LogEvent>& event) override;
};

class FileLogAppender : public ILogAppender{
public:
    explicit FileLogAppender(const std::string &path);
    void log(LogLevel::Level level, Ptr<LogEvent>& event) override;
    bool reOpen();
private:
    std::string m_Path;
    std::ofstream m_FileStream;
};

}
