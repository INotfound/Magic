/*
 * @Author: INotFound
 * @Date: 2020-03-12 21:03:34
 * @LastEditTime: 2020-10-01 16:39:47
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpSocket.h"
#include "Magic/NetWork/Http/HttpServer.h"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpServer::HttpServer(const Safe<IoPool>& pool,const Safe<TimingWheel>& timingWheel,const Safe<Config>& configuration)
        :TcpServer(pool,timingWheel,configuration)
        ,m_EnableSsl(configuration->at<bool>("NetWork.Server.EnableSsl",false))
        ,m_KeyFile(configuration->at<std::string>("NetWork.Server.Ssl.KeyFile",""))
        ,m_CertFile(configuration->at<std::string>("NetWork.Server.Ssl.CertFile",""))
        ,m_TempDirectory(configuration->at<std::string>("NetWork.Server.TempDirectory","./")){
        m_Acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    }

    void HttpServer::setServletDispatch(const Safe<HttpServletDispatch>& dispatch){
        m_ServletDispatch = dispatch;
    }

    void HttpServer::accept(){
        Safe<Socket> socket = std::make_shared<Socket>(m_TimeOutMs,4096,m_IoPool->get(),m_TimingWheel);
    #ifdef OPENSSL
        if(m_EnableSsl){
            asio::ssl::context sslContext(asio::ssl::context::sslv23);
            sslContext.set_options(asio::ssl::context::no_sslv2
                                   | asio::ssl::context::single_dh_use
                                   | asio::ssl::context::default_workarounds);
            sslContext.use_certificate_chain_file(m_CertFile);
            sslContext.use_private_key_file(m_KeyFile,asio::ssl::context::pem);
            socket->enableSsl(std::make_shared<asio::ssl::stream<asio::ip::tcp::socket&>>(*socket->getEntity(),sslContext));
        }
    #endif
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
            if(!err){
                socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
            #ifdef OPENSSL
                auto sslStream = socket->getSslEntity();
                if(sslStream){
                    sslStream->async_handshake(asio::ssl::stream_base::server,[this,socket](const asio::error_code& err){
                        if(err){
                            socket->close();
                            MAGIC_WARN() <<  err.message();
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
                MAGIC_WARN() <<  err.message();
            }
            if(m_IsRun)
                this->accept();
        });
    }
    
    void HttpServer::handleFunc(const Safe<Socket>& socket){
        Safe<HttpSocket> httpSocket = std::make_shared<HttpSocket>(socket);
        socket->runHeartBeat(httpSocket);
        httpSocket->setTempDirectory(m_TempDirectory);
        httpSocket->recvRequest(std::bind(&HttpServletDispatch::handle,m_ServletDispatch.get(),std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    }
}
}
}
