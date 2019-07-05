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
    static std::string toString();
};

class LogEvent{
public:
    LogEvent();
    uint32_t getLine() const;
    uint64_t getTime() const;
    uint64_t getElapse() const;
    uint64_t getFiberId() const;
    uint64_t getThreadId() const;
    const std::string& getFile() const;
    const std::string& getContext() const; 
private:
    uint32_t Line       =0;
    uint64_t Time       =0;
    uint64_t Elapse     =0;
    uint64_t FiberId    =0;
    uint64_t ThreadId   =0;
    std::string File;
    std::string Context;
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

class ILogFormatItem{
public:
    virtual ~ILogFormatItem(){};
    virtual void format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event) =0;
};

class LogFormatter{
public:
    void initFormat(const std::string &pattern);
    std::string format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event);

private:
    std::vector<std::shared_ptr<ILogFormatItem>> Item;
};
//.....format item interface
class MessageFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override;
};

class LevelFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
};


} // namespace Log
