/*
 * @Author: INotFound
 * @Date: 2020-12-17 00:35:45
 * @LastEditTime: 2021-01-18 06:00:25
 */
#define PERFORMANCE 1
#include <regex>
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


///// http://127.0.0.1/

class A{
public:
    ObjectWrapper<A> bb() {return ObjectWrapper<A>(this);};
};

using namespace Magic::NetWork::Http;
Safe<WebSocket> webSocket;
class ResourceServlet :public Magic::NetWork::Http::IHttpServlet{
    public:
        ResourceServlet(){
        }

        void websocket(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket
                       ,const Safe<Magic::NetWork::Http::HttpRequest>& request
                       ,const Safe<Magic::NetWork::Http::HttpResponse>& response){
            webSocket = httpSocket->upgradeWebSocket(request,response);
            webSocket->sendTextMessage("xxxxxx");
            webSocket->recvTextMessage([](const Safe<WebSocket>& socket,const std::string& msg){
                MAGIC_DEBUG() << msg;
                socket->sendTextMessage(msg);
            });
            webSocket->disconnectedCallBack([](const Safe<WebSocket>& socket){
                MAGIC_DEBUG() << "disconnected";
            });
        }

        void handle1(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket,const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response){
            response->setBody("Hello World")->setStatus(HttpStatus::OK);
            httpSocket->sendResponse(response);
        }
};

//std::atomic_int newNum(0);
//
//void* operator new(std::size_t size)
//{
//    newNum++;
//    std::cout << "New " << newNum << std::endl;
//    return std::malloc(size);
//}
//
//void operator delete(void* ptr)
//{
//    newNum--;
//    std::cout << "delete " << newNum << std::endl;
//    std::free(ptr);
//}


//int main(int /*argc*/,char** /*argv*/){
//    std::string cpp = "B(Safe<A> a,Safe<B> b,Safe<C> c)";
//    std::regex reg("B\\((.*?)\\)");
//    std::smatch match;
//    std::regex_search(cpp,match,reg);
//
//    std::smatch subMatch;
//    std::regex subReg("Safe<(.+?)+>");
//    std::string subCpp = match[1];
//    std::regex_search(subCpp,subMatch,subReg);
//    for(auto v : subMatch){
//        std::cout << subMatch.size() << " : " << v << std::endl;
//    }
//
////    std::regex_match(cpp,reg);
//    return 0;
//}


class AA{
public:
    AA(){
    }
};
class BB{
public:
    BB(const Safe<AA>&){
    }
    void init(const Safe<AA>& a){
        std::cout << "xxxxXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx: " << (a ? 1 : 0) << std::endl;
    }
};


int main(int /*argc*/,char** /*argv*/){
//    Magic::g_TraceAppender = std::make_shared<>
//    std::printf("ptr[1] size of %llu \n",sizeof(Safe<A>));
//    ObjectWrapper<HttpRequest>
//    std::printf("ptr[2] size of %llu \n",sizeof(ObjectWrapper<A>));
//    std::printf("ptr[3] size of %llu \n",sizeof(ObjectWrapper<Magic::NetWork::Http::HttpRequest>));

    system("chcp 65001");

//    std::printf("size of %llu\n",sizeof(void*));

    Magic::NetWork::Http::Uri uri("mysql://admin@0.0.0.0:8181/xxx?password=12345678901a&x=2");
    std::printf("%s\n",uri.getUser().c_str());
    std::printf("%s\n",uri.getHost().c_str());
    std::printf("%s\n",uri.getPath().c_str());
    std::printf("%s\n",uri.getQuery().c_str());
    std::printf("%s\n",uri.getScheme().c_str());
    std::printf("%s\n",uri.getFragment().c_str());
    std::printf("%d\n",uri.getPort());

    Magic::Thread::SetName("Master");
    auto ioc = Magic::Configure([](const Safe<Magic::Container>& ioc){
//        ioc->registerType<Magic::Logger,Safe<Magic::Config>>();
        ioc->registerType<AA>();
        ioc->registerType<BB,Safe<AA>>().registerProperty(&BB::init);
    });

    ioc->resolveAll();

    Safe<Magic::IConfigFormatter>  formatter = std::make_shared<Magic::InIConfigFormatter>();
    Safe<Magic::ConfigFile> configFile = std::make_shared<Magic::ConfigFile>(formatter);
    Safe<Magic::Config> config = std::make_shared<Magic::Config>(configFile);


    Magic::g_Logger = std::make_shared<Magic::Logger>(config);
    Safe<Magic::ILogAppender> logAppender = std::make_shared<Magic::StdOutLogAppender>();
    Magic::g_Logger->addILogAppender(logAppender);
    Magic::g_TimingWheel = std::make_shared<Magic::TimingWheel>(config);
    Safe<Magic::NetWork::IoPool> pool = std::make_shared<Magic::NetWork::IoPool>(config);
    Magic::g_TimingWheel->run();
    Magic::NetWork::Http::HttpServer server(pool,config);

    Safe<Magic::NetWork::Http::IHttpServlet> resservlet = std::make_shared<ResourceServlet>();
    Safe<Magic::NetWork::Http::HttpServletDispatch> dispatch = std::make_shared<Magic::NetWork::Http::HttpServletDispatch>();

    dispatch->setHttpServlet(resservlet);
//    dispatch->addHttpServlet(resservlet);

    resservlet->addRoute("/",&ResourceServlet::handle1);
    resservlet->addRoute("/chat",&ResourceServlet::websocket);

    server.setServletDispatch(dispatch);



//    Magic::NetWork::TcpClient client("www.baidu.com",80);
//    client.connect([&client](){
//        Safe<Magic::NetWork::Http::HttpRequest> request = std::make_shared<Magic::NetWork::Http::HttpRequest>();
//        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
//        std::ostream stream(streamBuffer.get());
//        stream << request;
//        client.send(streamBuffer);
//        client.recv([](Magic::NetWork::Socket::StreamBuffer& buffer){
//            MAGIC_DEBUG() << "xxx " << buffer.data();
//        });
//    });
//
//    client.run();
//    server.run();
//
    Magic::NetWork::Http::Uri uris("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=6f260538-b8d0-4c29-9492-6b04522ba3d3");
    std::printf("%s\n",uris.getUser().c_str());
    std::printf("%s\n",uris.getHost().c_str());
    std::printf("%s\n",uris.getPath().c_str());
    std::printf("%s\n",uris.getQuery().c_str());
    std::printf("%s\n",uris.getScheme().c_str());
    std::printf("%s\n",uris.getFragment().c_str());
    std::printf("%d\n",uris.getPort());
    MAGIC_DEBUG() << " Port " << uris.getPort();


//    ->setHeader("Content-Type","application/json")
//    ->setBody(R"Template({
//    "msgtype": "markdown",
//    "markdown": {
//        "content": "中间件开发测试消息<font color=\"warning\">111</font>，请相关同事注意。\n
//         >类型:<font color=\"comment\">测试</font>
//         >反馈:<font color=\"comment\">测试</font>",
//        "mentioned_list":["@all"]
//    }
//})Template");

    Magic::Thread thread("",[](){
        MAGIC_DEBUG() << "xxxx";
        Safe<Magic::NetWork::Http::HttpRequest> httpRequest = std::make_shared<Magic::NetWork::Http::HttpRequest>();
        httpRequest->setMethod(Magic::NetWork::Http::HttpMethod::GET);
        Safe<HttpClient> client = std::make_shared<HttpClient>("http://www.baidu.com/",1000);
        client->onTimeOut([](){
            MAGIC_DEBUG() << "请求超时";
        })->onResponse([](const Safe<Magic::NetWork::Http::HttpResponse>& response){
            MAGIC_DEBUG() << static_cast<uint32_t>(response->getStatus());
        })->execute(httpRequest);
    });
    thread.join();

    MAGIC_DEBUG() << "@@@@@@@@@@@@@@";
    std::getchar();
    return 0;
}
