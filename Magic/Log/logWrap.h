#pragma once
#include <memory>
#include <sstream>
#include "logLevel.h"
#include "../Define.h"

namespace Magic{
class Logger;
class LogEvent;

class LogWrap{
public:
    LogWrap(MagicPtr<Logger>& logger,LogLevel::Level level,MagicPtr<LogEvent>&& event);
    std::stringstream& get();
    ~LogWrap();
private:
    MagicPtr<Logger>& m_Logger;
    LogLevel::Level m_Level;
    MagicPtr<LogEvent> m_Event;
};

}
