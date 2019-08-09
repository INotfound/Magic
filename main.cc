#include <iostream>
#include <memory>
#include <time.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"

static auto&  log = MAGIC_LOG_ROOT();

int main(){
    MAGIC_LOG_DEBUG(log) << "hello XiaoBaiJun1";
    std::cout.flush();
	return 0;
}
