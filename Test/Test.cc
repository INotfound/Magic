#include "Magic.h"
#include "asio.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "IoPool.h"
#include "Http/HttpParser.h"
#include "TcpServer.h"
#include "Http/Http.h"
#include "Http/HttpServlet.h"
#include "Http/HttpServer.h"
#include "Http/HttpSession.h"
class IPluginModule {
public:
	virtual int arg() = 0;
	virtual ~IPluginModule() {};
};

void Plugin(){
    MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
	Safe<Magic::Plugin> plugin{ 
		new Magic::Plugin("TestLib", MAGIC_CONFIG()->at<std::string>("Library","TestLib.DLL"))
	};
	MAGIC_PLUGINMGR()->addPlugin(plugin);
	Safe<IPluginModule> pp{ MAGIC_PLUGIN("TestLib")->getInstance<IPluginModule>() };
	MAGIC_LOG(Magic::LogLevel::LogInfo) << pp->arg();
}

void Http(){
	Magic::Http::HttpRequest req;
	Magic::Http::HttpRequest::KeyValue value;
	value.emplace("Host","www.XiaoBaiJun.com");
	req.setHeaders(value);
	req.setBody("hello XiaoBaiJun");
	req.setMethod(Magic::Http::HttpMethod::POST);
	req.toStream(std::cout) << std::endl;

	Magic::Http::HttpResponse resp;
	Magic::Http::HttpResponse::KeyValue resValue;
	resValue.emplace("X-X","XIAOBAIJUN");
	resValue.emplace("Content-Length", "23333");
	resp.setHeaders(resValue);
	resp.setBody("hello XiaoBaiJun");
	resp.setStatus(Magic::Http::HttpStatus::BAD_REQUEST);
	resp.toStream(std::cout) << std::endl;
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
	requestParser.getData()->toStream(std::cout);
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
	responseParser.getData()->toStream(std::cout);
	std::cout << testResponseData << std::endl;
}
class DeafultServlet :public Magic::Http::HttpServlet{
	public:
		DeafultServlet()
			:HttpServlet("DeafultServlet"){
		}
		void handle (const Share<Magic::Http::HttpSession>& session,Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
			response->setStatus(Magic::Http::HttpStatus::NOT_FOUND);
			std::string notfound{R"Template(<html>
				<head><title>404 Not Found</title></head>
				<body>
				<center><h1>404 Not Found</h1></center>
				<hr><center>Magic/0.0.1</center>
				</body>
				</html>)Template"};
			response->setBody(notfound);
			return;
		}
};

class LogServlet :public Magic::Http::HttpServlet{
	public:
		LogServlet()
			:HttpServlet("LogServlet"){
		}
		void handle (const Share<Magic::Http::HttpSession>& session,Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
			response->setStatus(Magic::Http::HttpStatus::OK);
			std::fstream stream;
			response->setHeader("Content-type","text/html");
			stream.open("test.html",std::ios::in);
			stream.seekg(0,std::ios_base::end);
			uint32_t size = stream.tellg();
			Share<char> buffer(new char[size],[](char* ptr){delete[] ptr;});
			stream.seekg(0,std::ios_base::beg);
			stream.read(buffer.get(),size);
			std::string log{buffer.get(),size};
			response->setBody(log);
			return;
		}
};


void Server(){
	Magic::Http::HttpServer server("0.0.0.0",80,Magic::GetProcessorsNumber()*2);
	Safe<Magic::Http::HttpServlet> log{new LogServlet};
	Safe<Magic::Http::HttpServlet> deafult{new DeafultServlet};
	server.getHttpServletDispatch()->setDeafultServlet(deafult);
	server.getHttpServletDispatch()->addServlet("/log",log);
	server.run();
}

int main() {
	Magic::Init("test");
	Server();
	return 0;
}