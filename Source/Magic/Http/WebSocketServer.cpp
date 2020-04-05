/*
 * @file: 
 * @Author: INotFound
 * @Date: 2020-03-18 22:59:40
 * @LastEditTime: 2020-03-20 00:05:09
 */
#include <iostream>
#include "Macro.h"
#include "Crypto.h"
#include "Http/HttpParser.h"
#include "Http/WebSocket.h"
#include "Http/WebSocketServer.h"
namespace Magic{
namespace Http{
        WebSocketServer::WebSocketServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs)
            :TcpServer(addr,port,threadCount,timeOutMs){
        }
        void WebSocketServer::WebSocketServer::accept(){
            Share<Socket> socket = std::make_shared<WebSocket>(m_TimeOutMs,m_IoPool->get());
            m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
                if(err){
                    //TODO: ...
                    MAGIC_LOG(LogLevel::LogWarn) << err.message();
                    return;
                }
                this->handleFunc(socket);
                MAGIC_LOG(LogLevel::LogWarn) << "accept one";
                if(m_IsRun){
                    accept();
                }
            });
        }
        void WebSocketServer::handleFunc(const Share<Socket>& socket){
            socket->recv([this](const Share<Socket>& conn,Socket::StreamBuffer& data){
                Safe<HttpRequestParser> requestParser(new HttpRequestParser);
                size_t nparse = requestParser->execute(data.data(),data.size());
                if(requestParser->hasError()) {
                    MAGIC_LOG(LogLevel::LogWarn) << "HttpParser hasError";
                    return;
                }
                uint64_t m_Offset = data.size() - nparse;
                data.resize(m_Offset);
                if(!requestParser->isFinished()) {
                    this->accept();
                    return;
                }
                auto& request = requestParser->getData();
                if(StringCompareNoCase(request->getHeader("Upgrade"), "WebSocket")) {
                    return;
                }
                if(StringCompareNoCase(request->getHeader("Connection"), "Upgrade")) {
                    return;
                }
                if(StringAs<uint32_t>(request->getHeader("Sec-webSocket-Version")) != 13) {
                    return;
                }
                std::string key = request->getHeader("Sec-WebSocket-Key");
                if(key.empty()) {
                    return;
                }
                std::stringstream sstream;
                Safe<HttpResponse> response(new HttpResponse);
                std::string val = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
                val = Base64Encode(SHA1(val));
                response->setGzip(false);
                response->setWebSocket(true);
                response->setHeader("Upgrade", "websocket");
                response->setHeader("Connection", "Upgrade");
                response->setHeader("Sec-WebSocket-Accept", val);
                response->setStatus(Http::HttpStatus::SWITCHING_PROTOCOLS);
                sstream << response;
                conn->send(sstream.str());
                auto self = std::static_pointer_cast<WebSocket>(conn);
                //call onOpen() func
                data.clear();
                auto webSocket = std::static_pointer_cast<WebSocket>(conn);
                webSocket->onMessage([](const Share<WebSocket> conn,const Safe<WebSocketMessage>& msg){
                    MAGIC_LOG(LogLevel::LogDebug) << msg->getData();
                    conn->sendMessage(msg->getData().c_str(),msg->getData().size(),WebSocketMessageType::TEXT);
                });
            });

        }
}
}