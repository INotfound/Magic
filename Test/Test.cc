#include "../Magic.h"
#include <bits/stdint-uintn.h>
#include <iostream>
#include <string>
#include <vector>

class IPluginModule {
public:
	virtual int arg() = 0;
	virtual ~IPluginModule() {};
};

void Plugin(){
    MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
	MagicPtr<Magic::Plugin> plugin{ 
		new Magic::Plugin("TestLib", MAGIC_CONFIG()->at<std::string>("Library","TestLib.DLL"))
	};
	MAGIC_PLUGINMGR()->addPlugin(plugin);
	MagicPtr<IPluginModule> pp{ MAGIC_PLUGIN("TestLib")->getInstance<IPluginModule>() };
	MAGIC_LOG(Magic::LogLevel::LogInfo) << pp->arg();
}

uint32_t i {0};
void RunInFiber(){
    i++;
    Magic::Fiber::GetThis()->back();
    MAGIC_LOG(Magic::LogLevel::LogDebug) << " number: " << i;
}

MagicPtr<Magic::Fiber> g_mainFiber;
Magic::Mutex mutex;
void Fiber(){
    MAGIC_LOG(Magic::LogLevel::LogDebug) << "Fiber Begin";
    {
        Magic::Fiber::Root();
        MagicPtr<Magic::Fiber> fiber(new Magic::Fiber(RunInFiber,true));
        fiber->call();
        MAGIC_LOG(Magic::LogLevel::LogDebug) << "Fiber SwapIn";
        fiber->call();
    }
    MAGIC_LOG(Magic::LogLevel::LogDebug) << "Fiber End";
}

int main() {
	Magic::Init("test");
    Fiber();
	std::getchar();
	return 0;
}