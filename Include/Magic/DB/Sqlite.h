/*
 * @File: Sqlite.h
 * @Author: INotFound
 * @Date: 2020-02-18 11:35:25
 * @LastEditTime: 2020-04-06 17:29:16
 */
#pragma once
#include <mysql/mysql.h>

#include "Sql.h"
#include "../Core.h"
#include "../Mutex.h"

namespace DB{
    class Sqlite :public ISql{
    public:
        bool execute(const std::string& sql) override;
    private:
    };
    class SqliteStmt :public ISqlStmt{
    public:
        ~SqliteStmt();
        SqliteStmt(const Safe<Sqlite>& sql);
        bool next() override;
        bool query() override;
        bool execute() override;
        void printError() override;
        bool prepare(const std::string& sql) override;
        void bindNull(uint32_t index) override;
        void bind(uint32_t index,const float& value) override;
        void bind(uint32_t index,const double& value) override;
        void bind(uint32_t index,const int8_t& value) override;
        void bind(uint32_t index,const uint8_t& value) override;
        void bind(uint32_t index,const int16_t& value) override;
        void bind(uint32_t index,const uint16_t& value) override;
        void bind(uint32_t index,const int32_t& value) override;
        void bind(uint32_t index,const uint32_t& value) override;
        void bind(uint32_t index,const int64_t& value) override;
        void bind(uint32_t index,const uint64_t& value) override;
        void bind(uint32_t index,const std::string& value) override;
        void bindTime(uint32_t index,const time_t& value) override;
        void bindBlob(uint32_t index,const std::string& value) override;
        void bindBlob(uint32_t index,const void* value,uint64_t size) override;

        bool isNull(uint32_t index) override;
        int8_t getInt8(uint32_t index) override;
        uint8_t getUint8(uint32_t index) override;
        int16_t getInt16(uint32_t index) override;
        int16_t getUint16(uint32_t index) override;
        int32_t getInt32(uint32_t index) override;
        uint32_t getUint32(uint32_t index) override;
        int64_t getInt64(uint32_t index) override;
        uint64_t getUint64(uint32_t index) override;
        float getFloat(uint32_t index) override;
        double getDouble(uint32_t index) override;
        time_t getTime(uint32_t index) override;
        std::string getString(uint32_t index) override;
        std::string getBlob(uint32_t index) override;
    private:
    };
}