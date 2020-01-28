#include "TcpServer.h"
#include "Macro.h"

namespace Magic{
    TcpServer::~TcpServer(){
    }
    TcpServer::TcpServer(const std::string& addr,uint16_t port,uint32_t threadCount)
        :m_Address{addr}
        ,m_NetworkPort{port}
        ,m_IoPool{new IoPool{threadCount}}{
        auto endpoint = asio::ip::tcp::endpoint{asio::ip::address::from_string(addr),port};
        m_Acceptor.reset(new asio::ip::tcp::acceptor{m_IoPool->get(),endpoint});
    }
    void TcpServer::run(){
        if(m_Stop){
            return;
        }
        //TODO: ...
        accept();
        m_IoPool->run();
    }
    void TcpServer::stop(){
        //TODO: ...
        m_Stop = true;
    }
                

    void TcpServer::accept(){
        std::shared_ptr<asio::ip::tcp::socket> socket{ new asio::ip::tcp::socket{m_IoPool->get()} };
        m_Acceptor->async_accept(*socket,[this,socket](const asio::error_code& err){
            MAGIC_LOG(Magic::LogLevel::LogInfo) << "accept";
            if(err){
                //TODO: ...
                MAGIC_LOG(Magic::LogLevel::LogWarn) << err.message();
                return;
            }
            this->hanldeFunc(socket);
            if(!m_Stop){
                accept();
            }
        });
    }

    void TcpServer::hanldeFunc(std::shared_ptr<asio::ip::tcp::socket> socket){
    }
}