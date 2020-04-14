/*
 * @file: Test_Plugin.cpp
 * @Author: INotFound
 * @Date: 2020-03-16 00:15:10
 * @LastEditTime: 2020-03-16 00:25:49
 */
#include <iostream>
#include "Magic.h"
class IPluginModule {
public:
    virtual int arg() = 0;
    virtual ~IPluginModule() {};
};

void Plugin(){
    auto config = Magic::Config::GetInstance();
    Safe<Magic::Plugin> plugin{ 
        // so库要使用绝对路径
        new Magic::Plugin("TestLib", config->at<std::string>("Library","/home/magic/WorkSpace/Magic/build/libTest_PluginLib.so"))
    };
    auto& pluginMgr = Magic::PluginMgr::GetInstance();
    pluginMgr->addPlugin(plugin);
    Safe<IPluginModule> pp(pluginMgr->at("TestLib")->getInstance<IPluginModule>());
    MAGIC_INFO() << pp->arg();
}

int main(){
    Magic::Init("Test_Plugin");
    Plugin();
    getchar();
    return 0;
}