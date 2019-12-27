#include "../Magic.h"
#include <iostream>

class IPlugin {
public:
	// Free Load Library;
	~IPlugin() {};
	int arg() {};
};


int main() {
	Magic::Init("test");
	int64_t s = MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
	MagicPtr<Magic::Plugin> plugin{ new Magic::Plugin("TestLib", "TestLib.dll") };
	MAGIC_PLUGIN()->addPlugin(plugin);
	MagicPtr<IPlugin> pp{ MAGIC_PLUGINEX("TestLib")->getInstance<IPlugin>() };
	std::cout << s;
	std::getchar();
	return 0;
}