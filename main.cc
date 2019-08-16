#include <iostream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"
#include "Magic/Thread/Thread.h"
#include "Magic/Thread/Mutex.h"

static auto& g_rlog = MAGIC_LOG_ROOT();
void bibo(){
    for(int i=0;i<10;i++)
        MAGIC_LOG_INFO(MAGIC_LOG_NAME("root")) << "[root root root] " << i;
}
void momo(){
    for(int i=0;i<10;i++)
        MAGIC_LOG_INFO(MAGIC_LOG_NAME("xibo")) << "[xibo xibo xibo] " << i;
}

int main(){
    {
        Magic::Thread thread("111",&bibo);
        Magic::Thread thread1("222",&momo);
    }
    getchar();
	return 0;
}

