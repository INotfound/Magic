#include "Util.h"
#include <unistd.h>
#include <sys/syscall.h>

uint64_t Magic::getThreadId(){
    return static_cast<uint64_t>(syscall(SYS_gettid));
}

uint64_t Magic::getFiberId(){
    return 0;
}

