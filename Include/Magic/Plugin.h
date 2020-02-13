#pragma once
#include "Core.h"
#include "Adapter.h"

/*

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
 extern "C"
 {
     _declspec(dllexport) void* create()
     {
         return new PluginModule;
     }
 }
#endif

#if defined(linux) || defined(__linux__)
//Linux
extern "C"
{
    void* create()
    {
        return new PluginModule;
    }
}
#endif

*/
namespace Magic {
    class Plugin;
    class PluginManager;
    typedef void* (*create_t)();

    class Plugin {
    public:
        ~Plugin();
        Plugin(const std::string& name,const std::string& path);
        std::string getName() const;
        template<class T>
        Safe<T> getInstance() {
            return Safe<T>(static_cast<T*>(m_Create()));
        }
    private:
        std::string m_Name;
        create_t m_Create;
        plugin_t m_Handle;
        bool m_IsInstance;
    };

    class PluginManager {
    public:
        PluginManager();
        void addPlugin(Safe<Plugin>&);
        void delPlugin(Safe<Plugin>&);
        Safe<Plugin>& at(const std::string& name);
        std::map<std::string, Safe<Plugin>>& all();
    private:
        std::map<std::string, Safe<Plugin>> m_PluginMap;
    };

}