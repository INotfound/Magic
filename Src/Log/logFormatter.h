#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "logLevel.h"

namespace Magic{

class LogEvent;
class ILogFormatItem;

class LogFormatter{
public:
    LogFormatter(const std::string& pattern);
    std::string format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event);
private:
    std::vector<std::shared_ptr<ILogFormatItem>> m_Items;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem(){};
    virtual void format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event) =0;
};

}