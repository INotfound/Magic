#pragma once
#include "../Core.h"


#if defined(_WIN32) || defined(_WIN64)
#include "Win32.h"
#else
#include "Linux.h"
#endif

namespace Adapter{

int64_t GetThreadIdEx();
std::string BackTraceToString(uint32_t size =64,uint32_t skip =2,const std::string& prefix ="    ");

}