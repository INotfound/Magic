#if defined(_WIN32) || defined(_WIN64)
#include "Win32.h"
#include "../Util.h"
#include "../Macro.h"


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