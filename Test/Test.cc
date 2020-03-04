#include "Magic.h"
#include <iostream>
#include "WebSocket/WebSocketServer.h"
class IPluginModule {
public:
    virtual int arg() = 0;
    virtual ~IPluginModule() {};
};

void Plugin(){
    auto config = Magic::Config::GetInstance();
    Safe<Magic::Plugin> plugin{ 
        new Magic::Plugin("TestLib", config->at<std::string>("Library","TestLib.DLL"))
    };
    auto& pluginMgr = Magic::PluginMgr::GetInstance();
    pluginMgr->addPlugin(plugin);
    Safe<IPluginModule> pp(pluginMgr->at("TestLib")->getInstance<IPluginModule>());
    MAGIC_LOG(Magic::LogLevel::LogInfo) << pp->arg();
}

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
            stream.open("test.html",std::ios::in);
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
            std::cout << multiPart.getParams().count("xxxx") << std::endl;
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
    try{
        Magic::Http::HttpServer server(ip,port,Magic::GetProcessorsNumber()*2);
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
void Sql(){
    Safe<Magic::DB::MySql> sql(new Magic::DB::MySql);
    sql->connnetDB("WEB","127.0.0.1","magic","52100",3306);

    std::string userTable("CREATE TABLE IF NOT EXISTS `User`("
                        "`Id` INT UNSIGNED AUTO_INCREMENT COMMENT '用户id',"
                        "`Name` VARCHAR(30) NOT NULL DEFAULT '' COMMENT '名称',"
                        "`Emall` VARCHAR(50) NOT NULL DEFAULT '' COMMENT '邮箱',"
                        "`Account` VARCHAR(30) NOT NULL DEFAULT '' COMMENT '账号',"
                        "`Password` VARCHAR(50) NOT NULL DEFAULT '' COMMENT '密码',"
                        "`LoginTime` TIMESTAMP NOT NULL DEFAULT '1980-01-01 00:00:00',"
                        "`CreateTime` TIMESTAMP NOT NULL DEFAULT '1980-01-01 00:00:00',"
                        "`UpdateTime` TIMESTAMP NOT NULL DEFAULT '1980-01-01 00:00:00',"
                        "PRIMARY KEY (`Id`),"
                        "UNIQUE KEY `User_Name` (`Name`),"
                        "UNIQUE KEY `User_Emall` (`Emall`),"
                        "UNIQUE KEY `User_Account` (`Account`)"
                        ")ENGINE=InnoDB DEFAULT CHARSET=utf8;");
    if(sql->execute(userTable)){
        MAGIC_LOG(Magic::LogLevel::LogDebug) << "Create sql table successful";
    }
    // std::string insertsql("");
    
    // if(sql->execute(insertsql)){
    //     MAGIC_LOG(Magic::LogLevel::LogDebug) << "Create sql insert successful";
    // }
    // Magic::DB::MySqlStmt stmt(sql);
    // stmt.prepare("insert into user (account) values (?)");
    // stmt.bind(0,"INotFound");
    // if(!stmt.execute()){
    //     stmt.printError();
    // }
    // stmt.prepare("select account from user;");
    // if(!stmt.query()){
    //     stmt.printError();
    // }
    // std::cout << stmt.getString(0) << std::endl;
}
class A{
public:
    void printf(){
        std::cout << "hhhhh" << std::endl;
    }
};
int main() {
    Magic::Init("test");
    //Sql();
    Server();
    //Magic::WebSocket::WebSocketServer server("0.0.0.0",8181,1,60*1000);
    //server.run();
    getchar();
    return 0;
}


