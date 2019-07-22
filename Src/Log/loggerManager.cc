#include "loggerManager.h"
#include "logger.h"
#include "logAppender.h"

using namespace Magic;

LoggerManager::LoggerManager(){
    this->m_Root.reset(new Logger());
    this->m_Root->setFormatPattern("[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%c]%T%f:%l%T%m%n");
    this->m_Root->addILogAppender(std::make_shared<StdOutLogAppender>());
    this->m_Loggers[m_Root->m_LogName]= this->m_Root;
}

std::shared_ptr<Logger> LoggerManager::getRoot(){
    return this->m_Root;
}

std::shared_ptr<Logger> LoggerManager::getLogger(const std::string& name){
    auto it = this->m_Loggers.find(name);
    if(it != this->m_Loggers.end()){
        return it->second;
    }
    std::shared_ptr<Logger> loggerTemp =  std::make_shared<Logger>(name);
    loggerTemp->m_Root = this->m_Root;
    this->m_Loggers[name] = loggerTemp;
    return loggerTemp;
}