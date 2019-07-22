#pragma once
#include <map>
#include <string>
#include <memory> 
#include "../Util/Singleton.h"

namespace Magic{
class Logger;

class LoggerManager{
public:
    LoggerManager();
    std::shared_ptr<Logger> getRoot();
    std::shared_ptr<Logger> getLogger(const std::string& name);
private:
    std::shared_ptr<Logger> m_Root;
    std::map<std::string,std::shared_ptr<Logger>> m_Loggers;
};



}