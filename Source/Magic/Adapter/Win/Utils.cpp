/*
 * @Author: INotFound
 * @Date: 2020-05-09 09:39:20
 * @LastEditTime: 2021-02-01 22:33:42
 */
#if defined(_WIN32) || defined(_WIN64)

#include "Magic/Core/Core.h"
#include "Magic/Core/Adapter.h"

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

    bool IsFile(const std::string_view& path){
        struct stat fileStat;
        bool isOk = stat(path.data(),&fileStat) == 0;
        if(isOk){
            isOk = S_ISREG(fileStat.st_mode);
        }
        return isOk;
    }

    int32_t StringCompareCase(const std::string_view& dest,const std::string_view& src){
        return strncmp(dest.data(),src.data(),std::min(dest.size(),src.size()));
    }

    int32_t StringCompareNoCase(const std::string_view& dest,const std::string_view& src){
        return strnicmp(dest.data(),src.data(),std::min(dest.size(),src.size()));
    }
}
#endif