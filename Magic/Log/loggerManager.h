#pragma once
#include <map>
#include <string>
#include <memory> 
#include "../Define.h"
#include "../Thread/Mutex.h"
#include "../Util/Singleton.h"
namespace Magic{
class Logger;

class LoggerManager{
    typedef Mutex MutexType;
public:
    LoggerManager();
    MagicPtr<Logger>& getRoot();
    MagicPtr<Logger>& getLogger(const std::string& name);
private:
    MutexType m_Mutex;
    MagicPtr<Logger> m_Root;
    std::map<std::string,MagicPtr<Logger>> m_Loggers;
};

typedef Singleton<LoggerManager>        LoggerMgr;

}

#define MAGIC_LOG_ROOT()            Magic::LoggerMgr::GetInstance()->getRoot()

#define MAGIC_LOG_NAME(Name)        Magic::LoggerMgr::GetInstance()->getLogger(Name)
