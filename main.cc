#include <iostream>
#include <memory>
#include <time.h>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"
#include "Magic/Thread/Thread.h"
static auto& g_rlog = MAGIC_LOG_ROOT();
void bibo(){
    std::cout << "sadsad " <<std::endl;
    MAGIC_LOG_DEBUG(g_rlog) << "hello XiaoBaiJun1";

}

int main(){
    {
        MAGIC_LOG_DEBUG(g_rlog) << "hello XiaoBaiJun2";
        Magic::Thread thread("233",&bibo);
        thread.join();
    }
	return 0;
}
