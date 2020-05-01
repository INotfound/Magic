/*
 * @File: TcpServer.cpp
 * @Author: INotFound
 * @Date: 2020-03-12 01:33:35
 * @LastEditTime: 2020-03-14 00:15:18
 */
#include "Macro.h"
#include "TcpServer.h"


namespace Magic{
    TcpServer::~TcpServer(){
    }
    TcpServer::TcpServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs)
        :m_IsRun(false)
        ,m_TimeOutMs(timeOutMs)
        ,m_IoPool(new IoPool(threadCount))
        ,m_Address(addr)
        ,m_NetworkPort(port){
        auto endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(addr),port);
        m_Acceptor.reset(new asio::ip::tcp::acceptor(m_IoPool->get(),endpoint));
    }
    void TcpServer::run(){
        if(m_IsRun){
            return;
        }
        //TODO: ...
        m_IsRun = true;
        this->accept();
        MAGIC_INFO()  << "Server running";
        m_IoPool->run();
    }
    void TcpServer::stop(){
        //TODO: ...
        MAGIC_INFO()  << "Server stoping";
        m_IsRun = false;
    }
    void TcpServer::accept(){
        Share<Socket> socket = std::make_shared<Socket>(m_TimeOutMs,4096,m_IoPool->get());
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
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