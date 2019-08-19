#pragma once
#include <vector>
#include <string>
#include <stdint.h>

namespace Magic{
	void Init();
	uint64_t GetThreadId();
	uint64_t GetFiberId();
	void BackTrace(std::vector<std::string>&vec,uint32_t size,uint32_t skip);
	std::string BackTraceToString(uint32_t size =64,uint32_t skip =0,const std::string& prefix ="    ");
}
