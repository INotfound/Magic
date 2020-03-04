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
    void HttpServer::accept(){
        Share<Socket> socket = std::make_shared<HttpSocket>(m_TimeOutMs,m_IoPool->get());
        m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
            if(!err){
                socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
                this->handleFunc(socket);
            }else{
                //TODO: ...
                if(err != asio::error::eof || err != asio::error::connection_reset || err == asio::error::operation_aborted){
                    MAGIC_LOG(LogLevel::LogWarn) << err.message();
                }
            }
            if(m_IsRun){
                accept();
            }
        });
    }
    const Safe<HttpServletDispatch>& HttpServer::getHttpServletDispatch(){
        return m_ServletDispatch;
    }
    void HttpServer::handleFunc(const Share<Socket>& socket){
        Share<HttpSocket> httpSocket = std::static_pointer_cast<HttpSocket>(socket);
        httpSocket->enableTimeOut();
        httpSocket->Go([this](const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
            this->m_ServletDispatch->handle(request,response);
        });
    }
}
}