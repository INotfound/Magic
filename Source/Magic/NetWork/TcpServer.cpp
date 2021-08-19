/*
 * @File: TcpServer.cpp
 * @Author: INotFound
 * @Date: 2020-03-12 01:33:35
 * @LastEditTime: 2021-02-01 22:24:27
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/TcpServer.h"

namespace Magic{
namespace NetWork{
    TcpServer::~TcpServer(){
    }

    TcpServer::TcpServer(const Safe<IoPool>& pool,const Safe<TimingWheel>& timingWheel,const Safe<Config>& configuration)
        :m_IsRun(false)
        ,m_TimeOutMs(configuration->at<uint64_t>("NetWork.Server.TimeOutMs",100))
        ,m_IoPool(pool)
        ,m_Address(configuration->at<std::string>("NetWork.Server.IpAddress","127.0.0.1"))
        ,m_NetworkPort(configuration->at<uint16_t>("NetWork.Server.IpPort",8080))
        ,m_TimingWheel(timingWheel){
        m_Acceptor = std::make_shared<asio::ip::tcp::acceptor>(m_IoPool->get()
            ,asio::ip::tcp::endpoint(asio::ip::address::from_string(m_Address),m_NetworkPort));
    }

    void TcpServer::run(){
        if(m_IsRun){
            return;
        }

        m_IsRun = true;
        this->accept();
        MAGIC_INFO() << "Server Running";
        MAGIC_INFO() << "Server.IpPort: " << m_NetworkPort;
        MAGIC_INFO() << "Server.IpAddress: " << m_Address;
        m_IoPool->run();
    }

    void TcpServer::stop(){
        MAGIC_INFO() << "Server Stoping";
        m_IsRun = false;
        m_IoPool->stop();
    }
    
    void TcpServer::accept(){
        if(!m_IoPool)
            throw std::logic_error("IoPool Is Nullptr!!!");
        Safe<Socket> socket = std::make_shared<Socket>(m_TimeOutMs,4096,m_IoPool->get(),m_TimingWheel);
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
            socket->enableTimeOut();
            if(err){
                //TODO: ...
                MAGIC_WARN() << err.message();
                return;
            }
            this->handleFunc(socket);
            if(m_IsRun){
                accept();
            }
        });
    }
}
}
