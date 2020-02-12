#include "Macro.h"
#include "Http/Http.h"
#include "Http/HttpParser.h"
#include "Http/HttpServer.h"

namespace Magic{
namespace Http{
    HttpServer::HttpServer(const std::string& addr,uint16_t port,uint32_t threadCount)
        :TcpServer(addr,port,threadCount)
        ,m_ServletDispatch(new HttpServletDispatch){
    }
    void HttpServer::accept(){
        Share<Socket> socket = std::make_shared<Socket>(m_IoPool->get());
        m_Acceptor->async_accept(*socket,[this, socket](const asio::error_code& err){
            if(err){
                //TODO: ...
                MAGIC_LOG(LogLevel::LogWarn) << err.message();
                return;
            }
            this->handleFunc(socket);
            if(!m_Stop){
                accept();
            }
        });
    }
    Safe<HttpServletDispatch>& HttpServer::getHttpServletDispatch(){
        return m_ServletDispatch;
    }
    void HttpServer::handleFunc(Share<Socket> socket){
        uint32_t bufferSize = 1024*4;
        auto readStreamBuffer = std::make_shared<asio::streambuf>();
        Share<char> buffer(new char[bufferSize],[](char* ptr){delete[] ptr;});
        asio::async_read_until(*socket
            ,*readStreamBuffer
            ,"\r\n\r\n"
            ,[this,socket,readStreamBuffer,buffer](const asio::error_code &err, std::size_t length){
                if(err){
                    //TODO: ...
                    MAGIC_LOG(LogLevel::LogWarn) << err.message();
                    return;
                }
                length = readStreamBuffer->size();
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
                    asio::async_read(*socket
                        ,*readStreamBuffer
                        ,asio::transfer_exactly(contentLength - lastLength)
                        ,[this,socket,requestParser,readStreamBuffer,buffer,lastLength](const asio::error_code &err, std::size_t length){
                            if(err){
                                //TODO: ...
                                MAGIC_LOG(LogLevel::LogWarn) << err.message();
                                return;
                            }

                            std::string body;
                            auto& request = requestParser->getData();
                            auto streamBuffer = readStreamBuffer->data();
                            body.resize(length + lastLength);
                            body.append(buffer.get(),lastLength);
                            body.append(asio::buffers_begin(streamBuffer),asio::buffers_end(streamBuffer));
                            request->setBody(body);

                            Safe<HttpResponse> response(new HttpResponse(request->getkeepAlive(),request->getVersion()));
                            m_ServletDispatch->handle(request,response);
                            auto writeStreamBuffer = std::make_shared<asio::streambuf>();
                            std::ostream responseStream(writeStreamBuffer.get());
                            responseStream << response;
                            
                            asio::async_write(*socket
                                ,*writeStreamBuffer
                                , asio::transfer_exactly(writeStreamBuffer->size())
                                ,[this,socket, writeStreamBuffer](const asio::error_code &err, std::size_t length){
                                    if(err){
                                        //TODO: ...
                                        MAGIC_LOG(LogLevel::LogWarn) << err.message();
                                        return;
                                    }
                                    handleFunc(socket);
                                }
                            );
                        });
                }else{
                    auto& request = requestParser->getData();
                    Safe<HttpResponse> response(new HttpResponse(request->getkeepAlive(),request->getVersion()));
                    m_ServletDispatch->handle(request,response);
                    auto writeStreamBuffer = std::make_shared<asio::streambuf>();
                    std::ostream responseStream(writeStreamBuffer.get());
                    responseStream << response;
                    asio::async_write(*socket
                        ,*writeStreamBuffer
                        ,asio::transfer_exactly(writeStreamBuffer->size())
                        ,[this,socket, writeStreamBuffer](const asio::error_code &err, std::size_t length){
                            if(err){
                                //TODO: ...
                                MAGIC_LOG(LogLevel::LogWarn) << err.message();
                                return;
                            }
                            handleFunc(socket);
                        }
                    );
                }
        });
    }


}
}