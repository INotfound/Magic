#include <cstring>
#include "Magic/Utilty/Logger.h"
#include "Magic/DataBase/MySql.h"
#ifdef MYSQL
namespace Magic{
namespace DataBase{
    class MySqlLibraryWrapper{
    public:
        ~MySqlLibraryWrapper(){
            mysql_library_end();
        }
        MySqlLibraryWrapper(){
            mysql_library_init(0, NULL, NULL);
        }
    };

    static MySqlLibraryWrapper g_MySqlLibraryWrapper;

    void MySqlTimeToTime(const MYSQL_TIME& mt,std::time_t ts){
        struct tm time;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&time, &ts);
    #else
        localtime_r(&ts, &time);
    #endif
        time.tm_mday = mt.day;
        time.tm_hour = mt.hour;
        time.tm_min = mt.minute;
        time.tm_sec = mt.second;
        time.tm_mon = mt.month - 1;
        time.tm_year = mt.year - 1900;
        ts = mktime(&time);
        if(ts < 0){
            ts = 0;
        }
    }

    void TimeToMySqlTime(const std::time_t ts,MYSQL_TIME& mt){
        struct tm time;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&time, &ts);
    #else
        localtime_r(&ts, &time);
    #endif
        mt.day = time.tm_mday;
        mt.hour = time.tm_hour;
        mt.second = time.tm_sec;
        mt.minute = time.tm_min;
        mt.month = time.tm_mon + 1;
        mt.year = time.tm_year + 1900;
    }

    MySql::~MySql(){
        mysql_close(&m_MySql);
    }

    MySql::MySql(){
    }

    void MySql::printError(){
        MAGIC_ERROR() << mysql_error(&m_MySql);
    }

    bool MySql::execute(const std::string& sql){
        Mutex::Lock lock(m_Mutex);
        if(mysql_ping(&m_MySql) != 0){
            this->printError();
        }
        if(mysql_real_query(&m_MySql,sql.c_str(),sql.size()) == 0){
            return true;
        }
        return false;
    }

    bool MySql::connnetDB(const std::string& dataBase,const std::string& ip,const std::string& user,const std::string&  password,uint16_t port){
        Mutex::Lock lock(m_Mutex);
        if(!mysql_init(&m_MySql)){
            this->printError();
            return false;
        }
        char value = 1;
        mysql_options(&m_MySql, MYSQL_OPT_RECONNECT, &value);
        mysql_options(&m_MySql, MYSQL_SET_CHARSET_NAME, "utf8mb4");
        if(!mysql_real_connect(&m_MySql,ip.c_str(),user.c_str(),password.c_str(),dataBase.c_str(),port,0,0)){
            this->printError();
            return false;
        }
        MAGIC_INFO() << "Database Connected Successful";
        return true;
    }

    MySqlStmt::~MySqlStmt(){
        if(m_Stmt){
            mysql_stmt_free_result(m_Stmt);
            mysql_stmt_close(m_Stmt);
            for(auto& i :m_MySqlModifyBinds){
                if(i.buffer){
                    free(i.buffer);
                }
            }
        }
    }

    MySqlStmt::MySqlStmt(const Safe<MySql>& sql)
        :m_Stmt(nullptr)
        ,m_MySql(sql){
    }

    bool MySqlStmt::fetch(){
        RWMutex::WriteLock lock(m_Mutex);
        return mysql_stmt_fetch(m_Stmt) == 0;
    }

    bool MySqlStmt::query(){
        RWMutex::WriteLock lock(m_Mutex);
        if(mysql_ping(&m_MySql->m_MySql) != 0){
            this->printError();
        }
        mysql_stmt_bind_param(m_Stmt, m_MySqlModifyBinds.data());
        uint32_t err = mysql_stmt_errno(m_Stmt);
        if(err){
            MAGIC_WARN() << mysql_stmt_error(m_Stmt);
            return false;
        }
        MYSQL_RES* res = mysql_stmt_result_metadata(m_Stmt);
        if(!res){
            MAGIC_WARN() << "MySql Result Failed";
            return false;
        }
        uint32_t num = mysql_num_fields(res);
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        m_MySqlResults.resize(num);
        m_MySqlResultBinds.resize(num);
        std::memset(m_MySqlResultBinds.data(),0,sizeof(MYSQL_BIND)*num);
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
        mysql_free_result(res);
        if(mysql_stmt_bind_result(m_Stmt,m_MySqlResultBinds.data()) != 0){
            MAGIC_WARN() << "MySql Binding Failed";
            return false;
        }
        if(!this->execute()){
            return false;
        }
        if(mysql_stmt_store_result(m_Stmt) !=0){
            MAGIC_WARN() << "MySql Store Result Failed";
            return false;
        }
        return true;
    }

    bool MySqlStmt::execute(){
        if(mysql_ping(&m_MySql->m_MySql) != 0){
            this->printError();
        }
        mysql_stmt_bind_param(m_Stmt,m_MySqlModifyBinds.data());
        return mysql_stmt_execute(m_Stmt) == 0;
    }

    uint64_t MySqlStmt::rows(){
        return mysql_stmt_affected_rows(m_Stmt);
    }

    void MySqlStmt::printError(){
        MAGIC_WARN() << mysql_stmt_error(m_Stmt);
    }

    bool MySqlStmt::prepare(const std::string& sql){
        RWMutex::ReadLock lock(m_Mutex);
        if(mysql_ping(&this->m_MySql->m_MySql) != 0){
            this->printError();
        }
        m_Stmt = mysql_stmt_init(&this->m_MySql->m_MySql);
        if(!m_Stmt){
            this->printError();
            return false;
        }
        if(mysql_stmt_prepare(m_Stmt,sql.c_str(),sql.size()) == 0){
            uint32_t count = mysql_stmt_param_count(m_Stmt);
            m_MySqlModifyBinds.resize(count);
            std::memset(m_MySqlModifyBinds.data(),0,sizeof(MYSQL_BIND) * count);
            return true;
        }
        this->printError();
        return false;
    }

    void MySqlStmt::bindNull(uint32_t index){
        RWMutex::WriteLock lock(m_Mutex);
        m_MySqlModifyBinds[index].buffer_type = MYSQL_TYPE_NULL;
    }

    #define BIND_COPY(Type,Ptr,Size)                                \
        RWMutex::WriteLock lock(m_Mutex);                           \
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
    
    void MySqlStmt::bind(uint32_t index,const float value){
        BIND_COPY(MYSQL_TYPE_FLOAT,&value,sizeof(value));
    }
    
    void MySqlStmt::bind(uint32_t index,const double value){
        BIND_COPY(MYSQL_TYPE_DOUBLE,&value,sizeof(value));
    }

    void MySqlStmt::bind(uint32_t index,const int8_t value){
        BIND_COPY(MYSQL_TYPE_TINY,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }

    void MySqlStmt::bind(uint32_t index,const uint8_t value){
        BIND_COPY(MYSQL_TYPE_TINY,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }

    void MySqlStmt::bind(uint32_t index,const int16_t value){
        BIND_COPY(MYSQL_TYPE_SHORT,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }

    void MySqlStmt::bind(uint32_t index,const uint16_t value){
        BIND_COPY(MYSQL_TYPE_SHORT,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }

    void MySqlStmt::bind(uint32_t index,const int32_t value){
        BIND_COPY(MYSQL_TYPE_LONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }

    void MySqlStmt::bind(uint32_t index,const uint32_t value){
        BIND_COPY(MYSQL_TYPE_LONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }
    
    void MySqlStmt::bind(uint32_t index,const int64_t value){
        BIND_COPY(MYSQL_TYPE_LONGLONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = false;
    }

    void MySqlStmt::bind(uint32_t index,const uint64_t value){
        BIND_COPY(MYSQL_TYPE_LONGLONG,&value,sizeof(value));
        m_MySqlModifyBinds[index].is_unsigned = true;
    }

    void MySqlStmt::bind(uint32_t index,const std::string& value){
        BIND_COPY(MYSQL_TYPE_STRING,value.c_str(),value.size());
    }

    void MySqlStmt::bindTime(uint32_t index,const std::time_t value){
        bind(index,TimeToString(value));
    }

    void MySqlStmt::bindBlob(uint32_t index,const std::string& value){
        BIND_COPY(MYSQL_TYPE_BLOB,value.c_str(),value.size());
    }

    void MySqlStmt::bindBlob(uint32_t index,const void* value,uint64_t size){
        BIND_COPY(MYSQL_TYPE_BLOB,value,size);
    }

    bool MySqlStmt::isNull(uint32_t index){
        RWMutex::ReadLock lock(m_Mutex);
        return m_MySqlResults[index].m_IsNull;
    }

    #define CAST(T)                         \
        RWMutex::ReadLock lock(m_Mutex);    \
        return *reinterpret_cast<T*>(m_MySqlResults.at(index).m_Data)
    int8_t MySqlStmt::getInt8(uint32_t index){
        CAST(int8_t);
    }

    uint8_t MySqlStmt::getUint8(uint32_t index){
        CAST(uint8_t);
    }

    int16_t MySqlStmt::getInt16(uint32_t index){
        CAST(int16_t);
    }

    int16_t MySqlStmt::getUint16(uint32_t index){
        CAST(int16_t);
    }

    int32_t MySqlStmt::getInt32(uint32_t index){
        CAST(int32_t);
    }

    uint32_t MySqlStmt::getUint32(uint32_t index){
        CAST(uint32_t);
    }
    
    int64_t MySqlStmt::getInt64(uint32_t index){
        CAST(int64_t);
    }

    uint64_t MySqlStmt::getUint64(uint32_t index){
        CAST(uint64_t);
    }

    float MySqlStmt::getFloat(uint32_t index){
        CAST(float);
    }

    double MySqlStmt::getDouble(uint32_t index){
        CAST(double);
    }
    #undef CAST

    std::time_t MySqlStmt::getTime(uint32_t index){
        RWMutex::ReadLock lock(m_Mutex);
        MYSQL_TIME* mt = reinterpret_cast<MYSQL_TIME*>(m_MySqlResults[index].m_Data);
        std::time_t ts = 0;
        MySqlTimeToTime(*mt,ts);
        return ts;
    }

    std::string MySqlStmt::getString(uint32_t index){
        RWMutex::ReadLock lock(m_Mutex);
        return std::string(m_MySqlResults[index].m_Data,m_MySqlResults[index].m_Length);
    }

    std::string MySqlStmt::getBlob(uint32_t index){
        RWMutex::ReadLock lock(m_Mutex);
        return std::string(m_MySqlResults[index].m_Data,m_MySqlResults[index].m_Length);
    }

    MySqlStmt::SqlResult::SqlResult()
        :m_Data(nullptr)
        ,m_Error(false)
        ,m_IsNull(false)
        ,m_DataLength(0)
        ,m_Length(0)
        ,m_Type(){
    }

    MySqlStmt::SqlResult::~SqlResult(){
        if(m_Data){
            delete[] m_Data;
        }
    }

    void MySqlStmt::SqlResult::alloc(uint32_t size){
        if(m_Data){
            delete[] m_Data;
        }
        m_Data = new char[size]();
        m_Length = size;
        m_DataLength = size;
    }
}
}
#endif