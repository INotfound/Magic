#include "Magic.h"
#include "asio.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "IoPool.h"
#include "Http/HttpParser.h"

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

void Http(){
	Magic::Http::HttpRequest req;
	Magic::Http::HttpRequest::KeyValue value;
	value.emplace("Host","www.XiaoBaiJun.com");
	req.setHeaders(value);
	req.setBody("hello XiaoBaiJun");
	req.setMethod(Magic::Http::HttpMethod::POST);
	req.dump(std::cout) << std::endl;

	Magic::Http::HttpResponse resp;
	Magic::Http::HttpResponse::KeyValue resValue;
	resValue.emplace("X-X","XIAOBAIJUN");
	resValue.emplace("Content-Length", "23333");
	resp.setHeaders(resValue);
	resp.setBody("hello XiaoBaiJun");
	resp.setStatus(Magic::Http::HttpStatus::BAD_REQUEST);
	resp.dump(std::cout) << std::endl;
}
char testRequestData[] = "GET / HTTP/1.1\r\n"
						"Host: www.top.com\r\n"
						"Content-Length: 10\r\n\r\n"
						"1234567890";
void HttpParser(){
	Magic::Http::HttpRequestParser requestParser;
	std::string tmp {testRequestData,68};
	uint32_t s = requestParser.execute(testRequestData,tmp.size());
	MAGIC_LOG(Magic::LogLevel::LogInfo) << "execute rt= " <<  s
		<<" has_error= " << requestParser.hasError()
		<<" Finished= " << requestParser.isFinished();
	std::string val{""};
	MAGIC_LOG(Magic::LogLevel::LogInfo) << requestParser.getData()->hasHeader("Content-Length",val);
	MAGIC_LOG(Magic::LogLevel::LogInfo) << val.c_str();
	requestParser.getData()->dump(std::cout) << std::endl;
	std::cout << testRequestData << std::endl;
}
int main() {
	Magic::Init("test");
	Http();
	HttpParser();
	std::getchar();
	return 0;
}