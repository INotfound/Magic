/*
 * @Author: INotFound
 * @Date: 2020-03-12 20:41:54
 * @LastEditTime: 2021-02-01 22:29:25
 */
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/Crypto.h"
#include "Magic/NetWork/Http/HttpSocket.h"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpSocket::~HttpSocket() = default;

    HttpSocket::HttpSocket(const Safe<Socket>& socket)
        :m_Socket(socket)
        ,m_TotalLength(0)
        ,m_CurrentLength(0)
        ,m_Death(false)
        ,m_StreamBufferSize(1048576)
        ,m_TotalTransferLength(0)
        ,m_CurrentTransferLength(0){
        m_MultiPart = std::make_shared<MultiPart>();
        m_RequestParser = std::make_shared<HttpRequestParser>();
        m_ResponseParser = std::make_shared<HttpResponseParser>();

        m_Socket->setErrorCodeCallBack([this](const asio::error_code& err){
            m_Death = true;
        #ifdef WIN32
            if(err.value() == WSAECONNABORTED) return;
        #endif
            if(err == asio::error::eof || err == asio::error::operation_aborted)
                return;
            MAGIC_WARN() << err.message();
        });
    }

    void HttpSocket::runHeartBeat(){
        m_Socket->setHeartBeatCallBack([this](const Safe<Socket>& socket){
            if(m_WebSocket){ /// WebSocket
                socket->runHeartBeat(m_WebSocket);
                return;
            }else if(m_Death && !socket->isWorking()){
                socket->close();
                return;
            }
            m_Death = true;
            socket->runHeartBeat(this->shared_from_this());
        });
        m_Socket->runHeartBeat(this->shared_from_this());
    }

    void HttpSocket::setDirectory(const std::string& dirPath){
        m_MultiPart->setDirectory(dirPath);
    }

    const Safe<HttpRequest>& HttpSocket::getRequest() const{
        return m_RequestParser->getData();
    }

    const Safe<HttpResponse>& HttpSocket::getResponse() const{
        return m_ResponseParser->getData();
    }

    void HttpSocket::recvRequest(const HttpRecvBack& callback){
        m_RecvRequestCallBack = std::move(callback);
        this->requestParser();
    }

    void HttpSocket::recvResponse(const HttpRecvBack& callback){
        m_RecvResponseCallBack = std::move(callback);
        this->responseParser();
    }

    void HttpSocket::sendRequest(const Safe<HttpRequest>& httpRequest){
        if(!m_Socket)
            return;
        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
        std::ostream stream(streamBuffer.get());
        stream << httpRequest;
        m_Socket->send(streamBuffer);
    }

    void HttpSocket::sendResponse(const Safe<HttpResponse>& httpResponse){
        if(!m_Socket)
            return;
        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
        std::ostream stream(streamBuffer.get());
        if(httpResponse->hasResource()){
            const std::string& filePath = httpResponse->getResource();
            m_FileStream.open(filePath,std::ios::in | std::ios::binary);
            if(m_FileStream.is_open()){
                m_CurrentTransferLength = 0;
                m_FileStream.seekg(0,std::ios::end);
                uint64_t totalLength = m_FileStream.tellg();
                httpResponse->setContentType(Http::FileTypeToHttpContentType(filePath));
                m_StreamBuffer.reset(new char[m_StreamBufferSize],std::default_delete<char[]>());
                if(httpResponse->isRange()){
                    auto rangeStop = httpResponse->getRangeStop();
                    auto rangeStart = httpResponse->getRangeStart();
                    m_FileStream.seekg(rangeStart,std::ios::beg);
                    httpResponse->setStatus(HttpStatus::PARTIAL_CONTENT);
                    if(rangeStop == 0){
                        m_TotalTransferLength = (totalLength - rangeStart);
                    }else{
                        m_TotalTransferLength = (rangeStop - rangeStart) + 1;
                    }
                    httpResponse->setContentLength(m_TotalTransferLength);
                    httpResponse->setRange(rangeStart,rangeStart + m_TotalTransferLength - 1,totalLength);
                    /// Send Response
                    stream << httpResponse;
                    m_Socket->send(streamBuffer);
                    this->transferFileStream();
                }else{
                    m_FileStream.seekg(0,std::ios::beg);
                    m_TotalTransferLength = totalLength;
                    httpResponse->setStatus(HttpStatus::OK);
                    httpResponse->setContentLength(m_TotalTransferLength);
                    if(httpResponse->getContentType() < HttpContentType::APPLICATION_OCTET_STREAM){
                        httpResponse->setBody(std::string(std::istreambuf_iterator<char>(m_FileStream),std::istreambuf_iterator<char>()));
                        stream << httpResponse;
                        m_FileStream.close();
                        m_Socket->send(streamBuffer);
                    }else{
                        httpResponse->setHeader("Accept-Ranges","bytes");
                        stream << httpResponse;
                        m_Socket->send(streamBuffer);
                        this->transferFileStream();
                    }
                }
            }else{
                httpResponse->setStatus(HttpStatus::NOT_FOUND);
                stream << httpResponse;
                m_Socket->send(streamBuffer);
            }
        }else{
            stream << httpResponse;
            m_Socket->send(streamBuffer);
        }
    }

    const Safe<WebSocket>& HttpSocket::upgradeWebSocket(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response,bool mask){
        if(mask){
            request->setHeader("Upgrade","websocket")
                   ->setHeader("Connection","Upgrade")
                   ->setHeader("Sec-WebSocket-Version","13")
                   ->setHeader("Sec-WebSocket-Key","SU5vdEZvdW5kCg==");
            this->sendRequest(request);
        }else{
            std::string wsKey = request->getHeader("Sec-WebSocket-Key");
            if(wsKey.empty()){
                throw Failure("Upgrade WebSocket Failed: Sec-WebSocket-Key Missing Parameters!");
            }
            wsKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
            response->setHeader("Upgrade","websocket")
                    ->setHeader("Connection","Upgrade")
                    ->setStatus(HttpStatus::SWITCHING_PROTOCOLS)
                    ->setHeader("Sec-WebSocket-Accept",Base64Encode(SHA1(wsKey)));
            this->sendResponse(response);
        }
        Safe<Socket> socket = std::move(m_Socket);
        m_WebSocket = std::make_shared<WebSocket>(mask,socket);
        m_WebSocket->runAnalyse();
        return m_WebSocket;
    }

    void HttpSocket::handleRequest(){
        auto& params = m_MultiPart->getParamMap();
        auto& request = m_RequestParser->getData();
        auto& response = m_ResponseParser->getData();
        for(auto& v :params){
            request->setParam(v.first,v.second);
        }
        if(request->isRange()){
            response->setRange(request->getRangeStart(),request->getRangeStop(),0);
        }
        response->setVersion(request->getVersion());
        response->setKeepAlive(request->getKeepAlive());
        response->setHeader("Content-Encoding",request->getHeader("Accept-Encoding"));

        if(m_RecvRequestCallBack)
            m_RecvRequestCallBack(this->shared_from_this());

        m_MultiPart->reset();
        m_RequestParser->reset();
        m_ResponseParser->reset();
        this->requestParser();
    }

    void HttpSocket::handleResponse(){
        if(m_RecvResponseCallBack)
            m_RecvResponseCallBack(this->shared_from_this());

        m_MultiPart->reset();
        m_RequestParser->reset();
        m_ResponseParser->reset();
    }

    void HttpSocket::requestParser(){
        if(!m_Socket)
            return;
        auto self = this->shared_from_this();
        m_Socket->recv([this,self](Socket::StreamBuffer& data){
            m_Death = false;
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
            m_CurrentLength = 0;
            auto& request = m_RequestParser->getData();
            m_TotalLength = m_RequestParser->getContentLength();
            const std::string& value = request->getHeader("Content-Type");
            auto pos = value.find("boundary=");
            if(pos != std::string::npos){
                pos += 9;
                m_MultiPart->setBoundary(value.substr(pos,value.length() - pos));
                this->multiPartParser();
            }else{
                /// Max Request Size == 5MB
                constexpr uint32_t maxRequestSize = 1024 * 1024 * 5;
                if(m_TotalLength > 0 && m_TotalLength < maxRequestSize){
                    m_Socket->recv(m_TotalLength - data.size(),[this,self](Socket::StreamBuffer& data){
                        std::string body;
                        auto& request = m_RequestParser->getData();
                        body.reserve(data.size());
                        body.append(data.begin(),data.end());
                        request->setBody(body);
                        data.clear();
                        this->handleRequest();
                    });
                }else{
                    data.clear();
                    this->handleRequest();
                }
            }
        });
    }

    void HttpSocket::responseParser(){
        if(!m_Socket)
            return;
        auto self = this->shared_from_this();
        m_Socket->recv([this,self](Socket::StreamBuffer& data){
            m_Death = false;
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
                m_Socket->recv(contentLength - data.size(),[this,self](Socket::StreamBuffer& data){
                    std::string body;
                    auto& response = m_ResponseParser->getData();
                    body.reserve(data.size());
                    body.append(data.begin(),data.end());
                    response->setBody(body);
                    data.clear();
                    this->handleResponse();
                });
            }else{
                data.clear();
                this->handleResponse();
            }
        });
    }

    void HttpSocket::multiPartParser(){
        if(!m_Socket)
            return;
        auto self = this->shared_from_this();
        m_Socket->recv([this,self](Socket::StreamBuffer& data){
            size_t fed = 0;
            do{
                size_t ret = m_MultiPart->feed(data.data() + fed,data.size() - fed);
                fed += ret;
            }while(fed < data.size() && !m_MultiPart->stopped());
            m_CurrentLength += data.size();
            data.resize(0);
            if(m_CurrentLength == m_TotalLength){
                data.clear();
                this->handleRequest();
            }else{
                this->multiPartParser();
            }
        });
    }

    void HttpSocket::transferFileStream(){
        if(!m_Socket
            || m_FileStream.eof()
            || m_CurrentTransferLength == m_TotalTransferLength){
            m_FileStream.close();
            m_StreamBuffer.reset();
            m_ResponseParser->reset();
            m_TotalTransferLength = 0;
            m_CurrentTransferLength = 0;
            return;
        }
        uint64_t transferLength = m_TotalTransferLength > m_StreamBufferSize ? m_StreamBufferSize : m_TotalTransferLength;
        m_FileStream.read(m_StreamBuffer.get(),transferLength);
        auto length = m_FileStream.gcount();
        m_CurrentTransferLength += length;
        auto self = this->shared_from_this();
        m_Socket->send(m_StreamBuffer.get(),length,[this,self](){
            this->transferFileStream();
        });
    }
}
}
}
