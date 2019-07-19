#pragma once
#include <map>
#include <string>
#include <memory> 
namespace Magic{
class Logger;
    
class loggerManager{
public:
    loggerManager();
    std::shared_ptr<Logger> getRoot();
    std::shared_ptr<Logger> getLogger(const std::string& name);
private:
    std::shared_ptr<Logger> m_root;
    std::map<std::string,Logger> m_Loggers;
};

}