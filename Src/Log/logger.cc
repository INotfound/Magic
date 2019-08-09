#include "logger.h"
#include "logLevel.h"
#include "logAppender.h"
#include "logFormatter.h"
#include "loggerManager.h"

using namespace Magic;

Logger::Logger(const std::string& name) :m_LogName(name),m_Level(LogLevel::DEBUG){
	
}

void  Logger::addILogAppender(std::unique_ptr<ILogAppender>& logAppender){
	if(!logAppender->m_Formatter){
		logAppender->m_Formatter.reset(new LogFormatter(m_Formatter));
	}
	this->m_ILogAppenders.push_back(std::move(logAppender));
}

void  Logger::delILogAppender(std::unique_ptr<ILogAppender>& logAppender){
	auto vBegin = this->m_ILogAppenders.begin();
	auto vEnd	= this->m_ILogAppenders.end();
	for(;vBegin!=vEnd;vBegin++){
        if(*vBegin == logAppender){
            this->m_ILogAppenders.erase(vBegin);
            return;
        }
	}
}


void Logger::setLevel(LogLevel::Level val){
    this->m_Level = val;
}

 LogLevel::Level Logger::getLevel() const{
    return this->m_Level;
}

const std::string& Logger::getLogName() const{
	return this->m_LogName;
}

void  Logger::log(LogLevel::Level level, std::unique_ptr<LogEvent>& event){
    if(!this->m_ILogAppenders.empty()){
        for(auto &v :this->m_ILogAppenders){
            v->log(level,event);
        }
    }else if(MAGIC_LOG_ROOT()){
        MAGIC_LOG_ROOT()->log(level,event);
    }
}
