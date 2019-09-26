#include "loggerManager.h"
#include "logger.h"
#include "logAppender.h"

using namespace Magic;

LoggerManager::LoggerManager(){
    this->m_Root.reset(new Logger());
    this->m_Root->setFormatter("[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%c]%T%f:%l%T%m%n");
    Ptr<ILogAppender> appender(new StdOutLogAppender);
    this->m_Root->addILogAppender(appender);
}

Ptr<Logger>& LoggerManager::getRoot(){
    return this->m_Root;
}

Ptr<Logger>& LoggerManager::getLogger(const std::string& name){
    if(name == this->m_Root->m_LogName){
        return this->m_Root;
    }
    MutexType::Lock lock(m_Mutex);
    auto it = this->m_Loggers.find(name);
    if(it != this->m_Loggers.end()){
        return it->second;
    }
    this->m_Loggers[name].reset(new Logger(name));
    return  this->m_Loggers[name];
}
