#include "Magic/Magic"
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

#define leak

#ifdef leak
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
#endif

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
const Safe<Magic::Container>& Magic::Application::initialize(const std::function<void(const Safe<Container>&)>& callback){
    m_Container->registerType<Magic::Config,Safe<Magic::ConfigFile>>();
    m_Container->registerType<Magic::ConfigFile,Safe<Magic::IConfigFormatter>>();
    m_Container->registerTypeEx<Magic::IConfigFormatter,Magic::InIConfigFormatter>();

    m_Container->registerType<Magic::Logger,Safe<Magic::Config>>();
    m_Container->registerTypeEx<Magic::ILogAppender,Magic::StdOutLogAppender>();

    m_Container->registerType<Magic::NetWork::IoPool,Safe<Magic::Config>>();
    m_Container->registerType<Magic::TimingWheel,Safe<Magic::Config>>();

    m_Container->registerType<Magic::NetWork::Http::HttpServletDispatch>();
    m_Container->registerType<Magic::NetWork::Http::HttpServer,Safe<Magic::NetWork::IoPool>,Safe<Magic::Config>>();

    if(callback)
        callback(m_Container);

    auto logger = m_Container->resolve<Magic::Logger>();
    logger->externMode();
    for(auto& v : m_Container->resolveAll<Magic::ILogAppender>()){
        logger->addILogAppender(v);
    }

    auto timingWheel = m_Container->resolve<TimingWheel>();
    timingWheel->externMode();
    timingWheel->run();

    auto httpServer = m_Container->resolve<Magic::NetWork::Http::HttpServer>();
    httpServer->setServletDispatch(m_Container->resolve<Magic::NetWork::Http::HttpServletDispatch>());
    httpServer->run();

    m_Container->resolveAll();
    return m_Container;
}

int main(int /*argc*/,char** /*argv*/){
    Safe<Magic::Application> application = std::make_shared<Magic::Application>();
    application->initialize([](const Safe<Magic::Container>& ioc){
        ioc->registerTypeEx<Magic::NetWork::Http::IHttpServlet,ResourceServlet>();
    });
    return EXIT_SUCCESS;
}