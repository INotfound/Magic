#include "Magic.h"
#include "asio.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "IoPool.h"
#include "Http/HttpParser.h"
#include "TcpServer.h"
#include "asio.hpp"
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
void HttpRequestParser(){
	Magic::Http::HttpRequestParser requestParser;
	std::string tmp {testRequestData,68};
	uint32_t s = requestParser.execute(testRequestData,tmp.size());
	MAGIC_LOG(Magic::LogLevel::LogInfo) << "execute rt= " <<  s
		<<" has_error= " << requestParser.hasError()
		<<" Finished= " << requestParser.isFinished();
	std::string val{""};
	MAGIC_LOG(Magic::LogLevel::LogInfo) << requestParser.getData()->hasHeader("Content-Length",val);
	MAGIC_LOG(Magic::LogLevel::LogInfo) << val.c_str();
	requestParser.getData()->dump(std::cout);
	std::cout << testRequestData << std::endl;
}
char testResponseData[] = "HTTP/1.1 200 OK\r\n"
        "Date: Tue, 04 Jun 2019 15:43:56 GMT\r\n"
        "Server: Apache\r\n"
        "Last-Modified: Tue, 12 Jan 2010 13:48:00 GMT\r\n"
        "ETag: \"51-47cf7e6ee8400\"\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: 81\r\n"
        "Cache-Control: max-age=86400\r\n"
        "Expires: Wed, 05 Jun 2019 15:43:56 GMT\r\n"
        "Connection: Close\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html>\r\n"
        "<meta http-equiv=\"refresh\" content=\"0;url=http://www.baidu.com/\">\r\n"
        "</html>\r\n";
void HttpResponseParser(){
	Magic::Http::HttpResponseParser responseParser;
	uint32_t s = responseParser.execute(testResponseData,385);
	MAGIC_LOG(Magic::LogLevel::LogInfo) << "execute rt= " <<  s
		<<" has_error= " << responseParser.hasError()
		<<" Finished= " << responseParser.isFinished()
		<<" Content-Length= "<< responseParser.getContentLength();
	responseParser.getData()->dump(std::cout);
	std::cout << testResponseData << std::endl;
}

char str[] = "HTTP/1.0 200 OK\r\n\r\n"
          "<html>hello from http server</html>";
class EchoServer : public Magic::TcpServer{
	public:
		EchoServer(std::string addr,uint16_t p,uint32_t s):Magic::TcpServer(addr,p,s){

		}
	protected:
		virtual void hanldeFunc(std::shared_ptr<asio::ip::tcp::socket> socket) {
		auto readbuf = std::make_shared<asio::streambuf>();
		asio::async_read(*socket,*readbuf,[this,readbuf](const asio::error_code& ec,size_t len){
			if(err){
                //TODO: ...
                MAGIC_LOG(Magic::LogLevel::LogWarn) << err.message();
                return;
            }
			std::cout.rdbuf(readbuf);
			MAGIC_LOG(Magic::LogLevel::LogInfo) << "Post!";
		});
	}
};


void Server(){
	EchoServer server("127.0.0.1",6060,Magic::GetProcessorsNumber()*2);
	server.run();
}

int main() {
	Magic::Init("test");
	Server();
	return 0;
}