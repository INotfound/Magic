#pragma once
#include <memory>
#include <fstream>
#include "logLevel.h"

namespace Magic{
class LogEvent;
class LogFormatter;

class ILogAppender{
	friend class Logger;
public:
	virtual ~ILogAppender(){};
	virtual void log(LogLevel::Level level,std::shared_ptr<LogEvent> event) =0;
protected:
	std::shared_ptr<LogFormatter> m_Formatter;
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
    std::string m_Path;
    std::ofstream m_FileStream;
};

}