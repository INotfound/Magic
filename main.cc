#include <iostream>
#include <memory>
#include <time.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"
#include "Magic/Thread/Thread.h"
#include "Magic/Thread/Mutex.h"

static auto& g_rlog = MAGIC_LOG_ROOT();
Magic::RWMutex mutex;
void bibo(){
    Magic::RWMutex::WriteLock lock(mutex);
    MAGIC_LOG_INFO(g_rlog) << "hello XiaoBaiJun1";
    std::cout.flush();
}
int main(){
    {
        Magic::Thread thread("111",&bibo);
        Magic::Thread thread1("222",&bibo);
        Magic::Thread thread2("333",&bibo);
        getchar();
    }
    getchar();
	return 0;
}

