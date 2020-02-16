#include "Magic.h"
#include "asio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include <vector>
#include "IoPool.h"
#include "Http/HttpParser.h"
#include "TcpServer.h"
#include "Http/Http.h"
#include "Http/HttpServlet.h"
#include "Http/HttpServer.h"
#include "Http/Session.h"
#include "Http/MultiPart.h"
#include "Http/HttpFile.h"
#include "Any.h"
class IPluginModule {
public:
    virtual int arg() = 0;
    virtual ~IPluginModule() {};
};

void Plugin(){
    MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
    Safe<Magic::Plugin> plugin{ 
        new Magic::Plugin("TestLib", MAGIC_CONFIG()->at<std::string>("Library","TestLib.DLL"))
    };
    MAGIC_PLUGINMGR()->addPlugin(plugin);
    Safe<IPluginModule> pp(MAGIC_PLUGIN("TestLib")->getInstance<IPluginModule>());
    MAGIC_LOG(Magic::LogLevel::LogInfo) << pp->arg();
}

class DeafultServlet :public Magic::Http::HttpServlet{
    public:
        DeafultServlet()
            :HttpServlet("DeafultServlet"){
        }
        bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
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
        bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
            response->setStatus(Magic::Http::HttpStatus::OK);
            std::fstream stream;
            response->setHeader("Content-type","text/html");
            stream.open("test.html",std::ios::in);
            stream.seekg(0,std::ios_base::end);
            uint32_t size = stream.tellg();
            Share<char> buffer(new char[size],[](char* ptr){delete[] ptr;});
            stream.seekg(0,std::ios_base::beg);
            stream.read(buffer.get(),size);
            std::string log(buffer.get(),size);
            response->setBody(log);
            return true;
        }
};

class FileServlet :public Magic::Http::HttpServlet{
    public:
        FileServlet()
            :HttpServlet("FileServlet"){
        }
        bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
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
        bool handle (Safe<Magic::Http::HttpRequest>& request,Safe<Magic::Http::HttpResponse>& response) override{
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
    std::string ip = MAGIC_CONFIG()->at<std::string>("ServerIp","0.0.0.0");
    uint16_t port  = MAGIC_CONFIG()->at<uint16_t>("ServerPort",80);
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
        MAGIC_CONFIG()->update();
        server.run();
    }catch(std::system_error ec){
        std::cout << ec.what() << std::endl;
    }
}

#include <mysql/mysql.h>

class MySql{
    friend class MySqlStmt;
public:
    MYSQL* get(){
        return &m_MySql;
    }
    MySql(){
    }
    ~MySql(){
        mysql_close(&m_MySql);
    }
    bool connnetDB(const std::string& dataBase,const std::string& ip,const std::string& user,const std::string&  password,uint16_t port){
        if(!mysql_init(&m_MySql)){
            MAGIC_LOG(Magic::LogLevel::LogError) << "MySql initialization failed";
            return false;
        }
        mysql_options(&m_MySql, MYSQL_SET_CHARSET_NAME, "utf8mb4");
        if(!mysql_real_connect(&m_MySql,ip.c_str(),user.c_str(),password.c_str(),dataBase.c_str(),port,0,0)){
            MAGIC_LOG(Magic::LogLevel::LogError) << "MySql connection failed";
            return false;
        }
        MAGIC_LOG(Magic::LogLevel::LogInfo) << "Database connected successful";
        return true;
    }
    bool createBataBase(const std::string& dataBase){
        std::string queryString = "create database if not exists ";
        queryString += dataBase;
        if(mysql_real_query(&m_MySql,queryString.c_str(),queryString.size()) == 0){
            queryString = "use ";
            queryString += dataBase;
            if(mysql_real_query(&m_MySql,queryString.c_str(),queryString.size()) == 0){
                return true;
            }
        }
        return false;
    }
    bool createTable(const std::string& tableName,const std::string& key,const std::string& tableInfo){
        std::string tableQuery = "CREATE TABLE IF NOT EXISTS `" + tableName + "`(";
        tableQuery += tableInfo;
        tableQuery += ",PRIMARY KEY(`" + key + "`)";
        tableQuery += " )ENGINE=InnoDB DEFAULT CHARSET=utf8;";
        MAGIC_LOG(Magic::LogLevel::LogDebug) << tableQuery;
        if(mysql_real_query(&m_MySql,tableQuery.c_str(),tableQuery.size()) == 0){
            MAGIC_LOG(Magic::LogLevel::LogInfo) << "Table create successful";
            return true;
        }
        return false;
    }

private:
    MYSQL m_MySql;

};
class MySqlStmt{
public:
    MySqlStmt(Safe<MySql>& sql)
        :m_MySql(sql){
    }
    ~MySqlStmt(){
        if(m_Stmt){
            mysql_stmt_close(m_Stmt);
        }
        for(auto& i :m_MySqlModifyBinds){
            free(i.buffer);
        }
    }
    void printError(){
        MAGIC_LOG(Magic::LogLevel::LogDebug) << mysql_stmt_error(m_Stmt);
    }
    bool prepare(const std::string& sql){
        m_Stmt = mysql_stmt_init(m_MySql->get());
        if(!m_Stmt){
            return false;
        }
        if(mysql_stmt_prepare(m_Stmt,sql.c_str(),sql.size()) == 0){
            uint32_t count = mysql_stmt_param_count(m_Stmt);
            m_MySqlModifyBinds.resize(count);
            uint32_t null = 0;
            std::memcpy(m_MySqlModifyBinds.data(),&null,sizeof(MYSQL_BIND) * count);
            return true;
        }
        return false;
    }
    void bindNull(uint32_t index){
        m_MySqlModifyBinds[index].buffer_type = MYSQL_TYPE_NULL;
    }
    #define BIND_COPY(Type,Ptr,Size)                                \
        m_MySqlModifyBinds[index].buffer_type = Type;               \
        uint32_t length = Size;                                     \
        if(m_MySqlModifyBinds[index].buffer == nullptr){            \
            m_MySqlModifyBinds[index].buffer = std::malloc(length); \
        }else if(m_MySqlModifyBinds[index].buffer_length < length){ \
            free(m_MySqlModifyBinds[index].buffer);                 \
            m_MySqlModifyBinds[index].buffer = std::malloc(length); \
        }                                                           \
        std::memcpy(m_MySqlModifyBinds[index].buffer,Ptr,length);   \
        m_MySqlModifyBinds[index].buffer_length = length

    void bind(uint32_t index,const int8_t& value){
        BIND_COPY(MYSQL_TYPE_TINY,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }
    void bind(uint32_t index,const uint8_t& value){
        BIND_COPY(MYSQL_TYPE_TINY,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }
    void bind(uint32_t index,const int16_t& value){
        BIND_COPY(MYSQL_TYPE_SHORT,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }
    void bind(uint32_t index,const uint16_t& value){
        BIND_COPY(MYSQL_TYPE_SHORT,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }
    void bind(uint32_t index,const int32_t& value){
        BIND_COPY(MYSQL_TYPE_LONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }
    void bind(uint32_t index,const uint32_t& value){
        BIND_COPY(MYSQL_TYPE_LONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }
    void bind(uint32_t index,const int64_t& value){
        BIND_COPY(MYSQL_TYPE_LONGLONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }
    void bind(uint32_t index,const uint64_t& value){
        BIND_COPY(MYSQL_TYPE_LONGLONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }
    void bind(uint32_t index,const float& value){
        BIND_COPY(MYSQL_TYPE_FLOAT,&value,sizeof(value));
    }
    void bind(uint32_t index,const double& value){
        BIND_COPY(MYSQL_TYPE_DOUBLE,&value,sizeof(value));
    }
    void bind(uint32_t index,const std::string& value){
        BIND_COPY(MYSQL_TYPE_STRING,value.c_str(),value.size());
    }
    void bindTime(uint32_t index,const time_t& value){
        //bind(index,TimeToString(value));
    }
    void bindBlob(uint32_t index,const std::string& value){
        BIND_COPY(MYSQL_TYPE_BLOB,value.c_str(),value.size());
    }
    void bindBlob(uint32_t index,const void* value,uint64_t size){
        BIND_COPY(MYSQL_TYPE_BLOB,value,size);
    }
    bool execute(){
        mysql_stmt_bind_param(m_Stmt,m_MySqlModifyBinds.data());
        return mysql_stmt_execute(m_Stmt) == 0;
    }

    bool isNull(uint32_t index){
        return m_MySqlResults[index].m_IsNull;
    }
    #define CAST(T) \
        return *reinterpret_cast<T*>(m_MySqlResults[index].m_Data)
    int8_t getInt8(uint32_t index){
        CAST(int8_t);
    }
    uint8_t getUint8(uint32_t index){
        CAST(uint8_t);
    }
    int16_t getInt16(uint32_t index){
        CAST(int16_t);
    }
    int16_t getUint16(uint32_t index){
        CAST(int16_t);
    }
    int32_t getInt32(uint32_t index){
        CAST(int32_t);
    }
    uint32_t getUint32(uint32_t index){
        CAST(uint32_t);
    }
    int64_t getInt64(uint32_t index){
        CAST(int64_t);
    }
    uint64_t getUint64(uint32_t index){
        CAST(uint64_t);
    }
    float getFloat(uint32_t index){
        CAST(float);
    }
    double getDouble(uint32_t index){
        CAST(double);
    }
    #undef CAST
    time_t getTime(uint32_t index){
        //
        return 0;
    }
    std::string getString(uint32_t index){
        return std::string(m_MySqlResults[index].m_Data,m_MySqlResults[index].m_Length);
    }
    std::string getBlob(uint32_t index){
        return std::string(m_MySqlResults[index].m_Data,m_MySqlResults[index].m_Length);
    }
    bool query(){
        mysql_stmt_bind_param(m_Stmt, m_MySqlModifyBinds.data());
        uint32_t err = mysql_stmt_errno(m_Stmt);
        if(err){
            MAGIC_LOG(Magic::LogLevel::LogDebug) << mysql_stmt_error(m_Stmt);
            return false;
        }
        MYSQL_RES* res = mysql_stmt_result_metadata(m_Stmt);
        if(!res){
            MAGIC_LOG(Magic::LogLevel::LogDebug) << "MySql result failed";
            return false;
        }
        uint32_t num = mysql_num_fields(res);
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        m_MySqlResults.resize(num);
        m_MySqlResultBinds.resize(num);
        uint32_t null = 0;
        std::memcpy(m_MySqlResultBinds.data(),&null,sizeof(MYSQL_BIND)*num);
        for(uint32_t i =0;i<num;i++){
            m_MySqlResults[i].m_Type = fields[i].type;
            switch (fields[i].type){
            #define XX(M,T)                                             \
            case M:                                                     \
                m_MySqlResults[i].alloc(sizeof(T));                     \
                break;
            XX(MYSQL_TYPE_TINY, int8_t);
            XX(MYSQL_TYPE_SHORT, int16_t);
            XX(MYSQL_TYPE_LONG, int32_t);
            XX(MYSQL_TYPE_LONGLONG, int64_t);
            XX(MYSQL_TYPE_FLOAT, float);
            XX(MYSQL_TYPE_DOUBLE, double);
            XX(MYSQL_TYPE_TIMESTAMP, MYSQL_TIME);
            XX(MYSQL_TYPE_DATETIME, MYSQL_TIME);
            XX(MYSQL_TYPE_DATE, MYSQL_TIME);
            XX(MYSQL_TYPE_TIME, MYSQL_TIME);
            #undef XX
            default:
                m_MySqlResults[i].alloc(fields[i].length);
                break;
            }
            m_MySqlResultBinds[i].buffer = m_MySqlResults[i].m_Data;
            m_MySqlResultBinds[i].error = &m_MySqlResults[i].m_Error;
            m_MySqlResultBinds[i].length = &m_MySqlResults[i].m_Length;
            m_MySqlResultBinds[i].is_null = &m_MySqlResults[i].m_IsNull;
            m_MySqlResultBinds[i].buffer_type = m_MySqlResults[i].m_Type;
            m_MySqlResultBinds[i].buffer_length = m_MySqlResults[i].m_DataLength;
        }
        if(mysql_stmt_bind_result(m_Stmt,m_MySqlResultBinds.data()) != 0){
            MAGIC_LOG(Magic::LogLevel::LogDebug) << "MySql binding failed";
            return false;
        }
        this->execute();
        if(mysql_stmt_store_result(m_Stmt) !=0){
            MAGIC_LOG(Magic::LogLevel::LogDebug) << "MySql Store Result failed";
            return false;
        }
        return true;
    }
    bool next(){
        return mysql_stmt_fetch(m_Stmt) == 0;
    }
private:
    class SqlResult{
    public:
        SqlResult()
            :m_Data(nullptr)
            ,m_Error(false)
            ,m_IsNull(false)
            ,m_DataLength(0)
            ,m_Length(0)
            ,m_Type(){
        }
        ~SqlResult(){
            if(m_Data){
                delete[] m_Data;
            }
        }
        void alloc(uint32_t size){
            if(m_Data){
                delete[] m_Data;
            }
            m_Data = new char[size]();
            m_Length = size;
            m_DataLength = size;
        }
    public:
        char* m_Data;
        my_bool m_Error;
        my_bool m_IsNull;
        uint32_t m_DataLength;
        unsigned long m_Length;
        enum_field_types m_Type;
    };
private:
    MYSQL_STMT* m_Stmt;
    Safe<MySql>& m_MySql;
    std::vector<SqlResult>  m_MySqlResults;
    std::vector<MYSQL_BIND> m_MySqlModifyBinds;
    std::vector<MYSQL_BIND> m_MySqlResultBinds;
};
int main() {
    Magic::Init("test");
    Safe<MySql> sql(new MySql);
    sql->connnetDB("WEB","127.0.0.1","magic","52100",3306);
    if(sql->createBataBase("WEB")){
        MAGIC_LOG(Magic::LogLevel::LogInfo) << "Database WEB successful";
        std::string table = "`id` INT UNSIGNED AUTO_INCREMENT,`account` VARCHAR(30) NOT NULL";
        sql->createTable("user","id",table);
    }
    MySqlStmt stmt(sql);
    stmt.prepare("insert into user (account) values (?)");
    stmt.bind(0,"Magic");
    if(!stmt.execute()){
        stmt.printError();
    }
    return 0;
}