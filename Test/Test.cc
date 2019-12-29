#include "../Magic.h"
#include <iostream>

class IPluginModule {
public:
	virtual int arg() = 0;
	virtual ~IPluginModule() {};
};


int main() {
	Magic::Init("test");
	int64_t s = MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
	MagicPtr<Magic::Plugin> plugin{ new Magic::Plugin("TestLib", "TestLib.dll") };
	MAGIC_PLUGINMGR()->addPlugin(plugin);
	MagicPtr<IPluginModule> pp{ MAGIC_PLUGIN("TestLib")->getInstance<IPluginModule>() };
	std::cout << pp->arg();
	std::getchar();
	return 0;
}