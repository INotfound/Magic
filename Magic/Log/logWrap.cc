#include "logWrap.h"
#include "logger.h"
#include "logEvent.h"
using namespace Magic;

LogWrap::LogWrap(MagicPtr<Logger>& logger,LogLevel::Level level,MagicPtr<LogEvent>&& event):m_Logger(logger),m_Level(level),m_Event(std::move(event)){
}
std::stringstream& LogWrap::get(){
	return this->m_Event->getStream();
}
LogWrap::~LogWrap(){
	this->m_Logger->log(this->m_Level,this->m_Event);
}
