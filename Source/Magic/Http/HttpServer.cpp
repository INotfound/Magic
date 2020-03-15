/*
 * @file: HttpServer.cpp
 * @Author: INotFound
 * @Date: 2020-03-12 21:03:34
 * @LastEditTime: 2020-03-15 17:56:19
 */
#include "Macro.h"
#include "Http/Http.h"
#include "Http/HttpSocket.h"
#include "Http/HttpParser.h"
#include "Http/HttpServer.h"

namespace Magic{
namespace Http{
    HttpServer::HttpServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs)
        :TcpServer(addr,port,threadCount,timeOutMs)
        ,m_ServletDispatch(new HttpServletDispatch){
        m_Acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    }
    const Safe<HttpServletDispatch>& HttpServer::getHttpServletDispatch(){
        return m_ServletDispatch;
    }
    void HttpServer::accept(){
        Share<Socket> socket = std::make_shared<HttpSocket>(m_TimeOutMs,m_IoPool->get());
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
            if(!err){
                socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
                socket->setErrorCodeCallBack([](const asio::error_code & err){
                    if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
                        return;
                    }
                    MAGIC_LOG(LogLevel::LogWarn) <<  err.message();
                });
                this->handleFunc(socket);
            }else{
                if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
                        return;
                }
                MAGIC_LOG(LogLevel::LogWarn) <<  err.message();
            }
            if(m_IsRun){
                accept();
            }
        });
    }
    void HttpServer::handleFunc(const Share<Socket>& socket){
        Share<HttpSocket> httpSocket = std::static_pointer_cast<HttpSocket>(socket);
        //httpSocket->enableTimeOut();
        httpSocket->recvRequest(std::bind(&HttpServletDispatch::handle,m_ServletDispatch.get(),std::placeholders::_1,std::placeholders::_2));
    }
}
}