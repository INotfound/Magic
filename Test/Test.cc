#include "Magic.h"
#include "asio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include <vector>
#include "IoPool.h"
#include "Http/HttpParser.h"
#include "TcpServer.h"
#include "Http/Http.h"
#include "Http/HttpServlet.h"
#include "Http/HttpServer.h"
#include "Http/Session.h"
#include "Http/MultiPart.h"
#include "Http/HttpFile.h"
#include "Any.h"
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
            :HttpServlet("FileServlet"){
        }
        bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
            Magic::Http::MultiPart multiPart;
            multiPart.parse(request);
            std::cout << multiPart.getParams().count("xxxx") << std::endl;
            auto fileIter = multiPart.getFiles().begin();
            auto fileEnd = multiPart.getFiles().end();
            for(;fileIter!=fileEnd; fileIter++){
                (*fileIter)->save("www/Image/" + (*fileIter)->getName());
            }
            response->setStatus(Magic::Http::HttpStatus::OK);
            response->setBody("OK!!!");
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
            std::ifstream stream;
            std::string res    = "www";
            std::string path = request->getPath();
            if(path == "/"){
                path = "/index.html";
            }
            stream.open(res + path,std::ios::in);
            if(stream.is_open()){
                std::ostringstream staticRes;
                staticRes << stream.rdbuf();
                response->setContentType(Magic::Http::FileTypeToHttpContentType(path));
                response->setBody(staticRes.str());
                return true;
            }
            return false;  
        }
};


void Server(){
    std::string ip = MAGIC_CONFIG()->at<std::string>("ServerIp","0.0.0.0");
    uint16_t port  = MAGIC_CONFIG()->at<uint16_t>("ServerPort",80);
    try{
        Magic::Http::HttpServer server(ip,port,Magic::GetProcessorsNumber()*2);
        Safe<Magic::Http::HttpServlet> log(new LogServlet);
        Safe<Magic::Http::HttpServlet> deafult(new DeafultServlet);
        Safe<Magic::Http::HttpServlet> file(new FileServlet);
        Safe<Magic::Http::HttpServlet> main(new MainServlet);
        server.getHttpServletDispatch()->setDeafultServlet(deafult);
        server.getHttpServletDispatch()->addHttpServlet("/log",log);
        server.getHttpServletDispatch()->addHttpServlet("/file",file);
        server.getHttpServletDispatch()->addGlobHttpServlet("^/?(.*)$",main);
        MAGIC_CONFIG()->update();
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