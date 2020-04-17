/*
 * @file: Test_Server.cpp
 * @Author: INotFound
 * @Date: 2020-03-16 00:11:04
 * @LastEditTime: 2020-03-16 00:14:18
 */
#include <iostream>
#include "Magic.h"

class DeafultServlet :public Magic::Http::HttpServlet{
    public:
        DeafultServlet()
            :HttpServlet("DeafultServlet"){
        }
        bool handle (const Safe<Magic::Http::HttpRequest>& request,const Safe<Magic::Http::HttpResponse>& response) override{
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
        bool handle (const Safe<Magic::Http::HttpRequest>& request,const Safe<Magic::Http::HttpResponse>& response) override{
            response->setStatus(Magic::Http::HttpStatus::OK);
            std::fstream stream;
            response->setHeader("Content-type","text/html");
            stream.open("Test_Server.html",std::ios::in);
            if(stream.is_open()){
                std::stringstream sstream;
                sstream << stream.rdbuf();
                response->setBody(sstream.str());
            }
            return true;
        }
};

class FileServlet :public Magic::Http::HttpServlet{
    public:
        FileServlet()
            :HttpServlet("FileServlet"){
        }
        bool handle (const Safe<Magic::Http::HttpRequest>& request,const Safe<Magic::Http::HttpResponse>& response) override{
            Magic::Http::MultiPart multiPart;
            multiPart.parse(request);
            MAGIC_DEBUG() << multiPart.getParams().count("xxxx");
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
        bool handle (const Safe<Magic::Http::HttpRequest>& request,const Safe<Magic::Http::HttpResponse>& response) override{
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
    std::string ip = Magic::Config::GetInstance()->at<std::string>("ServerIp","0.0.0.0");
    uint16_t port  = Magic::Config::GetInstance()->at<uint16_t>("ServerPort",8080);
    uint16_t processorsNumber = Magic::Config::GetInstance()->at<uint16_t>("ServerProcessorsNumber",Magic::GetProcessorsNumber());
    try{
        Magic::Http::HttpServer server(ip,port,processorsNumber);
        Safe<Magic::Http::HttpServlet> log(new LogServlet);
        Safe<Magic::Http::HttpServlet> deafult(new DeafultServlet);
        Safe<Magic::Http::HttpServlet> file(new FileServlet);
        Safe<Magic::Http::HttpServlet> main(new MainServlet);
        server.getHttpServletDispatch()->setDeafultServlet(deafult);
        server.getHttpServletDispatch()->addHttpServlet("/log",log);
        server.getHttpServletDispatch()->addHttpServlet("/file",file);
        server.getHttpServletDispatch()->addGlobHttpServlet("^/?(.*)$",main);
        Magic::Config::GetInstance()->update();
        server.run();
    }catch(std::system_error ec){
        std::cout << ec.what() << std::endl;
    }
}

int main(){
    Magic::Init("Test_Server");
    Server();
    return 0;
}