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

// extern "C"
// {
// 	_declspec(dllexport) void* create()
// 	{
// 		return new PluginModule;
// 	}
// }


extern "C"
{
	void* create()
	{
		return new PluginModule;
	}
}