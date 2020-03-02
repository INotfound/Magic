#pragma once
#include "Core.h"
#include "IoPool.h"
#include "Socket.h"
namespace Magic{
    class TcpServer{
    public:
        virtual ~TcpServer();
        TcpServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs);
        void run();
        void stop();
    protected:
        virtual void accept();
        virtual void handleFunc(const Share<Socket>& socket) =0;
    protected:
        bool m_IsRun;
        uint64_t m_TimeOutMs;
        Safe<IoPool> m_IoPool;
        std::string m_Address;
        uint16_t m_NetworkPort;
        Safe<asio::ip::tcp::acceptor> m_Acceptor;
    };
}