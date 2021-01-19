/*
 * @Author: INotFound
 * @Date: 2020-03-12 21:03:34
 * @LastEditTime: 2020-10-01 16:39:47
 */
#include "Core/Logger.h"
#include "NetWork/Http/Http.h"
#include "NetWork/Http/HttpSocket.h"
#include "NetWork/Http/HttpParser.h"
#include "NetWork/Http/HttpServer.h"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpServer::HttpServer(const Safe<IoPool>& pool,const Safe<Config>& configuration)
        :TcpServer(pool,configuration){
        m_Acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    }

    void HttpServer::setServletDispatch(const Safe<HttpServletDispatch>& dispatch){
        m_ServletDispatch = dispatch;
        this->run();
    }

    void HttpServer::accept(){
        Safe<Socket> socket = std::make_shared<HttpSocket>(m_TimeOutMs,m_IoPool->get());
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
            if(!err){
                socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
                socket->setErrorCodeCallBack([](const asio::error_code & err){
                    if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
                        return;
                    }
                    MAGIC_WARN() <<  err.message();
                });
                this->handleFunc(socket);
            }else{
                if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
                        return;
                }
                MAGIC_WARN() <<  err.message();
            }
            if(m_IsRun){
                this->accept();
            }
        });
    }
    
    void HttpServer::handleFunc(const Safe<Socket>& socket){
        Safe<HttpSocket> httpSocket = std::static_pointer_cast<HttpSocket>(socket);
        httpSocket->recvRequest(std::bind(&HttpServletDispatch::handle,m_ServletDispatch.get(),std::placeholders::_1,std::placeholders::_2));
    }
}
}
}