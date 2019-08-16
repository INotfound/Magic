#include <iostream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "Magic/Log/Log.h"
#include "Magic/Util/Util.h"
#include "Magic/Thread/Thread.h"
#include "Magic/Thread/Mutex.h"

static auto& g_rlog = MAGIC_LOG_ROOT();
void momo(){
    MAGIC_LOG_INFO(MAGIC_LOG_NAME("root")) << "this -> root ";
    MAGIC_LOG_INFO(MAGIC_LOG_NAME("MM")) << "this -> MM";
}
int main(){
    std::vector<std::unique_ptr<Magic::Thread>> thrs;
    for(size_t i =0; i<20; i++){
        thrs.push_back(std::unique_ptr<Magic::Thread>(new Magic::Thread(std::to_string(i),&momo)));
    }
	return 0;
}

