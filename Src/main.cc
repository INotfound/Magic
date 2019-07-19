#include <iostream>
#include <memory>
#include <time.h>
#include "Log/Log.h"
#include "Util/Util.h"
#include "Util/Singleton.h"



int main(){
    std::shared_ptr<Magic::Logger> logger = std::make_shared<Magic::Logger>("[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%c]%T%f:%l%T%m%n");
    logger->addILogAppender(std::shared_ptr<Magic::ILogAppender>(new Magic::StdOutLogAppender()));
    logger->setLevel(Magic::LogLevel::DEBUG);

    std::cout.flush();
    std::getchar();
	return 0;
}