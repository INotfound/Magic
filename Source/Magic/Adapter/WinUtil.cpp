/*
 * @File: 
 * @Author: INotFound
 * @Date: 2020-05-09 09:39:20
 * @LastEditTime: 2020-06-23 09:00:35
 */
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
    bool ReadFileList(const std::string& basePath, std::vector<std::string>& paths){
        char findFilename[256];
        memset(findFilename, 0, 256);
        sprintf_s(findFilename, "%s\\*.*", basePath.c_str());
        WIN32_FIND_DATAA findFileData;
        HANDLE hFile = FindFirstFileA(findFilename, &findFileData);
        if (INVALID_HANDLE_VALUE == hFile){
            MAGIC_DEBUG()  << "Open dir error...";
            return false;
        }
        char temp[256];
        while (true){
            if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
                if (findFileData.cFileName[0] != '.'){
                    memset(temp, 0, 256);
                    sprintf_s(temp, "%s\\%s", basePath.c_str(), findFileData.cFileName);
                    ReadFileList(temp, paths);
                }
            }
            else{
                memset(temp, 0, 256);
                sprintf_s(temp, "%s\\%s", basePath.c_str(), findFileData.cFileName);
                paths.push_back(temp);
            }
            if (!FindNextFileA(hFile, &findFileData)){
                break;
            }
        }
        return true;
    }
    static void BackTrace(std::vector<std::string>& vec, uint32_t size, uint32_t skip) {
        //uint32_t i;
        //void* stack[size];
        //uint16_t frames;
        //SYMBOL_INFO* symbol;
        //HANDLE process;
        //process = GetCurrentProcess();
        //SymInitialize(process,NULL,TRUE);
        //frames = CaptureStackBackTrace(skip, size,stack, NULL) ;
        //symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        //symbol->MaxNameLen =255;
        //symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        //for (i = 0; i < frames; i++){
        //    SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        //    vec.push_back(symbol->Name);
        //}
        //SymCleanup(process);
        //free(symbol);
    }
    std::string BackTraceToString(uint32_t size, uint32_t skip, const std::string& prefix) {
        std::vector<std::string> vecString;
        BackTrace(vecString, size, skip);
        std::stringstream ss;
        for (size_t i = 0; i < vecString.size(); i++){
            ss << prefix << vecString.at(i) << std::endl;
        }
        ss << std::endl;
        return ss.str();
    }
    int32_t StringCompareNoCase(const std::string& dest,const std::string& src){
        return stricmp(dest.c_str(), src.c_str());
    }
}
#endif