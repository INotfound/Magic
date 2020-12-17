/*
 * @File: Test_PluginLib.cpp
 * @Author: INotFound
 * @Date: 2020-03-16 00:16:18
 * @LastEditTime: 2020-03-16 00:16:31
 */
#include <memory>

class IPluginModule {
public:
    virtual int arg() =0;
    virtual ~IPluginModule() {}
};


class PluginModule:public IPluginModule {
public:
    ~PluginModule() {}
    PluginModule() {}
    int arg() override{
        return 6666666;
    }
};


#if defined(_WIN32) || defined(_WIN64)
//Win
#   define DLL_API extern "C" __declspec(dllexport)
    DLL_API void* create(){
        return new PluginModule;
    }
#endif

#if defined(linux) || defined(__linux__)
#   define DLL_API extern "C"
//Linux
    DLL_API void* create(){
        return new PluginModule;
    }
#endif