#pragma once
#include <memory>
#include <sstream>
#include "logLevel.h"

namespace Magic{
class Logger;
class LogEvent;

class LogWrap{
public:
    LogWrap(std::shared_ptr<Logger> logger,LogLevel::Level level,std::shared_ptr<LogEvent> event);
    std::stringstream& get();
    ~LogWrap();
private:
    LogLevel::Level m_Level;
    std::shared_ptr<Logger> m_Logger;
    std::shared_ptr<LogEvent> m_Event;
};

}