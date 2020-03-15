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
                MAGIC_LOG(LogLevel::LogDebug) << "accept one";
                this->handleFunc(socket);
                if(m_IsRun){
                    accept();
                }
            });
        }
        void WebSocketServer::handleFunc(const Share<Socket>& socket){
            auto webSocket = std::static_pointer_cast<WebSocket>(socket);
            webSocket->accept();
        }
}
}