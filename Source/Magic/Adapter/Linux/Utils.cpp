/*
 * @Author: INotFound
 * @Date: 2020-09-12 21:40:10
 * @LastEditTime: 2021-02-01 22:33:09
 */
#if defined(linux) || defined(__linux__)

#include <cstring>

#include "Magic/Core/Core.hpp"
#include "Magic/Core/Adapter.hpp"

namespace Magic{
    uint64_t GetThreadId(){
        return syscall(SYS_gettid);
    }

    uint64_t GetCurrentTimeMS(){
        struct timeval tv;
        gettimeofday(&tv,nullptr);
        return tv.tv_sec * 1000ul + tv.tv_usec / 1000;
    }

    uint64_t GetCurrentTimeUS(){
        struct timeval tv;
        gettimeofday(&tv,nullptr);
        return tv.tv_sec * 1000 * 1000ul + tv.tv_usec;
    }

    bool IsFile(const Magic::StringView& path){
        struct stat fileStat;
        bool isOk = stat(path.data(),&fileStat) == 0;
        if(isOk){
            isOk = S_ISREG(fileStat.st_mode);
        }
        return isOk;
    }
}
#endif
