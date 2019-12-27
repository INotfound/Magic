#pragma once 
#include <memory>



class IPlugin {
public:
	// Free Load Library;
	~IPlugin(){}
	int arg() {
		return 6666666;
	}
};

extern "C"
{
	_declspec(dllexport) void* Create()
	{
		return new std::unique_ptr<IPlugin>;
	}
}