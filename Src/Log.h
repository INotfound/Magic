#pragma once
#include <list>
#include <tuple>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

//%p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL
//%r 输出自应用启动到输出该log信息耗费的毫秒数
//%c 输出所属的类目，通常就是所在类的全名
//%t 输出产生该日志事件的线程名
//%n 输出一个回车换行符，Windows平台为“rn”，Unix平台为“n”
//%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy MMM dd HH:mm:ss,SSS}，输出类似：2002年10月18日 22：10：28，921
//%l 输出日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。举例：Testlog4.main(TestLog4.java:10)
namespace Log{

class Logger;
class LogLevel;
class LogEvent;
class LogFormatter;
class ILogAppender;
class ILogFormatItem;

class LogLevel{
public:
    enum Level{
        DEBUG   =1,
        INFO    =2,
        WARN    =3,
        ERROR   =4,
        FATAL   =5
    };
};

class LogEvent{
public:
    LogEvent();

private:
    uint32_t Line       =0;
    uint64_t Time       =0;
    uint64_t Elapse     =0;
    uint64_t FiberId    =0;
    uint64_t ThreadId   =0;
    std::string File;
};

class ILogAppender{
public:
    virtual ~ILogAppender(){};
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) = 0;
    void setFormatter(std::shared_ptr<LogFormatter> formatter);
    std::shared_ptr<LogFormatter> getFormatter();

private:
    std::shared_ptr<LogFormatter> Formatter;
};

class Logger : public std::enable_shared_from_this<Logger>{
public:
    explicit Logger(const std::string &name = "root");
    void addILogAppender(std::shared_ptr<ILogAppender> logAppender);
    void delILogAppender(std::shared_ptr<ILogAppender> logAppender);
    void setFormatter(std::shared_ptr<LogFormatter> formatter);
    void setFormatter(const std::string &val);
    std::shared_ptr<LogFormatter> getFormatter();
    void log(LogLevel::Level level, std::shared_ptr<LogEvent> event);
    void debug(std::shared_ptr<LogEvent> event);
    void info(std::shared_ptr<LogEvent> event);
    void warn(std::shared_ptr<LogEvent> event);
    void error(std::shared_ptr<LogEvent> event);
    void fatal(std::shared_ptr<LogEvent> event);

private:
    std::string LogName;
    LogLevel::Level Level;
    std::shared_ptr<Logger> Root;
    std::shared_ptr<LogFormatter> Formatter;
    std::list<std::shared_ptr<ILogAppender>> ILogAppenders;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem(){};
    std::string format();
};

class LogFormatter{
public:
    void initFormat(const std::string &pattern);
    std::string format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event);

private:
    std::vector<std::shared_ptr<ILogFormatItem>> Item;
};

//.....format item interface

class StdOutLogAppender : public ILogAppender{
public:
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
};

class FileLogAppender : public ILogAppender{
public:
    FileLogAppender(const std::string &path);
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
    bool reOpen();

private:
    std::string Path;
    std::ofstream OutPutFile;
};

} // namespace Log
