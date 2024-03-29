/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Utils.cpp
 * @Date           : 2023-07-03 18:32
 ******************************************************************************
 */
#if defined(_WIN32) || defined(_WIN64)

#include "Magic/Core/Core.hpp"
#include "Magic/Core/Adapter.hpp"

namespace Magic{
    uint64_t GetThreadId(){
        return GetCurrentThreadId();
    }

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

    bool IsFile(const StringView& path){
        struct stat fileStat;
        bool isOk = stat(path.data(),&fileStat) == 0;
        if(isOk){
            isOk = S_ISREG(fileStat.st_mode);
        }
        return isOk;
    }
}
#endif