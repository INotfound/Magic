/*
 * @Author: INotFound
 * @Date: 2020-09-12 21:40:10
 * @LastEditTime: 2021-02-01 22:33:09
 */
#if defined(linux) || defined(__linux__)
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <strings.h>

#include "Magic/Core/Core.h"
#include "Magic/Core/Adapter.h"
#include "Magic/Utilty/Timer.h"
#include "Magic/Utilty/Logger.h"

namespace Magic {
    uint64_t GetCurrentTimeMS() {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000ul  + tv.tv_usec / 1000;
    }
    
    uint64_t GetCurrentTimeUS(){
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000 * 1000ul  + tv.tv_usec;
    }

    uint32_t GetProcessorsNumber() {
        unsigned int count = 1;
        count = sysconf(_SC_NPROCESSORS_CONF);
        return count;
    }

    uint64_t GetThreadId() {
        return syscall(SYS_gettid);
    }

    int32_t StringCompareNoCase(const std::string& dest,const std::string& src){
        return strcasecmp(dest.c_str(), src.c_str());
    }
}
#endif
