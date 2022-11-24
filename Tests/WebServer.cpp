#include "Magic/Magic"
#include "Magic/Core/Container.h"
#include "Magic/NetWork/Http/Uri.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpClient.h"

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

    void websocket(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){
        g_webSocket = httpSocket->upgradeWebSocket(httpSocket->getRequest(),httpSocket->getResponse());
        g_webSocket->sendTextMessage("xxxxxx");
        g_webSocket->recvTextMessage([](const Safe<WebSocket>& socket,const std::string& msg){
            MAGIC_DEBUG() << msg;
            socket->sendTextMessage(msg);
        });
        g_webSocket->disconnectedCallBack([](const Safe<WebSocket>& socket){
            MAGIC_DEBUG() << "disconnected";
        });
    }

    void handle1(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){
        auto& response = httpSocket->getResponse();
//        auto httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
//        auto httpClient = std::make_shared<Magic::NetWork::Http::HttpClient>("http://www.baidu.com/",10000);
//        httpClient->onTimeOut([](){
//            MAGIC_DEBUG() << "Time Out";
//        })->onResponse([httpSocket,response](const Safe<Magic::NetWork::Http::HttpResponse>& httpResponse){
//            response->setBody(httpResponse->getBody())
//            ->setStatus(HttpStatus::OK);
//           httpSocket->sendResponse(response);
//        });
//        httpClient->execute(httpRequest);

            response->setBody("HELLO WORD!")
            ->setStatus(HttpStatus::OK);
           httpSocket->sendResponse(response);
    }
};

class Aop{
public:
    bool before(const Safe<HttpSocket>& httpSocket){ std::cout << "111111111111" << std::endl; return true;};
    bool after(const Safe<HttpSocket>& httpSocket){  std::cout << "222222222222" << std::endl; return true;};
};
class AopEx{
public:
    bool before(const Safe<HttpSocket>& httpSocket){ std::cout << "XXXXXXXXXXXX" << std::endl; return true;};
    bool after(const Safe<HttpSocket>& httpSocket){  std::cout << "SSSSSSSSSSSS" << std::endl; return true;};
};

class AopGlobal{
public:
    bool before(const Safe<HttpSocket>& httpSocket){ return true;};
//    bool after(const Safe<HttpSocket>& httpSocket){  std::cout << "BBBBBBBBBB" << std::endl; return true;};
};

class AopGlobalEx{
public:
//    bool before(const Safe<HttpSocket>& httpSocket){ std::cout << "HHHHHHHHHH" << std::endl; return true;};
    bool after(const Safe<HttpSocket>& httpSocket){ return true;};
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
    m_Container->registerTypeEx<Magic::NetWork::Http::IHttpServlet,ResourceServlet>();
    m_Container->registerType<Magic::NetWork::Http::HttpServer,Safe<Magic::NetWork::IoPool>,Safe<Magic::Config>>();

    m_Container->registerType<Aop>();
    m_Container->registerType<AopEx>();
    m_Container->registerType<AopGlobal>();
    m_Container->registerType<AopGlobalEx>();

    if(callback)
        callback(m_Container);

    auto logger = m_Container->resolve<Magic::Logger>();
    logger->externMode();
    for(auto& v : m_Container->resolveAll<Magic::ILogAppender>()){
        logger->addILogAppender(v);
    }

    m_Container->resolve<Magic::NetWork::IoPool>()->externMode();

    auto timingWheel = m_Container->resolve<TimingWheel>();
    timingWheel->externMode();
    timingWheel->run();

    auto dispatch = m_Container->resolve<Magic::NetWork::Http::HttpServletDispatch>();
    for(auto& v : m_Container->resolveAll<Magic::NetWork::Http::IHttpServlet>()){
        dispatch->setHttpServlet(v);
    }

    auto httpServer = m_Container->resolve<Magic::NetWork::Http::HttpServer>();
    httpServer->setServletDispatch(dispatch);
    httpServer->run();

    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/",&ResourceServlet::handle1);
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/1",&ResourceServlet::handle1,m_Container->resolve<Aop>());
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/2",&ResourceServlet::handle1,m_Container->resolve<Aop>(),m_Container->resolve<AopEx>());

    m_Container->resolve<Magic::NetWork::Http::HttpServletDispatch>()->addGlobalAspect(m_Container->resolve<AopGlobal>(),m_Container->resolve<AopGlobalEx>());

    m_Container->resolveAll();
    return m_Container;
}

int main(int /*argc*/,char** /*argv*/){
    Safe<Magic::Application> application = std::make_shared<Magic::Application>();
    application->initialize([](const Safe<Magic::Container>& ioc){

    });
    return EXIT_SUCCESS;
}