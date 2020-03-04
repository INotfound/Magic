#pragma once
#include "Core.h"
#include <iostream>
namespace Magic{
    class Socket :public std::enable_shared_from_this<Socket>{
    public:
        friend class SocketTimeOutTask;
        virtual ~Socket();
        Socket(uint64_t timeOutMs,asio::io_context& context);
        void enableTimeOut();
        const Safe<asio::ip::tcp::socket>& getEntity() const;
        // void read(std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
        // void read(uint64_t size,std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
        // void write(const std::string& data,std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
        // void write(const char* data,uint64_t size,std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
    protected:
        void updateTimeOut();
        void doRead(std::function<void(const std::error_code& ec)> callBack);
        void doWrite(std::function<void(const std::error_code& ec)> callBack);
        void doRead(uint64_t size,std::function<void(const std::error_code& ec)> callBack);
    private:
        void runTimeOut();
    protected:
        uint64_t m_BufferSize;
        std::vector<char> m_ReadBuffer;
        std::vector<char> m_WriteBuffer;
    private:
        bool m_TimeOut;
        uint64_t m_TimeOutMs;
        Safe<char[]> m_BlockBuffer;
        Safe<asio::ip::tcp::socket> m_Socket;
    };
}