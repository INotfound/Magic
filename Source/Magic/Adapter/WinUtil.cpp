#if defined(_WIN32) || defined(_WIN64)
#include "Core.h"
#include "Macro.h"
#include "Adapter/Win32.h"

namespace Magic {

	uint64_t GetCurrentTimeMS(){
		SYSTEMTIME st;
		GetLocalTime(&st);
		return st.wMilliseconds;
	}
	uint64_t GetCurrentTimeUS(){
	#define EPOCHFILETIME (116444736000000000UL)
		FILETIME ft;
		LARGE_INTEGER li;
		int64_t tt = 0;
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
		tt = (li.QuadPart - EPOCHFILETIME) /10;
		return tt;
	#undef EPOCHFILETIME
	}

	uint32_t GetProcessorsNumber() {
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
	}

	uint64_t GetThreadId() {
		return GetCurrentThreadId();
	}

	std::string BackTraceToString(uint32_t, uint32_t, const std::string&) {
		return "Plase use WinDbg!!!";
	}
	int32_t StringCompareNoCase(const std::string& dest,const std::string& src){
		return stricmp(dest.c_str(), src.c_str());
	}
}
#endif