#pragma once
#include "Core.h"
#include "Macro.h"
#include "Socket.h"
#include "Http/Http.h"
#include "Http/HttpParser.h"
namespace Magic{
namespace Http{
    class HttpSocket :public Socket{
    public:
        typedef std::function<void(const Safe<HttpRequest>&,const Safe<HttpResponse>&)> HttpHandle;
        HttpSocket(uint64_t timeOutMs,asio::io_context& context)
            :Socket(timeOutMs,context){
            m_RequestParser.reset(new HttpRequestParser);
            m_ResponseParser.reset(new HttpResponseParser);
        }
        void Go(HttpHandle callBack){
            handle();
            m_CallBack = std::move(callBack);
        }
    private:
        void handle(){
            handleHeader();
        }
        void handleBody(){
            auto self = this->shared_from_this();
            uint64_t contentLength = m_RequestParser->getContentLength();
            if(contentLength >0){
                doRead(contentLength - m_ReadBuffer.size(),[this,self](){
                    std::string body;
                    std::stringstream sstream;
                    auto& request = m_RequestParser->getData();
                    auto& response = m_ResponseParser->getData();
                    body.reserve(m_ReadBuffer.size());
                    body.append(m_ReadBuffer.begin(),m_ReadBuffer.end());
                    request->setBody(body);
                    response->setVersion(request->getVersion());
                    response->setkeepAlive(request->getkeepAlive());
                    m_CallBack(request,response);
                    sstream << response;
                    const auto& buf = sstream.str();
                    m_WriteBuffer.assign(buf.begin(),buf.end());
                    handleResponse();
                });
            }else{
                    std::stringstream sstream;
                    auto& request = m_RequestParser->getData();
                    auto& response = m_ResponseParser->getData();
                    response->setVersion(request->getVersion());
                    response->setkeepAlive(request->getkeepAlive());
                    m_CallBack(request,response);
                    sstream << response;
                    const auto& buf = sstream.str();
                    m_WriteBuffer.assign(buf.begin(),buf.end());
                    handleResponse();
            }
        }
        void handleHeader(){
            auto self = this->shared_from_this();
            doRead([this,self](){
                size_t nparse = m_RequestParser->execute(m_ReadBuffer.data(),m_ReadBuffer.size());
                if(m_RequestParser->hasError()) {
                    MAGIC_LOG(LogLevel::LogDebug) << "HttpParser hasError";
                    return;
                }
                uint64_t m_Offset = m_ReadBuffer.size() - nparse;
                m_ReadBuffer.resize(m_Offset);
                if(!m_RequestParser->isFinished()) {
                    handleHeader();
                    return;
                }
                handleBody();
            });
        }
        void handleResponse(){
            auto self = this->shared_from_this();
            doWrite([this,self](){
                handleHeader();
            });
        }
    private:
        HttpHandle m_CallBack;
        Safe<HttpRequestParser> m_RequestParser;
        Safe<HttpResponseParser> m_ResponseParser;
    };
}
}