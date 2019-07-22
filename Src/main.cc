#include <iostream>
#include <memory>
#include <time.h>
#include "Log/Log.h"
#include "Util/Util.h"

static std::shared_ptr<Magic::Logger> log = MAGIC_LOG_ROOT();

int main(){
    MAGIC_LOG_DEBUG(log) << "hello XiaoBaiJun1";
    MAGIC_LOG_INFO(log) << "hello XiaoBaiJun2";
    MAGIC_LOG_WARN(log) << "hello XiaoBaiJun3";
    MAGIC_LOG_FATAL(log) << "hello XiaoBaiJun4";
                
    std::cout.flush();
    std::getchar();
	return 0;
}