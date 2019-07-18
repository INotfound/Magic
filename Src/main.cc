#include <iostream>
#include <time.h>
#include "Log/Log.h"
#include "Util/Util.h"

int main(){
    std::shared_ptr<Magic::Logger> logger = std::make_shared<Magic::Logger>();
    logger->addILogAppender(std::shared_ptr<Magic::ILogAppender>(new Magic::FileLogAppender("debug.txt")));
    logger->setLevel(Magic::LogLevel::DEBUG);
    
    MAGIC_LOG_DEBUG(logger) << "hello XiaoBaiJun";
    
    std::getchar();
	return 0;
}