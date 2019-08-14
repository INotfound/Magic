#include "Util.h"
#include <unistd.h>
#include <pthread.h>

uint64_t Magic::getThreadId(){
    return static_cast<uint64_t>(pthread_self());
}

uint64_t Magic::getFiberId(){
    return 0;
}

