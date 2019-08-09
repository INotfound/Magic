#include "logWrap.h"
#include "logger.h"
#include "logEvent.h"
using namespace Magic;

LogWrap::LogWrap(std::unique_ptr<Logger>& logger,LogLevel::Level level,std::unique_ptr<LogEvent> event):m_Logger(logger),m_Level(level){
    this->m_Event = std::move(event);
}
std::stringstream& LogWrap::get(){
	return this->m_Event->getStream();
}
LogWrap::~LogWrap(){
	this->m_Logger->log(this->m_Level,this->m_Event);
}
