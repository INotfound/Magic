/*
 * @file: Plugin.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:25:49
 * @LastEditTime: 2020-03-14 01:40:36
 */
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
    /// 函数类型
    typedef void* (*create_t)();
    /**
     * @brief: 插件类
     */
    class Plugin {
    public:
        /**
         * @brief: 虚构函数
         */
        ~Plugin();
        /**
         * @brief: 构造函数
         */
        Plugin(const std::string& name,const std::string& path);
        /**
         * @brief: 获取插件名称
         * @return: 返回插件名称
         */
        std::string getName() const;
        /**
         * @brief: 获取实体接口的对象
         * @param T 接口类型
         * @return: 返回接口对象
         */
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
namespace Instance{
    /**
     * @brief: 插件管理器类
     */
    class PluginManager {
    public:
        /**
         * @brief: 构造函数
         */
        PluginManager();
        /**
         * @brief: 添加插件函数
         * @param plugin 需添加的插件
         */
        void addPlugin(Safe<Plugin>& plugin);
        /**
         * @brief: 删除插件函数
         * @param plugin 需删除的插件
         */
        void delPlugin(Safe<Plugin>& plugin);
        /**
         * @brief: 获取插件函数
         * @param name 插件名称 
         * @return: 返回插件
         */
        const Safe<Plugin>& at(const std::string& name);
        /**
         * @brief: 获取所以的插件
         * @return: 返回所有插件的容器
         */
        std::map<std::string, Safe<Plugin>>& all();
    private:
        std::map<std::string, Safe<Plugin>> m_PluginMap;
    };
}
    typedef SingletonPtr<Instance::PluginManager> PluginMgr;
}
