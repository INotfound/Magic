#include "Magic.h"
#include "asio.hpp"
#include <iostream>
#include <fstream>
#include <exception>
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
	Safe<IPluginModule> pp(MAGIC_PLUGIN("TestLib")->getInstance<IPluginModule>());
	MAGIC_LOG(Magic::LogLevel::LogInfo) << pp->arg();
}

class DeafultServlet :public Magic::Http::HttpServlet{
	public:
		DeafultServlet()
			:HttpServlet("DeafultServlet"){
		}
		bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
			response->setStatus(Magic::Http::HttpStatus::NOT_FOUND);
			std::string notfound{R"Template(<html>
				<head><title>404 Not Found</title></head>
				<body>
				<center><h1>404 Not Found</h1></center>
				<hr><center>Magic/0.0.1</center>
				</body>
				</html>)Template"};
			response->setBody(notfound);
			return true;
		}
};


class LogServlet :public Magic::Http::HttpServlet{
	public:
		LogServlet()
			:HttpServlet("LogServlet"){
		}
		bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
			response->setStatus(Magic::Http::HttpStatus::OK);
			std::fstream stream;
			response->setHeader("Content-type","text/html");
			stream.open("test.html",std::ios::in);
			stream.seekg(0,std::ios_base::end);
			uint32_t size = stream.tellg();
			Share<char> buffer(new char[size],[](char* ptr){delete[] ptr;});
			stream.seekg(0,std::ios_base::beg);
			stream.read(buffer.get(),size);
			std::string log(buffer.get(),size);
			response->setBody(log);
			return true;
		}
};

class FileServlet :public Magic::Http::HttpServlet{
	public:
		FileServlet()
			:HttpServlet("LogServlet"){
		}
		bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
			std::cout << request->getBody() << std::endl;
			return true;
		}
};

class MainServlet :public Magic::Http::HttpServlet{
	public:
		MainServlet()
			:HttpServlet("MainServlet"){
		}
		bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
			response->setStatus(Magic::Http::HttpStatus::OK);
			std::fstream stream;
			std::string res	= "www";
			std::string path = request->getPath();
			if(path == "/"){
				path = "/index.html";
			}
			stream.open(res + path,std::ios::in);
			if(stream.is_open()){
				stream.seekg(0,std::ios_base::end);
				uint32_t size = stream.tellg();
				Share<char> buffer(new char[size],[](char* ptr){delete[] ptr;});
				stream.seekg(0,std::ios_base::beg);
				stream.read(buffer.get(),size);
				std::string staticres(buffer.get(),size);
				response->setContentType(Magic::Http::FileTypeToHttpContentType(path));
				response->setBody(staticres);
				return true;
			}
			return false;  
		}
};


void Server(){
	try{
		Magic::Http::HttpServer server("0.0.0.0",80,Magic::GetProcessorsNumber()*2);
		Safe<Magic::Http::HttpServlet> log(new LogServlet);
		Safe<Magic::Http::HttpServlet> deafult(new DeafultServlet);
		Safe<Magic::Http::HttpServlet> file(new FileServlet);
		Safe<Magic::Http::HttpServlet> main(new MainServlet);
		server.getHttpServletDispatch()->setDeafultServlet(deafult);
		server.getHttpServletDispatch()->addHttpServlet("/log",log);
		server.getHttpServletDispatch()->addHttpServlet("/file",log);
		server.getHttpServletDispatch()->addGlobHttpServlet("^/?(.*)$",main);
		server.run();
	}catch(std::system_error ec){
		std::cout << ec.what() << std::endl;
	}
}

int main() {
	Magic::Init("test");
	Server();
	return 0;
}