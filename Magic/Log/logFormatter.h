#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "logLevel.h"
#include "../Define.h"
namespace Magic{

class LogEvent;
class ILogFormatItem;

class LogFormatter{
public:
    explicit LogFormatter(const std::string& pattern);
    void format(std::ostream &os, LogLevel::Level level, MagicPtr<LogEvent>& event);
private:
    std::vector<MagicPtr<ILogFormatItem>> m_Items;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem();
    virtual void format(std::ostream &os, LogLevel::Level level, MagicPtr<LogEvent>& event) =0;
};

}
