#pragma once
#include <memory>
#include <sstream>
#include "logLevel.h"

namespace Magic{
class Logger;
class LogEvent;

class LogWrap{
public:
    LogWrap(std::unique_ptr<Logger>& logger,LogLevel::Level level,std::unique_ptr<LogEvent>&& event);
    std::stringstream& get();
    ~LogWrap();
private:
    std::unique_ptr<Logger>& m_Logger;
    LogLevel::Level m_Level;
    std::unique_ptr<LogEvent> m_Event;
};

}
