/*
 * @Author: INotFound
 * @Date: 2020-03-12 20:41:54
 * @LastEditTime: 2020-10-23 19:53:28
 */
#include "Core/Logger.h"
#include "NetWork/Http/HttpSocket.h"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpSocket::HttpSocket(uint64_t timeOutMs,asio::io_context& context)
        :Socket(timeOutMs,4096,context){
        m_RequestParser = std::make_shared<HttpRequestParser>();
        m_ResponseParser = std::make_shared<HttpResponseParser>();
    }
    void HttpSocket::recvRequest(const HttpRecvBack& callback){
        m_RecvRequestCallBack = std::move(callback);
        this->requestParser();
    }
    void HttpSocket::recvResponse(const HttpRecvBack& callback){
        m_RecvResponseCallBack = std::move(callback);
        this->responseParser();
    }
    void HttpSocket::requestParser(){
        this->recv([this](const Safe<Socket>& conn,StreamBuffer& data){
            uint64_t nparse = m_RequestParser->execute(data.data(),data.size());
            if(m_RequestParser->hasError()) {
                return;
            }
            uint64_t m_Offset = data.size() - nparse;
            data.resize(m_Offset);
            if(!m_RequestParser->isFinished()) {
                this->requestParser();
                return;
            }
            uint64_t contentLength = m_RequestParser->getContentLength();
            if(contentLength >0){
                this->recv(contentLength - data.size(),[this](const Safe<Socket>& conn,StreamBuffer& data){
                    std::string body;
                    std::stringstream sstream;
                    auto& request = m_RequestParser->getData();
                    auto& response = m_ResponseParser->getData();
                    body.reserve(data.size());
                    body.append(data.begin(),data.end());
                    request->setBody(body);
                    response->setVersion(request->getVersion());
                    response->setkeepAlive(request->getkeepAlive());
                    m_RecvRequestCallBack(request,response);
                    sstream << response;
                    conn->send(sstream.str());
                    data.clear();
                    m_RequestParser->reset();
                    this->requestParser();
                });
            }else{
                    std::stringstream sstream;
                    auto& request = m_RequestParser->getData();
                    auto& response = m_ResponseParser->getData();
                    response->setVersion(request->getVersion());
                    response->setkeepAlive(request->getkeepAlive());
                    m_RecvRequestCallBack(request,response);
                    sstream << response;
                    conn->send(sstream.str());
                    data.clear();
                    m_RequestParser->reset();
                    this->requestParser();
            }
        });
    }
    void HttpSocket::responseParser(){
        this->recv([this](const Safe<Socket>& conn,StreamBuffer& data){
            uint64_t nparse = m_ResponseParser->execute(data.data(),data.size());
            if(m_ResponseParser->hasError()) {
                return;
            }
            uint64_t m_Offset = data.size() - nparse;
            data.resize(m_Offset);
            if(!m_ResponseParser->isFinished()) {
                this->responseParser();
                return;
            }
            uint64_t contentLength = m_ResponseParser->getContentLength();
            if(contentLength >0){
                this->recv(contentLength - data.size(),[this](const Safe<Socket>& conn,StreamBuffer& data){
                    std::string body;
                    std::stringstream sstream;
                    auto& request = m_RequestParser->getData();
                    auto& response = m_ResponseParser->getData();
                    body.reserve(data.size());
                    body.append(data.begin(),data.end());
                    response->setBody(body);
                    m_RecvResponseCallBack(request,response);
                    data.clear();
                    m_ResponseParser->reset();
                    this->responseParser();
                });
            }else{
                    auto& request = m_RequestParser->getData();
                    auto& response = m_ResponseParser->getData();
                    m_RecvResponseCallBack(request,response);
                    data.clear();
                    m_ResponseParser->reset();
                    this->responseParser();
            }
        });
    }
}
}
}