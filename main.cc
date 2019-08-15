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
        Magic::Thread thread("111",&bibo);
        Magic::Thread thread1("222",&bibo);
        Magic::Thread thread2("333",&bibo);
        thread.join();
        thread1.join();
        thread2.join();
    }
	return 0;
}
