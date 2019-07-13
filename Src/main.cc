#include <iostream>
#include <time.h>
#include "Log/Log.h"

int	main(){
	std::shared_ptr<Magic::Logger> logger = std::make_shared<Magic::Logger>();
    logger->addILogAppender(std::shared_ptr<Magic::ILogAppender>(new Magic::StdOutLogAppender));
    std::shared_ptr<Magic::LogEvent> event = std::make_shared<Magic::LogEvent>(__LINE__,time(0),1,0,0,__FILE__,logger->getLogName(),"thread");
    Magic::LogWrap(logger,Magic::LogLevel::DEBUG,event).get() << "Hello World";
	return 0;
}