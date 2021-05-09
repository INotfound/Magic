/*
 * @Author: INotFound
 * @Date: 2020-03-12 20:41:54
 * @LastEditTime: 2021-02-01 22:29:25
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Http/HttpSocket.h"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpSocket::HttpSocket(uint64_t timeOutMs,asio::io_context& context)
        :Socket(timeOutMs,4096,context)
        ,m_TotalLength(0)
        ,m_CurrentLength(0)
        ,m_StreamBufferSize(1024*1024*2)
        ,m_TotalTransferLength(0)
        ,m_CurrentTransferLength(0){
        m_RequestParser = std::make_shared<HttpRequestParser>();
        m_ResponseParser = std::make_shared<HttpResponseParser>();
        this->setErrorCodeCallBack([this](const asio::error_code & err){
            m_MultiPart.reset();
            m_FileStream.close();
            m_StreamBuffer.reset();
            m_RequestParser->reset();
            m_ResponseParser->reset();
            m_TotalTransferLength = 0;
            m_CurrentTransferLength = 0;
            MAGIC_WARN() << err.message();
        });
    }

    void HttpSocket::recvRequest(const HttpRecvBack& callback){
        m_RecvRequestCallBack = callback;
        this->requestParser();
    }

    void HttpSocket::recvResponse(const HttpRecvBack& callback){
        m_RecvResponseCallBack = callback;
        this->responseParser();
    }

    void HttpSocket::handleRequest(){
        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
        std::ostream stream(streamBuffer.get());
        auto& request = m_RequestParser->getData();
        auto& response = m_ResponseParser->getData();
        response->setVersion(request->getVersion());
        response->setKeepAlive(request->getKeepAlive());
        m_RecvRequestCallBack(request,response);

        if(response->hasResource()){
            std::string filePath = response->getResource();
            response->setContentType(Http::FileTypeToHttpContentType(filePath));
            m_FileStream.open(filePath,std::ios::in|std::ios::binary);
            if(m_FileStream.is_open()){
                m_CurrentTransferLength = 0;
                m_FileStream.seekg(0,std::ios::end);
                m_TotalTransferLength = m_FileStream.tellg();
                m_StreamBuffer.reset(new char[m_StreamBufferSize], [](char *pointer) {delete[] pointer;});
                if(request->isRange()){
                    auto rangeStop = request->getRangeStop();
                    auto rangeStart = request->getRangeStart();
                    m_FileStream.seekg(rangeStart,std::ios::beg);
                    response->setStatus(HttpStatus::PARTIAL_CONTENT);
                    if(rangeStop == 0){
                        uint64_t length = (m_TotalTransferLength - rangeStart);
                        response->setContentLength(length);
                        response->setRange(rangeStart,m_TotalTransferLength - 1,m_TotalTransferLength);
                        m_TotalTransferLength = length;
                    }else{
                        uint64_t length = (rangeStop - rangeStart) + 1;
                        response->setContentLength(length);
                        response->setRange(rangeStart,rangeStop,m_TotalTransferLength);
                        m_TotalTransferLength = length;
                    }
                    stream << response;
                    this->send(streamBuffer);
                    this->transferFileStream();
                }else{
                    m_FileStream.seekg(0,std::ios::beg);
                    response->setStatus(HttpStatus::OK);
                    response->setContentLength(m_TotalTransferLength);
                    response->setHeader("Accept-Ranges","bytes");
                    stream << response;
                    this->send(streamBuffer);
                    this->transferFileStream();
                }
            }else{
                response->setStatus(HttpStatus::NOT_FOUND);
                stream << response;
                this->send(streamBuffer);
            }
        }else{
            stream << response;
            this->send(streamBuffer);
        }

        m_MultiPart.reset();
        m_RequestParser->reset();
        this->requestParser();
    }

    void HttpSocket::handleResponse(){
        auto& request = m_RequestParser->getData();
        auto& response = m_ResponseParser->getData();
        m_RecvResponseCallBack(request,response);
        m_ResponseParser->reset();
        this->responseParser();
    }

    void HttpSocket::requestParser(){
        this->recv([this](const Safe<Socket>& conn,StreamBuffer& data){
            uint64_t nparse = m_RequestParser->execute(data.data(),data.size());
            if(m_RequestParser->hasError()){
                return;
            }
            uint64_t m_Offset = data.size() - nparse;
            data.resize(m_Offset);
            if(!m_RequestParser->isFinished()){
                this->requestParser();
                return;
            }
            auto& request = m_RequestParser->getData();

            m_CurrentLength = 0;
            m_TotalLength = m_RequestParser->getContentLength();
            const std::string& value = request->getHeader("Content-Type");
            auto pos = value.find("boundary=");
            if(pos != std::string::npos){
                pos += 9;
                m_MultiPart.setBoundary(value.substr(pos,value.length() - pos));
                this->multiPartParser();
            }else{
                if(m_TotalLength > 0 && m_TotalLength < 1024*1024*5){
                    this->recv(m_TotalLength - data.size(),[this](const Safe<Socket>& conn,StreamBuffer& data){
                        std::string body;
                        auto& request = m_RequestParser->getData();
                        body.reserve(data.size());
                        body.append(data.begin(),data.end());
                        request->setBody(body);
                        this->handleRequest();
                        data.clear();
                    });
                }else{
                    this->handleRequest();
                    data.clear();
                }
            }
        });
    }

    void HttpSocket::responseParser(){
        this->recv([this](const Safe<Socket>& conn,StreamBuffer& data){
            uint64_t nparse = m_ResponseParser->execute(data.data(),data.size());
            if(m_ResponseParser->hasError()){
                return;
            }
            uint64_t m_Offset = data.size() - nparse;
            data.resize(m_Offset);
            if(!m_ResponseParser->isFinished()){
                this->responseParser();
                return;
            }
            uint64_t contentLength = m_ResponseParser->getContentLength();
            if(contentLength > 0){
                this->recv(contentLength - data.size(),[this](const Safe<Socket>& conn,StreamBuffer& data){
                    std::string body;
                    auto& response = m_ResponseParser->getData();
                    body.reserve(data.size());
                    body.append(data.begin(),data.end());
                    response->setBody(body);
                    this->handleResponse();
                    data.clear();
                });
            }else{
                this->handleResponse();
                data.clear();
            }
        });
    }

    void HttpSocket::setTempDirectory(const std::string& dirPath) {
        m_TempDirectory = dirPath;
    }

    void HttpSocket::multiPartParser() {
        this->recv([this](const Safe<Socket>& conn,StreamBuffer& data){
            size_t fed = 0;
            do {
                size_t ret = m_MultiPart.feed(data.data() + fed,data.size() - fed);
                fed += ret;
            }while(fed < data.size() && !m_MultiPart.stopped());
            m_CurrentLength += data.size();
            data.resize(0);
            if(m_CurrentLength == m_TotalLength){
                this->handleRequest();
                data.clear();
            }else{
                this->multiPartParser();
            }
        });
    }

    void HttpSocket::transferFileStream(){
        if(m_FileStream.eof() || m_CurrentTransferLength == m_TotalTransferLength){
            m_FileStream.close();
            m_StreamBuffer.reset();
            m_ResponseParser->reset();
            m_TotalTransferLength = 0;
            m_CurrentTransferLength = 0;
            return;
        }
        m_FileStream.read(m_StreamBuffer.get(),m_StreamBufferSize);
        auto length = m_FileStream.gcount();
        m_CurrentTransferLength += length;
        this->send(m_StreamBuffer.get(),length,[this](){
            this->transferFileStream();
        });
    }
}
}
}
