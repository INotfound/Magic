/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpSocket.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Logger.hpp"
#include "Magic/Utilty/Crypto.hpp"
#include "Magic/Core/StringView.hpp"
#include "Magic/NetWork/Http/HttpSocket.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpSocket::~HttpSocket() = default;

    HttpSocket::HttpSocket(const Safe<Socket>& socket)
        :m_Socket(socket)
        ,m_TotalLength(0)
        ,m_CurrentLength(0)
        ,m_Death(false)
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

    const Safe<MultiPart>& HttpSocket::getMultiPart() const{
        return m_MultiPart;
    }

    const Safe<HttpRequest>& HttpSocket::getRequest() const{
        return m_RequestParser->getData();
    }

    const Safe<HttpResponse>& HttpSocket::getResponse() const{
        return m_ResponseParser->getData();
    }

    void HttpSocket::recvRequest(HttpRecvBack callBack){
        m_RecvRequestCallBack = std::move(callBack);
        this->requestParser();
    }

    void HttpSocket::recvResponse(HttpRecvBack callBack){
        m_RecvResponseCallBack = std::move(callBack);
        this->responseParser();
    }

    void HttpSocket::setDirectory(const StringView& dirPath){
        m_MultiPart->setDirectory(dirPath);
    }

    void HttpSocket::sendRequest(HttpRequest& httpRequest){
        if(!m_Socket)
            return;
        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
        std::ostream stream(streamBuffer.get());
        stream << httpRequest;
        m_Socket->send(streamBuffer);
    }

    void HttpSocket::sendResponse(HttpResponse& httpResponse){
        if(!m_Socket)
            return;
        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
        std::ostream stream(streamBuffer.get());
        m_CurrentTransferLength = 0;
        if(httpResponse.hasResource()){
            StringView filePath = httpResponse.getResource();
            Safe<FileStream> fileStream = std::make_shared<FileStream>(filePath);
            if(IsFile(filePath) && fileStream->open(FileStream::OpenMode::Read)){
                uint64_t totalLength = fileStream->size();
                httpResponse.setContentType(Http::FileTypeToHttpContentType(filePath));
                if(httpResponse.isRange()){
                    auto rangeStop = httpResponse.getRangeStop();
                    auto rangeStart = httpResponse.getRangeStart();
                    fileStream->seek(rangeStart);
                    httpResponse.setStatus(HttpStatus::PartialContent);
                    if(rangeStop == 0){
                        m_TotalTransferLength = (totalLength - rangeStart);
                    }else{
                        m_TotalTransferLength = (rangeStop - rangeStart) + 1;
                    }
                    httpResponse.setContentLength(m_TotalTransferLength);
                    httpResponse.setRange(rangeStart,rangeStart + m_TotalTransferLength - 1,totalLength);
                }else{
                    m_TotalTransferLength = totalLength;
                    httpResponse.setStatus(HttpStatus::Ok);
                    httpResponse.setHeader("Accept-Ranges","bytes");
                    httpResponse.setContentLength(m_TotalTransferLength);
                }
                if(httpResponse.getContentType() < HttpContentType::ApplicationOctetStream){
                    httpResponse.setBody(fileStream);
                }else{
                    m_Stream = fileStream;
                }
            }else{
                httpResponse.setResource(StringView());
                httpResponse.setStatus(HttpStatus::NotFound);
            }
        }else{
            if(!httpResponse.hasBody() && httpResponse.getStatus() > HttpStatus::Ok){
                auto statusString = HttpStatusToString(httpResponse.getStatus());
                httpResponse.setBody(GenerateHtml(statusString,statusString,""));
            }
        }

        if(m_Stream){
            stream << httpResponse;
            m_Socket->send(streamBuffer);
            this->transferDataStream();
        }else{
            stream << httpResponse;
            m_Socket->send(streamBuffer);
        }

    }

    void HttpSocket::sendRequest(const Safe<HttpRequest>& httpRequest){
        this->sendRequest(*httpRequest);
    }

    void HttpSocket::sendResponse(const Safe<HttpResponse>& httpResponse){
        this->sendResponse(*httpResponse);
    }

    const Safe<WebSocket>& HttpSocket::upgradeWebSocket(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response,bool mask){
        if(mask){
            request->setHeader("Upgrade","websocket")
                   ->setHeader("Connection","Upgrade")
                   ->setHeader("Sec-WebSocket-Version","13")
                   ->setHeader("Sec-WebSocket-Key","SU5vdEZvdW5kCg==");
            this->sendRequest(request);
        }else{
            StringView wsKey = request->getHeader("Sec-WebSocket-Key");
            if(wsKey.empty()){
                throw Failure("Upgrade WebSocket Failed: Sec-WebSocket-Key Missing Parameters");
            }
            auto base64Encoder = std::make_shared<Base64Encoder>(std::make_shared<MessageDigest>(MessageDigest::Algorithm::SHA1
                                                                 ,std::make_shared<DataStream>(StringCat(wsKey,"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"))));
            response->setHeader("Upgrade","websocket")
                    ->setHeader("Connection","Upgrade")
                    ->setStatus(HttpStatus::SwitchingProtocols)
                    ->setHeader("Sec-WebSocket-Accept",base64Encoder->read());
            this->sendResponse(response);
        }
        Safe<Socket> socket = std::move(m_Socket);
        m_WebSocket = std::make_shared<WebSocket>(mask,socket);
        m_WebSocket->runAnalyse();
        return m_WebSocket;
    }

    void HttpSocket::handleRequest(){
        auto& request = m_RequestParser->getData();
        auto& response = m_ResponseParser->getData();
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
    #if __cplusplus >= 201402L
        m_Socket->recv([this,self = std::move(self)](DataStream& dataStream){
    #else
        m_Socket->recv([this,self](DataStream& dataStream){
    #endif
            m_Death = false;
            uint64_t nparse = m_RequestParser->execute(dataStream.data(),dataStream.size());
            if(m_RequestParser->hasError()){
                return;
            }
            uint64_t m_Offset = dataStream.size() - nparse;
            dataStream.resize(m_Offset);
            if(!m_RequestParser->isFinished()){
                this->requestParser();
                return;
            }
            m_CurrentLength = 0;
            auto& request = m_RequestParser->getData();
            m_TotalLength = m_RequestParser->getContentLength();
            StringView value = request->getHeader("Content-Type");
            auto pos = value.find("boundary=");
            if(pos != std::string::npos){
                pos += 9;
                m_MultiPart->setBoundary(value.substr(pos,value.length() - pos));
                this->multiPartParser();
            }else{
                /// Max Request Size == 5MB
                constexpr uint32_t maxRequestSize = 1024 * 1024 * 5;
                if(m_TotalLength > 0 && m_TotalLength < maxRequestSize){
                #if __cplusplus >= 201402L
                    m_Socket->recv(m_TotalLength - dataStream.size(),[this,self = std::move(self)](DataStream& dataStream){
                #else
                    m_Socket->recv(m_TotalLength - dataStream.size(),[this,self](DataStream& dataStream){
                #endif
                        auto& request = m_RequestParser->getData();
                        request->setBody(dataStream.read());
                        dataStream.resize(0);
                        this->handleRequest();
                    });
                }else{
                    dataStream.resize(0);
                    this->handleRequest();
                }
            }
        });
    }

    void HttpSocket::responseParser(){
        if(!m_Socket)
            return;
        auto self = this->shared_from_this();
    #if __cplusplus >= 201402L
        m_Socket->recv([this,self = std::move(self)](DataStream& dataStream){
    #else
        m_Socket->recv([this,self](DataStream& dataStream){
    #endif
            m_Death = false;
            uint64_t nparse = m_ResponseParser->execute(dataStream.data(),dataStream.size());
            if(m_ResponseParser->hasError()){
                return;
            }
            uint64_t m_Offset = dataStream.size() - nparse;
            dataStream.resize(m_Offset);
            if(!m_ResponseParser->isFinished()){
                this->responseParser();
                return;
            }
            uint64_t contentLength = m_ResponseParser->getContentLength();
            if(contentLength > 0){
            #if __cplusplus >= 201402L
                m_Socket->recv(contentLength - dataStream.size(),[this,self = std::move(self)](DataStream& dataStream){
            #else
                m_Socket->recv(contentLength - dataStream.size(),[this,self](DataStream& dataStream){
            #endif
                    auto& response = m_ResponseParser->getData();
                    response->setBody(dataStream.read());
                    dataStream.resize(0);
                    this->handleResponse();
                });
            }else{
                dataStream.resize(0);
                this->handleResponse();
            }
        });
    }

    void HttpSocket::multiPartParser(){
        if(!m_Socket)
            return;
        auto self = this->shared_from_this();
    #if __cplusplus >= 201402L
        m_Socket->recv([this,self = std::move(self)](DataStream& dataStream){
    #else
        m_Socket->recv([this,self](DataStream& dataStream){
    #endif
            size_t fed = 0;
            do{
                size_t ret = m_MultiPart->feed(IStream::BufferView(dataStream.data() + fed,dataStream.size() - fed));
                fed += ret;
            }while(fed < dataStream.size() && !m_MultiPart->stopped());
            m_CurrentLength += dataStream.size();
            dataStream.resize(0);
            if(m_CurrentLength == m_TotalLength){
                this->handleRequest();
            }else{
                this->multiPartParser();
            }
        });
    }

    void HttpSocket::transferDataStream(){
        if(!m_Socket
            || !m_Stream
            || m_Stream->eof()
            || m_CurrentTransferLength == m_TotalTransferLength){
            m_Stream.reset();
            m_ResponseParser->reset();
            m_TotalTransferLength = 0;
            m_CurrentTransferLength = 0;
            return;
        }
        IStream::BufferView buffer = m_Stream->read();
        m_CurrentTransferLength += buffer.size();
        auto self = this->shared_from_this();
    #if __cplusplus >= 201402L
        m_Socket->send(buffer,[this,self = std::move(self)](){
    #else
        m_Socket->send(buffer,[this,self](){
    #endif
            this->transferDataStream();
        });
    }

    std::string GenerateHtml(const StringView& status,const StringView& title,const StringView& message){
        return StringCat("<!DOCTYPE html><html lang=\"en\"><head><title>"
                         ,status
                         ,"</title></head><body><center><h1>"
                         ,title
                         ,"</h1><h3>"
                         ,message
                         ,"</h3></center><hr><center>Magic/2.0.0</center></body></html>");
    }
}
}
}
