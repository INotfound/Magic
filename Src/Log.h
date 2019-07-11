#pragma once
#include <list>
#include <tuple>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
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
    static const char* toString(LogLevel::Level level);
};

class LogEvent{
public:
    LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t fiberId,uint64_t threadId,const std::string& file,const std::string &logName,const std::string& context,const std::string& threadName);
    uint32_t getLine() const;
    uint64_t getTime() const;
    uint64_t getElapse() const;
    uint64_t getFiberId() const;
    uint64_t getThreadId() const;
    const std::string& getFile() const;
    const std::string& getLogName() const;
    const std::string& getContext() const; 
    const std::string& getThreadName()const;

private:
    uint32_t Line       =0;
    uint64_t Time       =0;
    uint64_t Elapse     =0;
    uint64_t FiberId    =0;
    uint64_t ThreadId   =0;
    std::string File;
    std::string LogName;
    std::string Context;
    std::string ThreadName;
};

class ILogAppender{
    friend class Logger;
public:
    virtual ~ILogAppender(){};
    virtual void log(LogLevel::Level level, std::shared_ptr<LogEvent> event) = 0;
protected:
    std::shared_ptr<LogFormatter> Formatter;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem(){};
    virtual void format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event) =0;
};

class LogFormatter{
public:
    LogFormatter(const std::string& pattern);
    std::string format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event);

private:
    std::vector<std::shared_ptr<ILogFormatItem>> Items;
};


class Logger : public std::enable_shared_from_this<Logger>{
public:
    Logger(const std::string& name = "root",const std::string& formatPattern = "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    void addILogAppender(std::shared_ptr<ILogAppender> logAppender);
    void delILogAppender(std::shared_ptr<ILogAppender> logAppender);
    void setNewFormatter(std::shared_ptr<LogFormatter> formatter);
    const std::string& getLogName() const;
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

class StdOutLogAppender : public ILogAppender{
public:
    void log(LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
};

class FileLogAppender : public ILogAppender{
public:
    FileLogAppender(const std::string &path);
    void log(LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
    bool reOpen();

private:
    std::string Path;
    std::ofstream OutPutFile;
};

} // namespace Log
