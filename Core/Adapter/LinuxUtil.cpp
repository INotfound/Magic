#if defined(linux) || defined(__linux__)
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "Linux.h"
#include "../Util.h"
#include "../Macro.h"


namespace Magic {

	uint32_t GetProcessorsNumber() {
		unsigned int count{ 1 };
		count = sysconf(_SC_NPROCESSORS_CONF);
		return count;
	}

	uint64_t GetThreadId() {
		return syscall(SYS_gettid);
	}

	static void BackTrace(std::vector<std::string>& vec, uint32_t size, uint32_t skip) {
		void** buffer = static_cast<void**>(std::malloc(sizeof(void*) * size));
		int32_t retValue{ backtrace(buffer, size) };
		char** string = backtrace_symbols(buffer, retValue);
		if (string == nullptr) {
			MAGIC_LOG(Magic::LogLevel::LogError) << "backtrace_symbols error!";
		}
		uint32_t newValue{ static_cast<uint32_t>(retValue) };
		for (uint32_t i{ skip }; i < newValue; i++) {
			vec.push_back(string[i]);
		}
		free(buffer);
	}

	std::string BackTraceToString(uint32_t size, uint32_t skip, const std::string& prefix) {
		std::vector<std::string> vecString{};
		BackTrace(vecString, size, skip);
		std::stringstream ss{};
		for (size_t i{ 0 }; i < vecString.size(); i++)
		{
			ss << prefix << vecString.at(i) << std::endl;
		}
		ss << std::endl;
		return ss.str();
	}
}
#endif
