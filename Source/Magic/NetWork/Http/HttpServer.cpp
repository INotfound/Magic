/*
 * @Author: INotFound
 * @Date: 2020-03-12 21:03:34
 * @LastEditTime: 2020-10-01 16:39:47
 */
#include "Magic/Utilty/Logger.hpp"
#include "Magic/NetWork/Http/Http.hpp"
#include "Magic/NetWork/Http/HttpSocket.hpp"
#include "Magic/NetWork/Http/HttpServer.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpServer::HttpServer(const Safe<IoPool>& pool,const Safe<Config>& configuration)
        :TcpServer(pool,configuration)
        ,m_EnableSsl(configuration->at<bool>("NetWork.Server.EnableSsl",false))
        ,m_KeyFile(configuration->at<std::string>("NetWork.Server.Ssl.KeyFile",""))
        ,m_CertFile(configuration->at<std::string>("NetWork.Server.Ssl.CertFile",""))
        ,m_UploadDirectory(configuration->at<std::string>("NetWork.Server.UploadDirectory",".")){
        m_Acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    }

    void HttpServer::setServletDispatch(const Safe<HttpServletDispatch>& dispatch){
        m_ServletDispatch = dispatch;
    }

    void HttpServer::accept(){
        Safe<Socket> socket = std::make_shared<Socket>(m_TimeOutMs,4096,*m_IoPool->get());
    #ifdef OPENSSL
        if(m_EnableSsl){
            socket->enableSsl(m_KeyFile,m_CertFile);
        }
    #endif
    const auto& entity = socket->getEntity();
    #if __cplusplus >= 201402L
        m_Acceptor->async_accept(*entity,[this,socket = std::move(socket)](const asio::error_code& err){
    #else
        m_Acceptor->async_accept(*entity,[this,socket](const asio::error_code& err){
    #endif
            if(!err){
                socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
            #ifdef OPENSSL
                const auto& sslStream = socket->getSslEntity();
                if(sslStream){
                #if __cplusplus >= 201402L
                    sslStream->async_handshake(asio::ssl::stream_base::server,[this,socket = std::move(const_cast<Safe<Socket>&>(socket))](const asio::error_code& err){
                #else
                    sslStream->async_handshake(asio::ssl::stream_base::server,[this,socket](const asio::error_code& err){
                #endif
                        if(err){
                            socket->close();
                            MAGIC_WARN() << err.message();
                            return;
                        }
                        this->handleFunc(socket);
                    });
                }else{
            #else
                {
            #endif
                    this->handleFunc(socket);
                }
            }else{
                socket->close();
                if(err == asio::error::operation_aborted)
                    return;
                MAGIC_WARN() << err.message();
            }
            if(m_IsRun)
                this->accept();
        });
    }

    void HttpServer::handleFunc(const Safe<Socket>& socket){
        Safe<HttpSocket> httpSocket = std::make_shared<HttpSocket>(socket);
        httpSocket->runHeartBeat();
        httpSocket->setDirectory(m_UploadDirectory);
        httpSocket->recvRequest([this](const Safe<HttpSocket>& httpSocket){ m_ServletDispatch->handle(httpSocket); });
    }
}
}
}
