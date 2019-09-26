#pragma once
#include <map>
#include <list>
#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "Core.h"
#include "Util.h"

typedef std::mutex Mutex;
typedef std::lock_guard<Mutex> MutexLock;

namespace Magic{

namespace LogLevel{
    enum Level{
        DEBUG   =1,
        INFO    =2,
        WARN    =3,
        ERROR   =4,
        FATAL   =5
    };
    const char* ToString(const LogLevel::Level level);
};

class LogEvent{
public:
    LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t fiberId,
    uint64_t threadId,const std::string& file,const std::string& LogName,const std::string& threadName);
    uint32_t getLine()                  const;
    uint64_t getTime()                  const;
    uint64_t getElapse()                const;
    uint64_t getFiberId()               const;
    uint64_t getThreadId()              const;
    const std::string  getContent()     const;
    const std::string& getFile()        const;
    const std::string& getLogName()     const;
    const std::string& getThreadName()  const;
    std::stringstream& getStream();
private:
    uint32_t m_Line     =0;
    uint64_t m_Time     =0;
    uint64_t m_Elapse   =0;
    uint64_t m_FiberId  =0;
    uint64_t m_ThreadId =0;
    std::string m_File;
    std::string m_LogName;
    std::string m_ThreadName;
    std::stringstream m_StringStream;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem();
    virtual void format(std::ostream &os,const LogLevel::Level  level,const Ptr<LogEvent>& event) =0;
};

class LogFormatter{
public:
    explicit LogFormatter(const std::string& pattern);
    void format(std::ostream &os,const LogLevel::Level  level,const Ptr<LogEvent>& event);
private:
    std::vector<Ptr<ILogFormatItem>> m_Items;
};


class ILogAppender{
    friend class Logger;
public:
    virtual ~ILogAppender();
    virtual void log(LogLevel::Level level,Ptr<LogEvent>& event) =0;
protected:
    Ptr<LogFormatter> m_Formatter;
};

class StdOutLogAppender :public ILogAppender{
public:
    void log(LogLevel::Level level,Ptr<LogEvent>& event) override;
};

class FileLogAppender :public ILogAppender{
public:
    explicit FileLogAppender(const std::string &path);
    void log(LogLevel::Level level,Ptr<LogEvent>& event) override;
    bool reOpen();
private:
    Mutex m_Mutex;
    std::string m_Path;
    std::ofstream m_FileStream;
};

class Logger {
    friend class LoggerManager;
public:
    explicit Logger(const std::string &name = "root");
    void addILogAppender(Ptr<ILogAppender>& logAppender);
    void delILogAppender(Ptr<ILogAppender>& logAppender);
    void setFormatter(const std::string &pattern);
    void setLevel(LogLevel::Level);
    LogLevel::Level getLevel() const;
    const std::string& getLogName() const;
    void log(LogLevel::Level level,Ptr<LogEvent>& event);
private:
    Mutex m_Mutex;
    std::string m_LogName;
    std::string m_Formatter;
	LogLevel::Level m_Level;
	std::list<Ptr<ILogAppender>> m_ILogAppenders;
};

class LoggerManager{
public:
    LoggerManager();
    Ptr<Logger>& getRoot();
    Ptr<Logger>& getLogger(const std::string& name);
private:
    Ptr<Logger> m_Root;
    std::map<std::string,Ptr<Logger>> m_Loggers;
};
typedef Singleton<LoggerManager>        LoggerMgr;

class LogWrap{
public:
    LogWrap(Ptr<Logger>& logger,const LogLevel::Level level,Ptr<LogEvent>&& event);
    std::stringstream& get();
    ~LogWrap();
private:
    Ptr<Logger>& m_Logger;
    LogLevel::Level m_Level;
    Ptr<LogEvent> m_Event;
};


}