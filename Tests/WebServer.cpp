#include "Magic/Magic"
#include "Magic/Core/Container.hpp"
#include "Magic/NetWork/Http/Uri.hpp"
#include "Magic/NetWork/Http/Http.hpp"
#include "Magic/NetWork/Http/HttpClient.hpp"

#include "Magic/NetWork/Http/HttpServer.hpp"
#include "Magic/Utilty/Logger.hpp"
#include "Magic/Utilty/TimingWheel.hpp"
#include "Magic/DataBase/ConnectionPool.hpp"
#include "Magic/Core/Adapter.hpp"
#include "Magic/Utilty/Trace.hpp"
#include "Magic/NetWork/TcpClient.hpp"
#include "Magic/NetWork/Http/HttpClient.hpp"
#include "Magic/Core/Except.hpp"
#include "Magic/Core/StringView.hpp"

using namespace Magic::NetWork::Http;

//#define leak

#ifdef leak
std::atomic_int newNum(0);

void* operator new(std::size_t size)
{
    newNum++;
    std::printf("Pointer Num: %d\n",newNum.load());
    return std::malloc(size);
}

void operator delete(void* ptr)
{
    newNum--;
    std::printf("Pointer Num: %d\n",newNum.load());
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
        g_webSocket->recvTextMessage([](const Safe<WebSocket>& socket,const std::string_view& msg){
            MAGIC_DEBUG() << msg;
            socket->sendTextMessage(msg);
        });
        g_webSocket->disconnectedCallBack([](const Safe<WebSocket>& socket){
            MAGIC_DEBUG() << "disconnected";
        });
    }
    void handle3(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){
        std::ifstream fileSteam;
        auto& response = httpSocket->getResponse();
        fileSteam.open("log.json",std::ios::in|std::ios::binary);
        if(fileSteam.is_open()){
            response->setStatus(HttpStatus::OK);
            response->setBody(std::string(std::istreambuf_iterator<char>(fileSteam),std::istreambuf_iterator<char>()));
        }else{
            response->setStatus(HttpStatus::NOT_FOUND);
        };

        httpSocket->sendResponse(response);
    }

    void handle2(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){
        auto& response = httpSocket->getResponse();
        response->setStatus(HttpStatus::OK);
        response->setResource("log.json");
        httpSocket->sendResponse(response);
    }

    void handle1(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){
        auto& response = httpSocket->getResponse();
        std::cout << httpSocket->getRequest();
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
//        throw Magic::Failure("data sell");

        response->setStatus(HttpStatus::OK)->setBody("hello world");
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

const Safe<Magic::Container>& Magic::Application::initialize(std::function<void(const Safe<Container>&)> callback){
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
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/ws",&ResourceServlet::websocket);
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/1",&ResourceServlet::handle1,m_Container->resolve<Aop>());
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/2",&ResourceServlet::handle1,m_Container->resolve<Aop>(),m_Container->resolve<AopEx>());
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/3",&ResourceServlet::handle2);
    m_Container->resolve<Magic::NetWork::Http::IHttpServlet,ResourceServlet>()->addRoute("/4",&ResourceServlet::handle3);

    m_Container->resolve<Magic::NetWork::Http::HttpServletDispatch>()->addGlobalAspect(m_Container->resolve<AopGlobal>(),m_Container->resolve<AopGlobalEx>());

    MAGIC_DEBUG() << "1";
    MAGIC_ERROR() << "2";
    MAGIC_FATAL() << "3";
    MAGIC_WARN() << "4";

    m_Container->resolveAll();
    return m_Container;
}


void printff(const std::string& str){
    std::cout << str << std::endl;
}

void pprintf(const std::string_view& view){
    printff(view.data());
}


int main(int /*argc*/,char** /*argv*/){
    std::cout << __cplusplus << std::endl;
    Safe<Magic::Application> application = std::make_shared<Magic::Application>();
    application->initialize([](const Safe<Magic::Container>& ioc){
    });
    return EXIT_SUCCESS;
}