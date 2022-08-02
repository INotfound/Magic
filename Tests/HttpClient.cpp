#define _WIN32_WINNT 0x0601
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/IoPool.h"
#include "Magic/NetWork/Http/Http.h"
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

int main(int /*argc*/,char** /*argv*/){
    /// Config
    Safe<Magic::IConfigFormatter>  formatter = std::make_shared<Magic::InIConfigFormatter>();
    Safe<Magic::ConfigFile> configFile = std::make_shared<Magic::ConfigFile>(formatter);
    Safe<Magic::Config> config = std::make_shared<Magic::Config>(configFile);
    /// Logger
    Magic::g_Logger = std::make_shared<Magic::Logger>(config);
    Safe<Magic::ILogAppender> logAppender = std::make_shared<Magic::StdOutLogAppender>();
    Magic::g_Logger->addILogAppender(logAppender);
    /// IoPool
    Safe<Magic::NetWork::IoPool> pool = std::make_shared<Magic::NetWork::IoPool>(config);
    /// TimingWheel
    Magic::g_TimingWheel = std::make_shared<Magic::TimingWheel>(config);
    Magic::g_TimingWheel->run();

    MAGIC_DEBUG() << "HttpClient Testing..." << _WIN32_WINNT;
    auto function1 = [configFile,pool,logAppender](){};
    auto funciont2 = [function1](){};
    auto funciont3 = [function1,funciont2](){};
    std::function<void()> function4 = [function1,funciont2,funciont3](){};

    MAGIC_DEBUG() << "Size: "  << sizeof(function1)<< " " << sizeof(funciont2) << " " << sizeof(funciont3) << " " << sizeof(function4) << sizeof(std::function<void()>);
//    MAGIC_DEBUG() << "Pointer Num: " << newNum << "\n";

    /// HttpClient
//    {
//        Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
//        httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
//        Safe<HttpClient> client = std::make_shared<HttpClient>("https://www.baidu.com/",1000);
//        client->onTimeOut([](){
//            MAGIC_WARN() << "请求超时";
//        })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
//            MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
//        })->execute(httpRequest);
//    }



//
    std::printf("_______________________________________________________________\n");
    {
        std::vector<Safe<Magic::Thread>> m_Thread;
        for(auto i = 0; i < 40; i++){
            m_Thread.emplace_back(std::make_shared<Magic::Thread>(std::string("HttpClient ") + Magic::AsString(i),[](){
                Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
                httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
                Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
                client->onTimeOut([](){
                    MAGIC_WARN() << "请求超时";
                })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
                    MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
                })->execute(httpRequest);
            }));
        }

        for(auto& v : m_Thread){
            v->join();
        }
    }


//    for(auto i = 0; i < 10; i++){
//        Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
//        httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
//        Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
//        client->onTimeOut([](){
//            MAGIC_WARN() << "请求超时";
//        })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
//            MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
//        })->execute(httpRequest);
//    }

    std::getchar();

    return 0;
}