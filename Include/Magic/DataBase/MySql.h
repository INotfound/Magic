/*
 * @Author: INotFound
 * @Date: 2020-02-18 11:35:25
 * @LastEditTime: 2021-02-01 22:20:28
 */
#pragma once
#ifdef MYSQL
#include <mysql/mysql.h>

#include "Magic/Core/Core.h"
#include "Magic/Utilty/Mutex.h"
#include "Magic/DataBase/ConnectionPool.h"

namespace Magic{
namespace DataBase{
    class MySql{
        friend class MySqlStmt;
    public:
        ~MySql();
        MySql();
        void printError();
        bool execute(const std::string& sql);
        bool connnetDB(const std::string& dataBase,const std::string& ip,const std::string& user,const std::string&  password,uint16_t port);
    private:
        Mutex m_Mutex;
        MYSQL m_MySql;
    };
    class MySqlStmt{
    public:
        ~MySqlStmt();
        MySqlStmt(const Safe<MySql>& sql);
        bool fetch();
        bool query();
        bool execute();
        uint64_t rows();
        void printError();
        void bindNull(uint32_t index);
        bool prepare(const std::string& sql);
        void bind(uint32_t index,const float value);
        void bind(uint32_t index,const double value);
        void bind(uint32_t index,const int8_t value);
        void bind(uint32_t index,const uint8_t value);
        void bind(uint32_t index,const int16_t value);
        void bind(uint32_t index,const uint16_t value);
        void bind(uint32_t index,const int32_t value);
        void bind(uint32_t index,const uint32_t value);
        void bind(uint32_t index,const int64_t value);
        void bind(uint32_t index,const uint64_t value);
        void bind(uint32_t index,const std::string& value);
        void bindTime(uint32_t index,const std::time_t value);
        void bindBlob(uint32_t index,const std::string& value);
        void bindBlob(uint32_t index,const void* value,uint64_t size);

        bool isNull(uint32_t index);
        int8_t getInt8(uint32_t index);
        uint8_t getUint8(uint32_t index);
        int16_t getInt16(uint32_t index);
        int16_t getUint16(uint32_t index);
        int32_t getInt32(uint32_t index);
        uint32_t getUint32(uint32_t index);
        int64_t getInt64(uint32_t index);
        uint64_t getUint64(uint32_t index);
        float getFloat(uint32_t index);
        double getDouble(uint32_t index);
        std::time_t getTime(uint32_t index);
        std::string getString(uint32_t index);
        std::string getBlob(uint32_t index);

    private:
        class SqlResult{
        public:
            SqlResult();
            ~SqlResult();
            void alloc(uint32_t size);
        public:
            char* m_Data;
            my_bool m_Error;
            my_bool m_IsNull;
            uint32_t m_DataLength;
            unsigned long m_Length;
            enum_field_types m_Type;
        };
    private:
        RWMutex m_Mutex;
        MYSQL_STMT* m_Stmt;
        const Safe<MySql>& m_MySql;
        std::vector<SqlResult>  m_MySqlResults;
        std::vector<MYSQL_BIND> m_MySqlModifyBinds;
        std::vector<MYSQL_BIND> m_MySqlResultBinds;
    };

}
}
#endif