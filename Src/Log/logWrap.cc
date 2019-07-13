#include "logWrap.h"
#include "logger.h"
#include "logEvent.h"
using namespace Magic;

LogWrap::LogWrap(std::shared_ptr<Logger> logger,LogLevel::Level level,std::shared_ptr<LogEvent> event):m_Level(level),m_Logger(logger),m_Event(event){
}
std::stringstream& LogWrap::get(){
	return this->m_Event->getStream();
}
LogWrap::~LogWrap(){
	this->m_Logger->log(this->m_Level,this->m_Event);
}
