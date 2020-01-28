#pragma once
#define ASIO_STANDALONE
#define ASIO_HAS_STD_CHRONO

#include "Core.h"
#include "IoPool.h"
#include "asio.hpp"
namespace Magic{

    class Session {
    public:
        Session(asio::io_context& io, uint32_t block_size);
        asio::ip::tcp::socket& socket();
    private:
        asio::ip::tcp::socket m_Socket;
    };

    class TcpServer{
    public:
        virtual ~TcpServer();
        TcpServer(const std::string& addr,uint16_t port,uint32_t threadCount);
        void run();
        void stop();
    protected:
        virtual void accept();
        virtual void hanldeFunc(std::shared_ptr<asio::ip::tcp::socket> socket);
    private:
        bool m_Stop{false};
        std::string m_Address{};
        uint16_t m_NetworkPort{};
        MagicPtr<IoPool> m_IoPool{};
        MagicPtr<asio::ip::tcp::acceptor> m_Acceptor{};
    };
}