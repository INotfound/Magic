#include <iostream>
#include <memory>
#include <time.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"
#include "Magic/Thread/Thread.h"
static auto& g_rlog = MAGIC_LOG_ROOT();

int main(){
    MAGIC_LOG_DEBUG(g_rlog) << "hello XiaoBaiJun1";
    std::cout.flush();
	return 0;
}
