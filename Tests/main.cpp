/*
 * @Author: INotFound
 * @Date: 2020-12-17 00:35:45
 * @LastEditTime: 2021-01-18 06:00:25
 */
#define PERFORMANCE 1
#include <regex>
#include <type_traits>
#include "Magic/Magic"
#include "Magic/Utilty/String.h"
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


struct Hello
{
    void before(int) { return; }
};

struct Generic {
    void xxx(int) { return; }
};

// SFINAE test
template<typename T,typename Sign>
class HasBefore{
    typedef char no[2];
    typedef char yes[1];
    template <typename U,U>
    struct TypeCheck;
    template <typename C>
    static no& Check(...);
    template <typename C>
    static yes& Check(TypeCheck<Sign, &C::before>*);
public:
    enum { value = sizeof(Check<T>(nullptr)) == sizeof(char) };
};

template <typename Ret, typename... Args>
std::tuple<Ret, Args...> FunctionChecker(std::function<Ret(Args...)>);

void syszuxPrint(){std::cout<<std::endl;}

template<typename T, typename... Ts>
void syszuxPrint(T arg1, Ts... arg_left){
    std::cout<<arg1<<", ";
    syszuxPrint(arg_left...);
}

class Aop{
public:
    bool before(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){return false;}
    bool after(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket){return false;}
};


int main(int /*argc*/,char** /*argv*/){
    std::cout << HasBefore<Hello,void(Hello::*)(int)>::value << std::endl;
    std::cout << HasBefore<Generic,std::string(Generic::*)()>::value << std::endl;

    Hello hello;
    Generic generic;
    std::function<void(int)> xxx = std::bind(&Hello::before,&hello,std::placeholders::_1);
    std::function<void(int)> xxs = std::bind(&Generic::xxx,&generic,std::placeholders::_1);
    std::cout << std::is_same<decltype(FunctionChecker(xxx)),decltype(FunctionChecker(xxs))>::value << std::endl;

//    Safe<Aop> aop = std::make_shared<Aop>();
    using RouteHandle = std::function<void(const Safe<Magic::NetWork::Http::HttpSocket>&)>;
    std::cout << std::is_same<decltype(Magic::NetWork::Http::FunctionChecker(typename std::remove_reference<RouteHandle&>::type())),decltype(Magic::NetWork::Http::FunctionChecker(RouteHandle()))>::value << std::endl;
    return 0;
}
//int main(int /*argc*/,char** /*argv*/){
//    Safe<Magic::Application> application = std::make_shared<Magic::Application>();
//    application->initialize();
//    Magic::Thread thread("Test",[](){
//        Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
//        httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
//        Safe<Magic::NetWork::Http::HttpClient> client = std::make_shared<Magic::NetWork::Http::HttpClient>("http://www.baidu.com/");
//        client->onTimeOut([](){
//            MAGIC_DEBUG() << "请求超时";
//        })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
//            MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
//        })->execute(httpRequest);
//    });
//    thread.join();
//    std::string string("12:32:ab:23");
//    MAGIC_DEBUG() << "xxx: " << Magic::Replace(Magic::ToUpper(string),":","");
//
//    std::getchar();
//    thread.join();
//    MAGIC_DEBUG() << "@@@@@@@@@@@@@@";
//    std::getchar();
//    return 0;
//}
