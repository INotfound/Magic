#if defined(_WIN32) || defined(_WIN64)
#include "Util.h"
#include "Macro.h"
#include "Adapter/Win32.h"

namespace Magic {
	uint32_t GetProcessorsNumber() {
		SYSTEM_INFO info{};
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
	}

	uint64_t GetThreadId() {
		return GetCurrentThreadId();
	}

	std::string BackTraceToString(uint32_t, uint32_t, const std::string&) {
		return "Plase use WinDbg!!!";
	}

}
#endif