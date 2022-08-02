#include "Magic/Core/Container.h"
#include "Magic/NetWork/Http/Uri.h"
#include "Magic/NetWork/Http/Http.h"

#include "Magic/NetWork/Http/HttpServer.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/TimingWheel.h"
#include "Magic/DataBase/ConnectionPool.h"
#include "Magic/Core/Adapter.h"
#include "Magic/Utilty/Trace.h"
#include "Magic/NetWork/TcpClient.h"
#include "Magic/NetWork/Http/HttpClient.h"

using namespace Magic::NetWork::Http;

std::atomic_int newNum(0);

void* operator new(std::size_t size)
{
    newNum++;
    std::cout << "Pointer Num: " << newNum << std::endl;
    return std::malloc(size);
}

void operator delete(void* ptr)
{
    newNum--;
    std::cout << "Pointer Num: " << newNum << std::endl;
    std::free(ptr);
}

Safe<WebSocket> g_webSocket;
class ResourceServlet :public Magic::NetWork::Http::IHttpServlet{
public:
    ResourceServlet(){
    }

    void websocket(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket
            ,const Safe<Magic::NetWork::Http::HttpRequest>& request
            ,const Safe<Magic::NetWork::Http::HttpResponse>& response){
        g_webSocket = httpSocket->upgradeWebSocket(request,response);
        g_webSocket->sendTextMessage("xxxxxx");
        g_webSocket->recvTextMessage([](const Safe<WebSocket>& socket,const std::string& msg){
            MAGIC_DEBUG() << msg;
            socket->sendTextMessage(msg);
        });
        g_webSocket->disconnectedCallBack([](const Safe<WebSocket>& socket){
            MAGIC_DEBUG() << "disconnected";
        });
    }

    void handle1(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket,const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response){
        response->setBody("Hello World")
                ->setStatus(HttpStatus::OK);
        httpSocket->sendResponse(response);
    }
};

int main(int /*argc*/,char** /*argv*/){
    /// Config
    Safe<Magic::IConfigFormatter>  formatter = std::make_shared<Magic::InIConfigFormatter>();
    Safe<Magic::ConfigFile> configFile = std::make_shared<Magic::ConfigFile>(formatter);
    Safe<Magic::Config> config = std::make_shared<Magic::Config>(configFile);
    /// Logger
    Magic::g_Logger = std::make_shared<Magic::Logger>(config);
    Safe<Magic::ILogAppender> logAppender = std::make_shared<Magic::StdOutLogAppender>();
    Magic::g_Logger->addILogAppender(logAppender);
    /// TimingWheel
    Safe<Magic::NetWork::IoPool> pool = std::make_shared<Magic::NetWork::IoPool>(config);
    Magic::g_TimingWheel = std::make_shared<Magic::TimingWheel>(config);
    Magic::g_TimingWheel->run();
    /// WebServer
    Magic::NetWork::Http::HttpServer server(pool,config);
    Safe<Magic::NetWork::Http::IHttpServlet> resservlet = std::make_shared<ResourceServlet>();
    Safe<Magic::NetWork::Http::HttpServletDispatch> dispatch = std::make_shared<Magic::NetWork::Http::HttpServletDispatch>();

    dispatch->setHttpServlet(resservlet);
    resservlet->addRoute("/",&ResourceServlet::handle1);
    resservlet->addRoute("/chat",&ResourceServlet::websocket);
    server.setServletDispatch(dispatch);
    MAGIC_DEBUG() << "WebServer Testing...";
    server.run();
    return 0;
}