#include "../Magic.h"
#include <iostream>

class IPluginModule {
public:
	virtual int arg() = 0;
	virtual ~IPluginModule() {};
};


int main() {
	Magic::Init("test");
	MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
	MagicPtr<Magic::Plugin> plugin{ 
		new Magic::Plugin("TestLib", MAGIC_CONFIG()->at<std::string>("Library","TestLib.DLL"))
	};
	MAGIC_PLUGINMGR()->addPlugin(plugin);
	MagicPtr<IPluginModule> pp{ MAGIC_PLUGIN("TestLib")->getInstance<IPluginModule>() };
	MAGIC_LOG(Magic::LogLevel::LogInfo) << pp->arg();
	std::getchar();
	return 0;
}