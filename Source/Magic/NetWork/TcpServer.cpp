/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : TcpServer.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Utilty/Logger.hpp"
#include "Magic/NetWork/TcpServer.hpp"

namespace Magic{
namespace NetWork{
    TcpServer::~TcpServer() = default;

    TcpServer::TcpServer(const Safe<IoPool>& pool,const Safe<Config>& configuration)
        :m_IsRun(false)
        ,m_TimeOutMs(configuration->at<uint64_t>("NetWork.Server.TimeOutMs",60000))
        ,m_IoPool(pool)
        ,m_Address(configuration->at<std::string>("NetWork.Server.IpAddress","0.0.0.0"))
        ,m_NetworkPort(configuration->at<uint16_t>("NetWork.Server.IpPort",8080)){
        m_Acceptor = std::make_shared<asio::ip::tcp::acceptor>(*m_IoPool->get(),asio::ip::tcp::endpoint(asio::ip::address::from_string(m_Address),m_NetworkPort));
    }

    void TcpServer::run(){
        if(m_IsRun)
            return;
        m_IsRun = true;
        MAGIC_INFO() << "Server Start Running";
        MAGIC_INFO() << "Server.IpPort: " << m_NetworkPort;
        MAGIC_INFO() << "Server.IpAddress: " << m_Address;
        this->accept();
    }

    void TcpServer::stop(){
        MAGIC_INFO() << "Server Start Stoping";
        m_IsRun = false;
    }

    void TcpServer::accept(){
        if(!m_IoPool)
            throw Failure("IoPool Is Nullptr");
        Safe<Socket> socket = std::make_shared<Socket>(m_TimeOutMs,4096,*m_IoPool->get());
    #if __cplusplus >= 201402L
        const auto& entity = socket->getEntity();
        m_Acceptor->async_accept(*entity,[this,socket = std::move(socket)](const asio::error_code& err){
    #else
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
    #endif
            if(err){
                /// TODO: ...
                MAGIC_WARN() << err.message();
                return;
            }
            this->handleFunc(socket);
            if(m_IsRun)
                this->accept();
        });
    }
}
}
