#include <iostream>
#include "Macro.h"
#include "Crypto.h"
#include "Http/HttpParser.h"
#include "WebSocket/WebSocketFrame.h"
#include "WebSocket/WebSocketServer.h"
namespace Magic{
namespace WebSocket{
        WebSocketServer::WebSocketServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs)
            :TcpServer(addr,port,threadCount,timeOutMs){
        }
        void WebSocketServer::WebSocketServer::accept(){
            // Share<Socket> socket = std::make_shared<Socket>(4*1024,m_TimeOutMs,m_IoPool->get());
            // m_Acceptor->async_accept(*socket->getEntity(),[this,socket](const asio::error_code& err){
            //     if(err){
            //         //TODO: ...
            //         MAGIC_LOG(LogLevel::LogWarn) << err.message();
            //         return;
            //     }
            //     MAGIC_LOG(LogLevel::LogDebug) << "accept one";
            //     this->handleFunc(socket);
            //     if(m_IsRun){
            //         accept();
            //     }
            // });
        }
        void WebSocketServer::handleFunc(const Share<Socket>& socket){
            // handleShake(socket);
        }
        void WebSocketServer::handleShake(const Share<Socket>& socket){
            // auto readStreamBuffer = std::make_shared<asio::streambuf>();
            // asio::async_read_until(*socket->getEntity()
            //     ,*readStreamBuffer
            //     ,"\r\n\r\n"
            //     ,[this,socket,readStreamBuffer](const asio::error_code &err, std::size_t length){
            //     if(err){
            //         //TODO: ...
            //         if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
            //             return;
            //         }
            //         MAGIC_LOG(LogLevel::LogWarn) << err.message();
            //         return;
            //     }
            //     uint32_t bufferSize = 1024*4;
            //     Share<char> buffer(new char[bufferSize],[](char* ptr){delete[] ptr;});
            //     Share<Http::HttpRequestParser> requestParser = std::make_shared<Http::HttpRequestParser>();
            //     length = readStreamBuffer->size();
            //     readStreamBuffer->sgetn(buffer.get(),length);
            //     requestParser->execute(buffer.get(),length);
            //     do{
            //         if(requestParser->hasError()){
            //             return;
            //         }
            //         if(requestParser->isFinished()){
            //             break;
            //         }
            //     }while(true);
            //     auto& request = requestParser->getData();
            //     if(StringCompareNoCase(request->getHeader("Upgrade"), "WebSocket")) {
            //         return;
            //     }
            //     if(StringCompareNoCase(request->getHeader("Connection"), "Upgrade")) {
            //         return;
            //     }
            //     if(StringAs<uint32_t>(request->getHeader("Sec-webSocket-Version")) != 13) {
            //         return;
            //     }
            //     std::string key = request->getHeader("Sec-WebSocket-Key");
            //     if(key.empty()) {
            //         return;
            //     }

            //     Safe<Http::HttpResponse> response(new Http::HttpResponse);
            //     std::string val = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
            //     val = Base64Encode(SHA1(val));
            //     response->setGzip(false);
            //     response->setWebSocket(true);
            //     response->setHeader("Upgrade", "websocket");
            //     response->setHeader("Connection", "Upgrade");
            //     response->setHeader("Sec-WebSocket-Accept", val);
            //     response->setStatus(Http::HttpStatus::SWITCHING_PROTOCOLS);
            //     auto writeStreamBuffer = std::make_shared<asio::streambuf>();
            //     std::ostream responseStream(writeStreamBuffer.get());
            //     responseStream << response;
            //     asio::async_write(*socket->getEntity()
            //         ,*writeStreamBuffer
            //         ,asio::transfer_exactly(writeStreamBuffer->size())
            //         ,[this,socket,writeStreamBuffer](const asio::error_code &err, std::size_t length){
            //             if(err){
            //                 //TODO: ...
            //                 if(err == asio::error::eof || err == asio::error::connection_reset){
            //                     return;
            //                 }
            //                 MAGIC_LOG(LogLevel::LogWarn) << err.message();
            //                 return;
            //             }
            //             handleWSFrame(socket);
            //     });
            // });
        }
        void WebSocketServer::handleWSFrame(const Share<Socket>& socket){
            // auto readStreamBuffer = std::make_shared<asio::streambuf>();
            // asio::async_read(*socket->getEntity()
            //     ,*readStreamBuffer
            //     ,asio::transfer_exactly(sizeof(WSFrame))
            //     ,[this,socket,readStreamBuffer](const asio::error_code &err, std::size_t length){
            //     if(err){
            //         //TODO: ...
            //         if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
            //             return;
            //         }
            //         MAGIC_LOG(LogLevel::LogWarn) << err.message();
            //         return;
            //     }
            //     WSFrame frame;
            //     readStreamBuffer->sgetn(reinterpret_cast<char*>(&frame),length);
            //     std::cout <<"fin: " << frame.m_Fin << " mask: " << frame.m_Mask << std::endl;
            //     //规定client -> server 必须要有Mask , server -> client 并不一定要Mask
            //     if(!frame.m_Mask){
            //         MAGIC_LOG(LogLevel::LogWarn) << "WebSocket frame not mask";
            //         return;
            //     }
            //     if(frame.m_Code == OPCODE::PING){
            //         MAGIC_LOG(LogLevel::LogWarn) << "WebSocket frame to ping";
            //     }
            //     if(frame.m_Code == OPCODE::BIN_FRAME || frame.m_Code == OPCODE::TEXT_FREAME
            //         || frame.m_Code == OPCODE::CONTINUE){
            //         uint64_t length = 0;
            //         bool isContinue = true;
            //         if(frame.m_Fin){
            //             isContinue = false;
            //         }
            //         if(frame.m_Payload == 126){
            //             uint16_t len = 0;
            //             asio::read(*socket->getEntity(),*readStreamBuffer,asio::transfer_exactly(sizeof(uint16_t)));
            //             readStreamBuffer->sgetn(reinterpret_cast<char*>(&len),sizeof(uint16_t));
            //             length = len;
            //             handleWSFrameData(socket,isContinue,length);
            //         }else if(frame.m_Payload == 127){
            //             uint64_t len = 0;
            //             asio::read(*socket->getEntity(),*readStreamBuffer,asio::transfer_exactly(sizeof(uint64_t)));
            //             readStreamBuffer->sgetn(reinterpret_cast<char*>(&len),sizeof(uint64_t));
            //             length = len;
            //             handleWSFrameData(socket,isContinue,length);
            //         }else{
            //             length = frame.m_Payload;
            //             handleWSFrameData(socket,isContinue,length);
            //         }
            //     }
            // });
        }
        void WebSocketServer::handleWSFrameData(const Share<Socket>& socket,bool isContinue,uint64_t length){
            // char mask[4] = {0};
            // asio::async_read(*socket->getEntity()
            //     ,asio::buffer(mask)
            //     ,[this,socket,isContinue,length,mask](const asio::error_code &err, std::size_t){
            //    if(err){
            //         //TODO: ...
            //         if(err == asio::error::eof || err == asio::error::connection_reset || err == asio::error::operation_aborted){
            //             return;
            //         }
            //         MAGIC_LOG(LogLevel::LogWarn) << err.message();
            //         return;
            //     }
            //     auto readStreamBuffer = std::make_shared<asio::streambuf>();
            //     asio::async_read(*socket->getEntity()
            //         ,*readStreamBuffer
            //         ,asio::transfer_exactly(length)
            //         ,[this,socket,isContinue,mask,readStreamBuffer](const asio::error_code &err, std::size_t length){
            //         auto& data = socket->getData();
            //         uint64_t offset = data.size();
            //         data.reserve(offset + length);
            //         readStreamBuffer->sgetn(&data[offset],length);
            //         for(uint32_t i = 0; i < length; ++i) {
            //             data[offset + i] ^= mask[i % 4];
            //         }
            //         if(!isContinue){
            //             std::cout << data.data() << std::endl;
            //             data.clear();
            //         }
            //         handleWSFrame(socket);
            //     });
            // });
        }
}
}