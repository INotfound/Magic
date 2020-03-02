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
    // void HttpServer::accept(){
    //     Share<Socket> socket = std::make_shared<Socket>(4*1024,m_TimeOutMs,m_IoPool->get());
    //     m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
    //         if(!err){
    //             socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
    //             socket->useTimeOut();
    //             this->handleFunc(socket);
    //         }else{
    //             //TODO: ...
    //             if(err != asio::error::eof || err != asio::error::connection_reset || err == asio::error::operation_aborted){
    //                 MAGIC_LOG(LogLevel::LogWarn) << err.message();
    //             }
    //         }
    //         if(m_IsRun){
    //             accept();
    //         }
    //     });
    // }
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
        httpSocket->Go([this](const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
            this->m_ServletDispatch->handle(request,response);
        });
    }
    // void HttpServer::handleFunc(const Share<Socket>& socket){
    //     socket->clearData();
    //     auto readStreamBuffer = std::make_shared<asio::streambuf>();
    //     asio::async_read_until(*socket->getEntity()
    //         ,*readStreamBuffer
    //         ,"\r\n\r\n"
    //         ,[this,socket,readStreamBuffer](const asio::error_code &err, std::size_t length){
    //         if(err){
    //             //TODO: ...
    //             if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
    //                 return;
    //             }
    //             MAGIC_LOG(LogLevel::LogWarn) << err.message();
    //             return;
    //         }
    //         socket->updateTimeOut();
    //         //解析Request
    //         char* buffer = socket->getData().data();
    //         length = readStreamBuffer->size();
    //         Share<HttpRequestParser> requestParser = std::make_shared<HttpRequestParser>();
    //         readStreamBuffer->sgetn(buffer,length);
    //         uint32_t parserLength = requestParser->execute(buffer,length);
    //         uint32_t lastLength = length - parserLength;
    //         do{
    //             if(requestParser->hasError()){
    //                 return;
    //             }
    //             if(requestParser->isFinished()){
    //                 break;
    //             }
    //         }while(true);
    //         //判断是否有内容
    //         uint64_t contentLength = requestParser->getContentLength();
    //         if(contentLength > 0){
    //             asio::async_read(*socket->getEntity()
    //                 ,*readStreamBuffer
    //                 ,asio::transfer_exactly(contentLength - lastLength)
    //                 ,[this,socket,requestParser,readStreamBuffer,lastLength](const asio::error_code &err, std::size_t length){
    //                 if(err){
    //                     //TODO: ...
    //                     if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
    //                         return;
    //                     }
    //                     MAGIC_LOG(LogLevel::LogWarn) << err.message();
    //                     return;
    //                 }
    //                 std::string body;
    //                 char* buffer = socket->getData().data();
    //                 auto& request = requestParser->getData();
    //                 auto streamBuffer = readStreamBuffer->data();
    //                 body.reserve(length+lastLength);
    //                 body.append(buffer,lastLength);
    //                 body.append(asio::buffers_begin(streamBuffer),asio::buffers_end(streamBuffer));
    //                 request->setBody(body);
    //                 //响应报文
    //                 Safe<HttpResponse> response(new HttpResponse(request->getkeepAlive(),request->getVersion()));
    //                 m_ServletDispatch->handle(request,response);
    //                 auto writeStreamBuffer = std::make_shared<asio::streambuf>();
    //                 std::ostream responseStream(writeStreamBuffer.get());
    //                 responseStream << response;
    //                 asio::async_write(*socket->getEntity()
    //                     ,*writeStreamBuffer
    //                     , asio::transfer_exactly(writeStreamBuffer->size())
    //                     ,[this,socket,writeStreamBuffer](const asio::error_code &err, std::size_t length){
    //                     if(err){
    //                         //TODO: ...
    //                         if(err == asio::error::eof || err == asio::error::connection_reset){
    //                             return;
    //                         }
    //                         MAGIC_LOG(LogLevel::LogWarn) << err.message();
    //                         return;
    //                     }
    //                     handleFunc(socket);
    //                 });
    //             });
    //         }else{
    //             //响应报文
    //             auto& request = requestParser->getData();
    //             Safe<HttpResponse> response(new HttpResponse(request->getkeepAlive(),request->getVersion()));
    //             m_ServletDispatch->handle(request,response);
    //             auto writeStreamBuffer = std::make_shared<asio::streambuf>();
    //             std::ostream responseStream(writeStreamBuffer.get());
    //             responseStream << response;
    //             asio::async_write(*socket->getEntity()
    //                 ,*writeStreamBuffer
    //                 ,asio::transfer_exactly(writeStreamBuffer->size())
    //                 ,[this,socket,writeStreamBuffer](const asio::error_code &err, std::size_t length){
    //                 if(err){
    //                     //TODO: ...
    //                     if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
    //                         return;
    //                     }
    //                     MAGIC_LOG(LogLevel::LogWarn) << err.message();
    //                     return;
    //                 }
    //                 handleFunc(socket);
    //             });
    //         }
    //     });
    // }

}
}