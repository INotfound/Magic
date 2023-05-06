/*
 * @Author: INotFound
 * @Date: 2020-05-09 09:39:20
 * @LastEditTime: 2021-02-01 22:33:42
 */
#if defined(_WIN32) || defined(_WIN64)

#include "Magic/Core/Core.h"
#include "Magic/Utilty/Timer.h"
#include "Magic/Utilty/Thread.h"
#include "Magic/Utilty/String.h"
#include "Magic/Utilty/Logger.h"

namespace Magic{
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
        tt = (li.QuadPart - EPOCHFILETIME) / 10;
        return tt;
    #undef EPOCHFILETIME
    }

    uint64_t GetThreadId(){
        return GetCurrentThreadId();
    }

    int32_t StringCompareCase(const std::string_view& dest,const std::string_view& src){
        return strcmp(dest.data(),src.data());
    }

    int32_t StringCompareNoCase(const std::string_view& dest,const std::string_view& src){
        return stricmp(dest.data(),src.data());
    }
}
#endif
