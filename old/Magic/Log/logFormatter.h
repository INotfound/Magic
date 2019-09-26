#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "logLevel.h"
#include "../Util/Macro.h"
namespace Magic{

class LogEvent;
class ILogFormatItem;

class LogFormatter{
public:
    explicit LogFormatter(const std::string& pattern);
    void format(std::ostream &os,const LogLevel::Level level,const Ptr<LogEvent>& event);
private:
    std::vector<Ptr<ILogFormatItem>> m_Items;
};

class ILogFormatItem{
public:
    virtual ~ILogFormatItem();
    virtual void format(std::ostream &os,const LogLevel::Level level,const Ptr<LogEvent>& event) =0;
};

}
