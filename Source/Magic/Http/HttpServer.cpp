#include "Macro.h"
#include "Http/Http.h"
#include "Http/HttpParser.h"
#include "Http/HttpServer.h"

namespace Magic{
namespace Http{
    HttpServer::HttpServer(const std::string& addr,uint16_t port,uint32_t threadCount)
        :TcpServer{addr,port,threadCount}
        ,m_ServletDispatch{new HttpServletDispatch}{
    }
    void HttpServer::accept(){
        Share<Session> session{new HttpSession{m_IoPool->get()}};
        auto& socket = session->socket();
        m_Acceptor->async_accept(*socket,[this, session](const asio::error_code& err){
            if(err){
                //TODO: ...
                MAGIC_LOG(LogLevel::LogWarn) <<"ErrorCode: " << err.value() <<" ErrorMsg: "  << err.message();
                return;
            }
            this->handleFunc(session);
            if(!m_Stop){
                accept();
            }
        });
    }
    Safe<HttpServletDispatch>& HttpServer::getHttpServletDispatch(){
        return m_ServletDispatch;
    }
    void HttpServer::handleFunc(Share<Session> session){
        //TODO:: ...
        Share<HttpSession> httpSession = std::static_pointer_cast<HttpSession>(session);
        process(httpSession);
    }
    void HttpServer::process(Share<HttpSession> session){
        uint32_t bufferSize = 1024*4;
        Share<char> buffer(new char[bufferSize],[](char* ptr){delete[] ptr;});
        auto readStreamBuffer = std::make_shared<asio::streambuf>();
        asio::async_read_until(*session->socket()
            ,*readStreamBuffer
            ,"\r\n\r\n"
            ,[this,session,readStreamBuffer,buffer](const asio::error_code &err, std::size_t length){
                if(err){
                    //TODO: ...
                    MAGIC_LOG(LogLevel::LogWarn) <<"ErrorCode: " << err.value() <<" ErrorMsg: "  << err.message();
                    return;
                }
                Share<HttpRequestParser> requestParser = std::make_shared<HttpRequestParser>();
                readStreamBuffer->sgetn(buffer.get(),length);
                uint32_t parserLength = requestParser->execute(buffer.get(),length);
                uint32_t lastLength = length - parserLength;
                do{
                    if(requestParser->hasError()){
                        return;
                    }
                    if(requestParser->isFinished()){
                        break;
                    }
                }while(true);
                uint64_t contentLength = requestParser->getContentLength();
                if(contentLength > 0){

                    asio::async_read(*session->socket()
                        ,*readStreamBuffer
                        ,asio::transfer_exactly(contentLength)
                        ,[this,session,requestParser,readStreamBuffer,buffer,lastLength](const asio::error_code &err, std::size_t length){
                            if(err){
                                //TODO: ...
                                MAGIC_LOG(LogLevel::LogWarn) <<"ErrorCode: " << err.value() <<" ErrorMsg: "  << err.message();
                                return;
                            }

                            std::string body{};
                            auto& request = requestParser->getData();
                            auto streamBuffer = readStreamBuffer->data();
                            body.resize(length + lastLength);
                            body.append(buffer.get(),lastLength);
                            body.append(asio::buffers_begin(streamBuffer),asio::buffers_end(streamBuffer));
                            request->setBody(body);

                            Safe<HttpResponse> response{new HttpResponse{request->getkeepAlive(),request->getVersion()}};
                            response->setHeader("Server","MagicServer");
                            m_ServletDispatch->handle(session,request,response);
                            auto writeBuffer = std::make_shared<asio::streambuf>();
                            std::ostream responseStream(writeBuffer.get());
                            responseStream << response;
                            
                            asio::async_write(*session->socket()
                                ,*writeBuffer
                                ,[this,session](const asio::error_code &err, std::size_t length){
                                    if(err){
                                        //TODO: ...
                                        MAGIC_LOG(LogLevel::LogWarn) <<"ErrorCode: " << err.value() <<" ErrorMsg: "  << err.message();
                                        return;
                                    }
                                    process(session);
                            });
                        });
                }else{
                    auto& request = requestParser->getData();
                    Safe<HttpResponse> response{new HttpResponse{request->getkeepAlive(),request->getVersion()}};
                    m_ServletDispatch->handle(session,requestParser->getData(),response);
                    auto writeBuffer = std::make_shared<asio::streambuf>();
                    std::ostream responseStream(writeBuffer.get());
                    responseStream << response;
                    asio::async_write(*session->socket()
                        ,*writeBuffer
                        ,[this,session](const asio::error_code &err, std::size_t length){
                            if(err){
                                //TODO: ...
                                MAGIC_LOG(LogLevel::LogWarn) <<"ErrorCode: " << err.value() <<" ErrorMsg: "  << err.message();
                                return;
                            }
                            process(session);
                    });
                }
        });
    }

}
}