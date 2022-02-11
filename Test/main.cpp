/*
 * @Author: INotFound
 * @Date: 2020-12-17 00:35:45
 * @LastEditTime: 2021-01-18 06:00:25
 */
#define PERFORMANCE 1
#define _WIN32_WINNT 0x0601
#include "Magic/Core/Container.h"
#include "Magic/NetWork/Http/Uri.h"
#include "Magic/NetWork/Http/Http.h"

#include "Magic/NetWork/Http/HttpServer.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/TimingWheel.h"
#include "Magic/DataBase/MySql.h"
#include "Magic/DataBase/ConnectionPool.h"
#include "Magic/Core/Adapter.h"
#include "Magic/Utilty/Trace.h"
///// http://127.0.0.1/


class ResourceServlet :public Magic::NetWork::Http::IHttpServlet{
    public:
        ResourceServlet():Magic::NetWork::Http::IHttpServlet("^/?(.*)$",Magic::NetWork::Http::HttpServletType::Global){

        }
        bool handle(const Safe<Magic::NetWork::Http::HttpSocket>& httpSocket,const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response) override{
            std::string res = "./www";
            std::string path = request->getPath();
            if(path == "/"){
                path = "/index.html";
            }
            res += path;
            if(IS_FILE(res.c_str()) != 0){
                return false;
            }

            std::async(std::launch::async,[httpSocket,res,response](){
                response->setResource(res);
                httpSocket->sendResponse(response);
            });

            return true;
        }
};

class AA{
public:
    void printf(){
        std::printf("hello world");
    }
};

void func3(){
    MAGIC_TRACE_PERFORMANCE(__FUNCTION__);
    for (int i = 0; i < 100000; ++i) {
        std::printf("XXXXXXXXXXXXXX %d",i);
    }
}

void func2(){
    MAGIC_TRACE_PERFORMANCE(__FUNCTION__);
    func3();
}

void func1(){
    MAGIC_TRACE_PERFORMANCE(__FUNCTION__);
    func2();
}



int main(int argc,char** argv){
//    Magic::g_TraceAppender = std::make_shared<>
#ifdef PERFORMANCE
    Magic::g_TraceAppender = std::make_shared<Magic::ChromiumTraceAppender>("Profile.json");
#endif
    func1();
    Magic::g_TraceAppender->complete();

    Magic::NetWork::Http::Uri uri;
    uri.execute("mysql://admin@0.0.0.0/xxx?password=12345678901a");
    std::printf("%s\n",uri.getUser().c_str());
    std::printf("%s\n",uri.getHost().c_str());
    std::printf("%s\n",&(uri.getPath().c_str()[1]));
    std::printf("%s\n",uri.getQuery().c_str());
    std::printf("%s\n",uri.getScheme().c_str());
    std::printf("%s\n",uri.getFragment().c_str());

    Magic::Thread::SetName("Master");
    Magic::Configure([](const Safe<Magic::Container>& ioc){
        ioc->registerType<Magic::Config>();
        ioc->registerType<Magic::Logger,Magic::Config>();
    });

    Safe<Magic::Config> config = std::make_shared<Magic::Config>();
    Safe<Magic::ConfigFile> configFile = std::make_shared<Magic::ConfigFile>();
    Safe<Magic::IConfigFormatter>  formatter = std::make_shared<Magic::InIConfigFormatter>();
    configFile->addFormatter(formatter);
    config->addConfigFile(configFile);

    Magic::g_Logger = std::make_shared<Magic::Logger>(config);
    Safe<Magic::ILogAppender> logAppender = std::make_shared<Magic::StdOutLogAppender>();
    Magic::g_Logger->addILogAppender(logAppender);
    Safe<Magic::TimingWheel> timingWheel = std::make_shared<Magic::TimingWheel>(config);
    Safe<Magic::NetWork::IoPool> pool = std::make_shared<Magic::NetWork::IoPool>(config);
    timingWheel->run();
    Magic::NetWork::Http::HttpServer server(pool,timingWheel,config);

    Safe<Magic::DataBase::ConnectionPool<Magic::DataBase::MySql>> connectionPool = std::make_shared<Magic::DataBase::ConnectionPool<Magic::DataBase::MySql>>(config,timingWheel);



    connectionPool->initialize([](){
        Safe<Magic::DataBase::MySql>  sql = std::make_shared<Magic::DataBase::MySql>();
        if(sql->connnetDB("Test","1.15.39.123","root","1234567890a",3306)){
            return sql;
        }
        return Safe<Magic::DataBase::MySql>();
    });
    auto conn = connectionPool->getConnection();
    if(conn){
        Magic::DataBase::MySqlStmt stmt(*conn);
        stmt.prepare("INSERT INTO device(sn,mac,room_id,store_id,room_name,created_time,updated_time,last_version,update_version)VALUES(?,?,?,?,?,?,?,?,?)");
        stmt.bind(0,"a01");
        stmt.bind(1,"FF:FF:FF:FF");
        stmt.bind(2,"A9999");
        stmt.bind(3,"9999");
        stmt.bind(4,"B9999");
        stmt.bind(5,time(0));
        stmt.bind(6,time(0));
        stmt.bind(7,"version 1");
        stmt.bind(8,"version 2");
        stmt.execute();
        stmt.printError();

//        stmt.prepare("select * from device where store_id = ?");
//        stmt.bind(0,99999);
//        bool ret = stmt.query();
//        if(ret){
//            MAGIC_DEBUG() << "1";
//        }else{
//            MAGIC_DEBUG() << "2";
//        }
//        if(stmt.fetch()){
//            MAGIC_DEBUG() << "id : " << stmt.getUint32(0);
//        }
    }


    Safe<Magic::NetWork::Http::IHttpServlet> servlet = std::make_shared<Magic::NetWork::Http::NotFoundServlet>();
    Safe<Magic::NetWork::Http::IHttpServlet> resservlet = std::make_shared<ResourceServlet>();
    Safe<Magic::NetWork::Http::HttpServletDispatch> dispatch = std::make_shared<Magic::NetWork::Http::HttpServletDispatch>();

    MAGIC_DEBUG() << __cplusplus;

    dispatch->addHttpServlet(servlet);
    dispatch->addHttpServlet(resservlet);
    server.setServletDispatch(dispatch);
    server.run();
    return 0;
}

//int main(){
//    printf("以下是测试文字：\n");
//    printf("\033[0m默认文字\033[0m\n");
//    printf("\033[1m高亮文字\033[0m\n");
//    printf("\033[2m低亮文字\033[0m\n");
//    //printf("\033[3m无效文字\033[0m\n");
//    printf("\033[4m下划线文字\033[0m\n");
//    printf("\033[5m闪烁文字(无效)\033[0m\n");
//    //printf("\033[6m无效文字\033[0m\n");
//    printf("\033[7m反显文字\033[0m\n");
//    printf("\033[8m消隐文字(无效)\033[0m\n");
//
//    printf("\n\033[31;1m字体颜色\033[0m测试文字\n");
//    printf("\033[30m低亮黑色文字\033[0m\t\033[30;1m高亮黑色文字\033[0m\n");
//    printf("\033[31m低亮红色文字\033[0m\t\033[31;1m高亮红色文字\033[0m\n");
//    printf("\033[32m低亮绿色文字\033[0m\t\033[32;1m高亮绿色文字\033[0m\n");
//    printf("\033[33m低亮黄色文字\033[0m\t\033[33;1m高亮黄色文字\033[0m\n");
//    printf("\033[34m低亮蓝色文字\033[0m\t\033[34;1m高亮蓝色文字\033[0m\n");
//    printf("\033[35m低亮紫色文字\033[0m\t\033[35;1m高亮紫色文字\033[0m\n");
//    printf("\033[36m低亮浅蓝文字\033[0m\t\033[36;1m高亮浅蓝文字\033[0m\n");
//    printf("\033[37m低亮白色文字\033[0m\t\033[37;1m高亮白色文字\033[0m\n");
//    printf("\033[38m测试文字\033[0m\n");
//    printf("\033[39m测试文字\033[0m\n");
//
//    printf("\n\033[31;1m背景颜色\033[0m测试文字\n");
//    printf("\033[40m低亮文字黑色背景\033[0m\t\033[40;1m高亮文字黑色背景\033[0m\n");
//    printf("\033[41m低亮文字红色背景\033[0m\t\033[41;1m高亮文字红色背景\033[0m\n");
//    printf("\033[42m低亮文字绿色背景\033[0m\t\033[42;1m高亮文字绿色背景\033[0m\n");
//    printf("\033[43m低亮文字黄色背景\033[0m\t\033[43;1m高亮文字黄色背景\033[0m\n");
//    printf("\033[44m低亮文字蓝色背景\033[0m\t\033[44;1m高亮文字蓝色背景\033[0m\n");
//    printf("\033[45m低亮文字紫色背景\033[0m\t\033[45;1m高亮文字紫色背景\033[0m\n");
//    printf("\033[46m低亮文字浅蓝背景\033[0m\t\033[46;1m高亮文字浅蓝背景\033[0m\n");
//    printf("\033[47m低亮文字白色背景\033[0m\t\033[47;1m高亮文字白色背景\033[0m\n");
//    printf("\033[48m测试文字\033[0m\n");
//    printf("\033[49m测试文字\033[0m\n");
//}
