#include <iostream>
#include <time.h>
#include <memory>
#include "Log.h"

int main(){	
    std::shared_ptr<Log::Logger> logger = std::make_shared<Log::Logger>();
    std::shared_ptr<Log::LogFormatter> format = std::make_shared<Log::LogFormatter>();
    format->initFormat("%m [%p %c %t] %f");
    logger->setFormatter(format);
    logger->addILogAppender(std::shared_ptr<Log::ILogAppender>(new Log::StdOutLogAppender));
    std::shared_ptr<Log::LogEvent> event = std::make_shared<Log::LogEvent>(__LINE__,time(0),1,0,0,__FILE__,logger->getLogName(),"2223333","thread");
    logger->debug(event);
	return 0;
}
 