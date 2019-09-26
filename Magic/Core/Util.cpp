#include "Util.h"
#include "Adapter/Adapter.h"

uint64_t Magic::GetFiberId(){
	return 0;
}

uint64_t Magic::GetThreadId(){
	return Adapter::GetThreadIdEx();
    return 0;
}