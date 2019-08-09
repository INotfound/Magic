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
    explicit LogFormatter(const std::string& pattern);
    void format(std::ostream &os, LogLevel::Level level, std::unique_ptr<LogEvent>& event);
private:
    std::vector<std::unique_ptr<ILogFormatItem>> m_Items;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem();
    virtual void format(std::ostream &os, LogLevel::Level level, std::unique_ptr<LogEvent>& event) =0;
};

}
