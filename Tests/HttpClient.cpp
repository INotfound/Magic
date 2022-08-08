#define _WIN32_WINNT 0x0601
#include "Magic/Magic"
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/IoPool.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpClient.h"

using namespace Magic::NetWork::Http;

//#define leak

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
const Safe<Magic::Container>& Magic::Application::initialize(const std::function<void(const Safe<Container>&)>& callback){
    m_Container->registerType<Magic::Config,Safe<Magic::ConfigFile>>();
    m_Container->registerType<Magic::ConfigFile,Safe<Magic::IConfigFormatter>>();
    m_Container->registerTypeEx<Magic::IConfigFormatter,Magic::InIConfigFormatter>();

    m_Container->registerType<Magic::Logger,Safe<Magic::Config>>();
    m_Container->registerTypeEx<Magic::ILogAppender,Magic::StdOutLogAppender>();

    m_Container->registerType<Magic::NetWork::IoPool,Safe<Magic::Config>>();
    m_Container->registerType<Magic::TimingWheel,Safe<Magic::Config>>();

    if(callback)
        callback(m_Container);

    auto logger = m_Container->resolve<Magic::Logger>();
    logger->externMode();
    for(auto& v : m_Container->resolveAll<Magic::ILogAppender>()){
        logger->addILogAppender(v);
    }

    auto ioPool = m_Container->resolve<Magic::NetWork::IoPool>();
    ioPool->externMode();

    auto timingWheel = m_Container->resolve<TimingWheel>();
    timingWheel->externMode();
    timingWheel->run();

    m_Container->resolveAll();
    return m_Container;
}

int main(int /*argc*/,char** /*argv*/){
    {
        Safe<Magic::Application> application = std::make_shared<Magic::Application>();
        application->initialize();

        auto function1 = [](){};
        auto funciont2 = [function1](){};
        auto funciont3 = [function1,funciont2](){};
        std::function<void()> function4 = [function1,funciont2,funciont3](){};

        MAGIC_DEBUG() << "Size: "  << sizeof(function1)<< " " << sizeof(funciont2) << " " << sizeof(funciont3) << " " << sizeof(function4) << sizeof(std::function<void()>);

        std::printf("\n_______________________________________________________________\n");
        Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
        httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
        Safe<HttpClient> client = std::make_shared<HttpClient>("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=6f260538-b8d0-4c29-9492-6b04522ba3d3&debug=1",1000000);
        client->onTimeOut([](){
            MAGIC_WARN() << "Time Out";
        })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
            MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus()) << " " << response->getBody() << " end";
        })->execute(httpRequest);

//#define looper
#ifdef looper
        std::getchar();
        for(auto i = 0; i < 10; i++){
            Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
            httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
            Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
            client->onTimeOut([](){
                MAGIC_WARN() << "Time Out";
            })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
                MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
            })->execute(httpRequest);
        }
        std::getchar();
        for(auto i = 0; i < 10; i++){
            Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
            httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
            Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
            client->onTimeOut([](){
                MAGIC_WARN() << "Time Out";
            })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
                MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
            })->execute(httpRequest);
        }
        std::getchar();
        for(auto i = 0; i < 10; i++){
            Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
            httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
            Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
            client->onTimeOut([](){
                MAGIC_WARN() << "Time Out";
            })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
                MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
            })->execute(httpRequest);
        }
        std::getchar();
        for(auto i = 0; i < 10; i++){
            Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
            httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
            Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
            client->onTimeOut([](){
                MAGIC_WARN() << "Time Out";
            })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
                MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
            })->execute(httpRequest);
        }
        std::getchar();
        ioc->resolve<Magic::NetWork::IoPool>()->stop();
#endif
    }
    std::printf("\n________________________________+_______________________________\n");
    std::getchar();
    return EXIT_SUCCESS;
}