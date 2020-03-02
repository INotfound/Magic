#if defined(linux) || defined(__linux__)
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <strings.h>

#include "Core.h"
#include "Macro.h"
#include "Adapter/Linux.h"

namespace Magic {
    
    uint64_t GetCurrentTimeMS() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000ul  + tv.tv_usec / 1000;
    }
    uint64_t GetCurrentTimeUS(){
        struct timeval tv;
        gettimeofday(&tv, NULL);
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
    bool ReadFileList(const std::string& basePath,std::vector<std::string>& paths){
        DIR *dir;
        char base[1000];
        struct dirent *direntPtr;
        if ((dir=opendir(basePath.c_str())) == NULL){
            MAGIC_LOG(Magic::LogLevel::LogDebug)  << "Open dir error...";
            return false;
        }
        
        std::string path;
        while ((direntPtr=readdir(dir)) != NULL){
            path.clear();
            if(strcmp(direntPtr->d_name,".")==0 || strcmp(direntPtr->d_name,"..")==0)    ///current dir OR parrent dir
                continue;
            else if(direntPtr->d_type == 8){
                path += basePath;
                path += "/";
                path += direntPtr->d_name;
                paths.push_back(path);
            }else if(direntPtr->d_type == 10){
                path += basePath;
                path += "/";
                path += direntPtr->d_name;
                paths.push_back(path);
            }
            if(direntPtr->d_type == 4)
            {
                memset(base,0,sizeof(base));
                strcpy(base,basePath.c_str());
                strcat(base,"/");
                strcat(base,direntPtr->d_name);
                ReadFileList(base,paths);
            }
        }
        closedir(dir);
        return true;
    }
    static void BackTrace(std::vector<std::string>& vec, uint32_t size, uint32_t skip) {
        void** buffer = static_cast<void**>(std::malloc(sizeof(void*) * size));
        int32_t retValue = backtrace(buffer, size);
        char** string = backtrace_symbols(buffer, retValue);
        if (string == nullptr) {
            MAGIC_LOG(Magic::LogLevel::LogError) << "backtrace_symbols error!";
        }
        uint32_t newValue = static_cast<uint32_t>(retValue);
        for (uint32_t i = skip; i < newValue; i++) {
            vec.push_back(string[i]);
        }
        free(buffer);
    }

    std::string BackTraceToString(uint32_t size, uint32_t skip, const std::string& prefix) {
        std::vector<std::string> vecString;
        BackTrace(vecString, size, skip);
        std::stringstream ss;
        for (size_t i = 0; i < vecString.size(); i++)
        {
            ss << prefix << vecString.at(i) << std::endl;
        }
        ss << std::endl;
        return ss.str();
    }

    int32_t StringCompareNoCase(const std::string& dest,const std::string& src){
        return strcasecmp(dest.c_str(), src.c_str());
    }
}
#endif
