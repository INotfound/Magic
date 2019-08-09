#include <iostream>
#include <memory>
#include <time.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"

static auto&  log = MAGIC_LOG_ROOT();

int main(){
    MAGIC_LOG_DEBUG(log) << "hello XiaoBaiJun1";
    MAGIC_LOG_DEBUG(log) << "hello XiaoBaiJun2";
    MAGIC_LOG_DEBUG(log) << "hello XiaoBaiJun3";
    MAGIC_LOG_DEBUG(log) << "hello XiaoBaiJun4";

    std::cout.flush();
	return 0;
}
