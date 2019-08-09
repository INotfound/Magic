#include "../Util.h"

#include <windows.h>

uint64_t Magic::getThreadId(){
    return GetCurrentThreadId();
}
