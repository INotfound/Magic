#include "Http/HttpSocket.h"

namespace Magic{
namespace Http{
    HttpSocket::HttpSocket(uint64_t timeOutMs,asio::io_context& context)
        :Socket(timeOutMs,context){
        m_RequestParser.reset(new HttpRequestParser);
        m_ResponseParser.reset(new HttpResponseParser);
    }
    void HttpSocket::Go(HttpHandle callBack){
        m_CallBack = std::move(callBack);
        this->handle();
    }
    void HttpSocket::handle(){
        this->handleHeader();
    }
    void HttpSocket::handleBody(){
        uint64_t contentLength = m_RequestParser->getContentLength();
        if(contentLength >0){
            doRead(contentLength - m_ReadBuffer.size(),[this](const std::error_code& ec){
                if(ec){
                    if(ec == asio::error::eof || ec == asio::error::connection_reset || ec == asio::error::operation_aborted){
                        return;
                    }
                    MAGIC_LOG(LogLevel::LogWarn) << ec.message();
                    return;
                }
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
                this->handleResponse();
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
                this->handleResponse();
        }
    }
    void HttpSocket::handleHeader(){
        doRead([this](const std::error_code& ec){
            if(ec){
                if(ec == asio::error::eof || ec == asio::error::connection_reset || ec == asio::error::operation_aborted){
                    return;
                }
                MAGIC_LOG(LogLevel::LogWarn) << ec.message();
                return;
            }

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
            this->handleBody();
        });
    }
    void HttpSocket::handleResponse(){
        doWrite([this](const std::error_code& ec){
            if(ec){
                if(ec == asio::error::eof || ec == asio::error::connection_reset || ec == asio::error::operation_aborted){
                    return;
                }
                MAGIC_LOG(LogLevel::LogWarn) << ec.message();
                return;
            }
            this->handleHeader();
        });
    }
}
}