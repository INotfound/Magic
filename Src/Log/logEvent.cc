#include "logEvent.h"

using namespace Magic;

LogEvent::LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t fiberId,uint64_t threadId,const std::string& file,const std::string& logName,const std::string& threadName)
	:m_Line(line),m_Time(time),m_Elapse(elapse),m_FiberId(fiberId),
	m_ThreadId(threadId),m_File(file),m_LogName(logName),m_ThreadName(threadName){
}

uint32_t LogEvent::getLine()const{return this->m_Line;}
uint64_t LogEvent::getTime()const{return this->m_Time;}
uint64_t LogEvent::getElapse()const{return this->m_Elapse;}
uint64_t LogEvent::getFiberId()const{return this->m_FiberId;}
uint64_t LogEvent::getThreadId()const{return this->m_ThreadId;}
std::stringstream& LogEvent::getStream(){return this->m_StrStream;}
const std::string  LogEvent::getContent()const{return this->m_StrStream.str();}
const std::string& LogEvent::getFile()const{return this->m_File;}
const std::string& LogEvent::getLogName()const{return this->m_LogName;}
const std::string& LogEvent::getThreadName()const{return this->m_ThreadName;}