/*
 * @Author: INotFound
 * @Date: 2020-03-12 21:03:34
 * @LastEditTime: 2020-10-01 16:39:47
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpSocket.h"
#include "Magic/NetWork/Http/HttpParser.h"
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
        this->run();
    }

    void HttpServer::accept(){
        Safe<HttpSocket> socket = std::make_shared<HttpSocket>(m_TimeOutMs,m_IoPool->get(),m_TimingWheel);
        socket->setTempDirectory(m_TempDirectory);
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
            socket->enableTimeOut();
            if(!err){
                socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
                if(err == asio::error::eof
                   || err == asio::error::broken_pipe
                   || err == asio::error::connection_reset
                #ifdef OPENSSL
                   || err == asio::error::operation_aborted
                   || err == asio::ssl::error::stream_truncated){
                #else
                   || err == asio::error::operation_aborted){
                #endif
                    return;
                }
                socket->setErrorCodeCallBack([](const asio::error_code& err){
                #ifdef WIN32
                    if(err.value() == WSAECONNABORTED) return;
                #endif
                    if(err == asio::error::eof
                       || err == asio::error::broken_pipe
                       || err == asio::error::connection_reset
                   #ifdef OPENSSL
                       || err == asio::error::operation_aborted
                       || err == asio::ssl::error::stream_truncated){
                    #else
                        || err == asio::error::operation_aborted){
                    #endif
                        return;
                    }
                    MAGIC_WARN() << err.message();
                });
            #ifdef OPENSSL
                auto sslStream = socket->getSslEntity();
                if(sslStream){
                    sslStream->async_handshake(asio::ssl::stream_base::server,[this,socket](const asio::error_code& err){
                        if(err){
                            if(err == asio::ssl::error::stream_truncated){
                                return;
                            }
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
