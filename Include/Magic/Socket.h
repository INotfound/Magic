#pragma once
#include "Core.h"
#include "Macro.h"
#include "TimingWheel.h"
#include <iostream>
namespace Magic{
    // class Socket :public std::enable_shared_from_this<Socket>{
    // public:
    //     virtual ~Socket();
    //     Socket(uint64_t bufferSize,uint64_t timeOutMs,asio::io_context& context);
    //     void clearData();
    //     void useTimeOut();
    //     void updateTimeOut();
    //     std::vector<char>& getData();
    //     const Safe<asio::ip::tcp::socket>& getEntity() const;
    // private:
    //     enum class State{
    //         Ready,
    //         Reset
    //     };
    //     class SocketTimeOutTask :public ITaskNode{
    //     public:
    //         SocketTimeOutTask(const Share<Socket>& socket);
    //         void notify() override;
    //     private:
    //         Share<Socket> m_Socket;
    //     };
    // private:
    //     void timeOut();
    // private:
    //     bool m_IsTimeOut;
    //     uint64_t m_TimeOutMs;
    //     State m_TimeOutState;
    //     std::vector<char> m_Data;
    //     Safe<asio::ip::tcp::socket> m_Socket;
    // };

    class Socket :public std::enable_shared_from_this<Socket>{
    public:
        virtual ~Socket(){
            m_Socket->close();
            MAGIC_LOG(LogLevel::LogWarn) << "Socket Go dead!!!";
        }
        Socket(uint64_t timeOutMs,asio::io_context& context)
            :m_BufferSize(4096)
            ,m_Socket(new asio::ip::tcp::socket(context)){
            m_Buffer.reset(new char[m_BufferSize]);
            m_ReadBuffer.reserve(m_BufferSize);
            m_WriteBuffer.reserve(m_BufferSize);
            std::fill(m_Buffer.get(),m_Buffer.get() + m_BufferSize,0);
        }
        void doRead(std::function<void()> callBack){
            asio::async_read(*m_Socket
                ,asio::buffer(m_Buffer.get(),m_BufferSize)
                ,asio::transfer_at_least(1)
                ,[this,callBack](const asio::error_code &err, std::size_t length){
                if(err){
                    //TODO: ...
                    if(err == asio::error::eof || err == asio::error::connection_reset ){
                        return;
                    }
                    MAGIC_LOG(LogLevel::LogWarn) << err.message();
                    return;
                }
                //|| err == asio::error::operation_aborted
                m_ReadBuffer.insert(m_ReadBuffer.begin() + m_ReadBuffer.size()
                    ,m_Buffer.get()
                    ,m_Buffer.get() + length);
                callBack();
            });
        }
        void doRead(uint64_t size,std::function<void()> callBack){
            asio::async_read(*m_Socket
                ,asio::buffer(m_Buffer.get(),m_BufferSize)
                ,asio::transfer_exactly(size)
                ,[this,callBack](const asio::error_code &err, std::size_t length){
                if(err){
                    //TODO: ...
                    if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
                        return;
                    }
                    MAGIC_LOG(LogLevel::LogWarn) << err.message();
                    return;
                }
                m_ReadBuffer.insert(m_ReadBuffer.begin() + m_ReadBuffer.size()
                    ,m_Buffer.get()
                    ,m_Buffer.get() + length);
                callBack();
            });
        }
        void doWrite(std::function<void()> callBack){
            asio::async_write(*m_Socket
                ,asio::const_buffer(m_WriteBuffer.data(),m_WriteBuffer.size())
                ,[this,callBack](const asio::error_code &err, std::size_t length){
                if(err){
                    //TODO: ...
                    if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
                        return;
                    }
                    MAGIC_LOG(LogLevel::LogWarn) << err.message();
                    return;
                }
                callBack();
            });
        }
        const Safe<asio::ip::tcp::socket>& getEntity() const{
            return m_Socket;
        }
    protected:
        void enableTimeOut(){
        }
        void updateTimeOut(){
        }
    protected:
        uint64_t m_BufferSize;
        std::vector<char> m_ReadBuffer;
        std::vector<char> m_WriteBuffer;
    private:
        bool m_IsTimeOut;
        Safe<char[]> m_Buffer;
        Safe<asio::ip::tcp::socket> m_Socket;
    };
}