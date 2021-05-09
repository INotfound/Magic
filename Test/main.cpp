/*
 * @Author: INotFound
 * @Date: 2020-12-17 00:35:45
 * @LastEditTime: 2021-01-18 06:00:25
 */
#define _WIN32_WINNT 0x0601
#include "Magic/NetWork/Http/Uri.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpParser.h"

#include "Magic/NetWork/Http/HttpServer.h"
#include "Magic/NetWork/IoPool.h"
#include "Magic/Utilty/Config.h"
#include "Magic/NetWork/Http/HttpServlet.h"
#include "Magic/Utilty/Logger.h"



class DeafultServlet :public Magic::NetWork::Http::IHttpServlet{
public:
    DeafultServlet()
            :IHttpServlet("",Magic::NetWork::Http::HttpServletType::Deafult){
    }
    bool handle (const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response) override{
        response->setStatus(Magic::NetWork::Http::HttpStatus::NOT_FOUND);
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

class ResourceServlet :public Magic::NetWork::Http::IHttpServlet{
    public:
        ResourceServlet():Magic::NetWork::Http::IHttpServlet("^/?(.*)$",Magic::NetWork::Http::HttpServletType::Global){

        }
        bool handle(const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response) override{
            std::string res = "./www";
            std::string path = request->getPath();
            if(path == "/"){
                path = "/index.html";
            }
            res += path;
            response->setResource(res);
            if(IS_FILE(res.c_str()) != 0){
                return false;
            }
            return true;
        }
};


int main(int argc,char** argv){
    Magic::NetWork::Http::Uri uri;
    uri.execute("mysql://admin@0.0.0.0/xxx?password=12345678901a");
    std::printf("%s\n",uri.getUser().c_str());
    std::printf("%s\n",uri.getHost().c_str());
    std::printf("%s\n",&(uri.getPath().c_str()[1]));
    std::printf("%s\n",uri.getQuery().c_str());
    std::printf("%s\n",uri.getScheme().c_str());
    std::printf("%s\n",uri.getFragment().c_str());

    Magic::NetWork::Http::HttpRequestParser parser;
    std::string httpRequestContext("GET /bg-upper.png HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: 127.0.0.1:8180\r\nAccept: */*\r\nRange: bytes=0-10\r\n\r\n");
    char context[] = "GET /bg-upper.png HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: 127.0.0.1:8180\r\nAccept: */*\r\nRange: bytes=0-\r\n\r\n";
    parser.execute(context,httpRequestContext.size());
    auto request = parser.getData();
    std::cout << request->isRange() << std::endl;
    std::cout << request->getRangeStop() << std::endl;
    std::cout << request->getRangeStart() << std::endl;
    
    Magic::NetWork::Http::HttpResponseParser responseParser;
    char responseContext[] = "HTTP/1.1 206 Partial Content\r\nAccept-Ranges: bytes\r\nContent-Range: bytes 20-2000/4932\r\nContent-Length: 2001\r\n\r\n";
    std::string httpResponseContext("HTTP/1.1 206 Partial Content\r\nAccept-Ranges: bytes\r\nContent-Range: bytes 20-2000/4932\r\nContent-Length: 2001\r\n\r\n");
    responseParser.execute(responseContext,httpResponseContext.length());
    auto response = responseParser.getData();
    
    std::cout << "-------------" << std::endl;
    std::cout << response->isRange() << std::endl;
    std::cout << "Stop: " << response->getRangeStop() << std::endl;
    std::cout << "Start: " << response->getRangeStart() << std::endl;

    Safe<Magic::Config> config = std::make_shared<Magic::Config>();
    Safe<Magic::ConfigFile> configFile = std::make_shared<Magic::ConfigFile>();
    configFile->setFilePath("Magic.conf");
    Safe<Magic::IConfigFormatter>  formatter = std::make_shared<Magic::InIConfigFormatter>();
    configFile->addFormatter(formatter);
    config->addConfigFile(configFile);

    Magic::g_Logger = std::make_shared<Magic::Logger>(config);
    Safe<Magic::ILogAppender> logAppender = std::make_shared<Magic::StdOutLogAppender>();
    Magic::g_Logger->addILogAppender(logAppender);

    Safe<Magic::NetWork::IoPool> pool = std::make_shared<Magic::NetWork::IoPool>(config);

    Magic::NetWork::Http::HttpServer server(pool,config);

    Safe<Magic::NetWork::Http::IHttpServlet> servlet = std::make_shared<DeafultServlet>();
    Safe<Magic::NetWork::Http::IHttpServlet> resservlet = std::make_shared<ResourceServlet>();
    Safe<Magic::NetWork::Http::HttpServletDispatch> dispatch = std::make_shared<Magic::NetWork::Http::HttpServletDispatch>();

    dispatch->addHttpServlet(servlet);
    dispatch->addHttpServlet(resservlet);
    server.setServletDispatch(dispatch);
    return 0;
}
