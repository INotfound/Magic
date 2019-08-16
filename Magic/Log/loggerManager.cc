#include "loggerManager.h"
#include "logger.h"
#include "logAppender.h"

using namespace Magic;

LoggerManager::LoggerManager(){
    this->m_Root.reset(new Logger());
    this->m_Root->setFormatter("[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%c]%T%f:%l%T%m%n");
    std::unique_ptr<ILogAppender> appender(new StdOutLogAppender);
    this->m_Root->addILogAppender(appender);
}

std::unique_ptr<Logger>& LoggerManager::getRoot(){
    return this->m_Root;
}

std::unique_ptr<Logger>& LoggerManager::getLogger(const std::string& name){
    if(name == this->m_Root->m_LogName){
        return this->m_Root;
    }
    Spinlock::Lock lock(m_Mutex);
    auto it = this->m_Loggers.find(name);
    if(it != this->m_Loggers.end()){
        return it->second;
    }
    this->m_Loggers[name].reset(new Logger(name));
    return  this->m_Loggers[name];
}
