/*
 * @file: Sql.h
 * @Author: INotFound
 * @Date: 2020-02-18 11:35:25
 * @LastEditTime: 2020-04-06 17:29:16
 */
#pragma once
#include "../Core.h"
namespace Magic{
namespace DB{
    class ISql{
    public:
        virtual ~ISql(){};
        virtual bool execute(const std::string& sql) =0;
        virtual bool connnetDB(const std::string& dataBase,const std::string& ip,const std::string& user,const std::string&  password,uint16_t port) =0;
    };
    class ISqlStmt{
    public:
        virtual ~ISqlStmt(){};
        virtual bool next() =0;
        virtual bool query() =0;
        virtual bool execute() =0;
        virtual void printError() =0;
        virtual bool prepare(const std::string& sql) =0;
        virtual void bindNull(uint32_t index) =0;
        virtual void bind(uint32_t index,const float& value) =0;
        virtual void bind(uint32_t index,const double& value) =0;
        virtual void bind(uint32_t index,const int8_t& value) =0;
        virtual void bind(uint32_t index,const uint8_t& value) =0;
        virtual void bind(uint32_t index,const int16_t& value) =0;
        virtual void bind(uint32_t index,const uint16_t& value) =0;
        virtual void bind(uint32_t index,const int32_t& value) =0;
        virtual void bind(uint32_t index,const uint32_t& value) =0;
        virtual void bind(uint32_t index,const int64_t& value) =0;
        virtual void bind(uint32_t index,const uint64_t& value) =0;
        virtual void bind(uint32_t index,const std::string& value) =0;
        virtual void bindTime(uint32_t index,const time_t& value) =0;
        virtual void bindBlob(uint32_t index,const std::string& value) =0;
        virtual void bindBlob(uint32_t index,const void* value,uint64_t size) =0;

        virtual bool isNull(uint32_t index) =0;
        virtual int8_t getInt8(uint32_t index) =0;
        virtual uint8_t getUint8(uint32_t index) =0;
        virtual int16_t getInt16(uint32_t index) =0;
        virtual int16_t getUint16(uint32_t index) =0;
        virtual int32_t getInt32(uint32_t index) =0;
        virtual uint32_t getUint32(uint32_t index) =0;
        virtual int64_t getInt64(uint32_t index) =0;
        virtual uint64_t getUint64(uint32_t index) =0;
        virtual float getFloat(uint32_t index) =0;
        virtual double getDouble(uint32_t index) =0;
        virtual time_t getTime(uint32_t index) =0;
        virtual std::string getBlob(uint32_t index) =0;
        virtual std::string getString(uint32_t index) =0;
    };
}
}