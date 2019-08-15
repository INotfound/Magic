#include <iostream>
#include <memory>
#include <time.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"
#include "Magic/Thread/Thread.h"

static auto& g_rlog = MAGIC_LOG_ROOT();

void bibo(){
    MAGIC_LOG_INFO(g_rlog) << "hello XiaoBaiJun1";
    std::cout.flush();
}

int main(){
    {
        Magic::Thread thread("233",&bibo);
        thread.join();
        MAGIC_LOG_INFO(g_rlog) << "hello XiaoBaiJun2";
    }
	return 0;
}
