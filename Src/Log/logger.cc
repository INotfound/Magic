#include "logger.h"
#include "logAppender.h"
#include "logFormatter.h"

using namespace Magic;

Logger::Logger(const std::string& name,const std::string& formatPattern) :m_LogName(name){
	this->m_Formatter = std::make_shared<LogFormatter>(formatPattern);
}

void  Logger::addILogAppender(std::shared_ptr<ILogAppender> logAppender){
	if(!logAppender->m_Formatter){
		logAppender->m_Formatter = this->m_Formatter;
	}
	this->m_ILogAppenders.push_back(logAppender);
}

void  Logger::delILogAppender(std::shared_ptr<ILogAppender> logAppender){
	auto vBegin = this->m_ILogAppenders.begin();
	auto vEnd	= this->m_ILogAppenders.end();
	for(;vBegin!=vEnd;vBegin++){
		this->m_ILogAppenders.erase(vBegin);
	}
}

const std::string& Logger::getLogName() const{
	return this->m_LogName;
}

void  Logger::log(LogLevel::Level level, std::shared_ptr<LogEvent> event){
	for(auto &v :this->m_ILogAppenders){
		v->log(level,event);
	}
}