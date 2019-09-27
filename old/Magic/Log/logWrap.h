#pragma once
#include <memory>
#include <sstream>
#include "logLevel.h"
#include "../Util/Macro.h"

namespace Magic{
class Logger;
class LogEvent;

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