#pragma once
#include "Core.h"
#include "Adapter.h"

namespace Magic{

uint32_t GetProcessorsNumber();

uint64_t GetFiberId();

uint64_t GetThreadId();

std::string BackTraceToString(uint32_t size = 64, uint32_t skip = 2, const std::string& prefix = "    ");

}
