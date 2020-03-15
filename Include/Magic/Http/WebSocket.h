#pragma once
#include "Macro.h"
#include "Socket.h"
#include "Crypto.h"
#include "Http/Http.h"
#include "Http/HttpParser.h"
#include "WebSocketMessage.h"
#include "WebSocketServlet.h"
namespace Magic{
namespace Http{
    enum class WebSocketMessageType{
        CONTINUE = 0,
        TEXT = 1,
        CLOSE = 8,
        PING = 9,
        PONG = 0xA
    };
    class WebSocket :public Socket{
    public:
        typedef std::function<void(const Share<WebSocket>,const Safe<WebSocketMessage>&)> WebSocketHandle;
        WebSocket(uint64_t timeOutMs,Safe<WebSocketServletDispatch>& Servlet,asio::io_context& context);
        void accept();
        void recvMessage();
        void sendMessage(const Safe<WebSocketMessage>& msg,bool isContinue =false);
        void recvMessage(const WebSocketHandle& callBack){
            m_RecvWebSocketCallBack = std::move(callBack);
            this->recv(2,[this](const Share<Socket>& conn,StreamBuffer& data){
                WebSocketMessageType wsMsgType;
                const char* frame = data.data();
                bool fin = (frame[0] >> 7) & 0x01;
                unsigned char opCode = frame[0] & 0x0F;
                bool isMask = (frame[1] >> 7) & 0x01;
                uint16_t length = frame[1] & (~0x80);
                switch(opCode){
                    case 0:
                        wsMsgType = WebSocketMessageType::CONTINUE;
                        break;
                    case 1:
                        wsMsgType = WebSocketMessageType::TEXT;
                        break;
                    case 8:
                        wsMsgType = WebSocketMessageType::CLOSE;
                        break;
                    case 9:
                        wsMsgType = WebSocketMessageType::PING;
                        break;
                    case 0xA:
                        wsMsgType = WebSocketMessageType::PONG;
                        break;
                    default:
                        MAGIC_LOG(LogLevel::LogDebug) << "OpCode error";
                        return;
                }
                if(!isMask && m_Server){
                     MAGIC_LOG(LogLevel::LogDebug) << "Mask error";
                }
                data.clear();
                if(length == 126){
                    this->recv(sizeof(uint16_t),[this](const Share<Socket>& conn,StreamBuffer& data){
                        uint64_t len = *reinterpret_cast<uint16_t*>(data.data());
                        data.clear();
                        this->recvMessageData(len);
                    });
                }else if(length == 127){
                    this->recv(sizeof(uint64_t),[this](const Share<Socket>& conn,StreamBuffer& data){
                        uint64_t len = *reinterpret_cast<uint64_t*>(data.data());
                        data.clear();
                        this->recvMessageData(len);
                    });
                }else{
                    uint64_t len = length;
                    this->recvMessageData(len);
                }
            });
        }

        void sendMessage(const char* data,uint64_t length,WebSocketMessageType type){
            std::string bytesFormatted;
            uint16_t indexStartPos = 0;
            bytesFormatted.resize(10);
            bytesFormatted[0] = char(0x80 | (static_cast<uint8_t>(type) & 0x0F));
            if(length <= 125){
                bytesFormatted[1] = length;
                indexStartPos = 2;
            }else if(length <= 65535){
                bytesFormatted[1] = 126;
                bytesFormatted[2] = (length >> 8) & 0xFF;
                bytesFormatted[3] = (length & 0xFF);
                indexStartPos = 4;
            }else{
                bytesFormatted[1] = 127;
                bytesFormatted[2] = ((length >> 56) & 0xFF);
                bytesFormatted[3] = ((length >> 48) & 0xFF);
                bytesFormatted[4] = ((length >> 40) & 0xFF);
                bytesFormatted[5] = ((length >> 32) & 0xFF);
                bytesFormatted[6] = ((length >> 24) & 0xFF);
                bytesFormatted[7] = ((length >> 16) & 0xFF);
                bytesFormatted[8] = ((length >> 8) & 0xFF);
                bytesFormatted[9] = ((length) & 0xFF);
                indexStartPos = 10;
            }
            if(!m_Server){
                unsigned char mask[4] = {0};
                uint32_t randValue = std::rand();
                bytesFormatted.resize(indexStartPos + 4 + length);
                *(reinterpret_cast<uint32_t*>(&bytesFormatted[indexStartPos])) = randValue;
                for(uint64_t i =0; i < length; i++){
                    bytesFormatted[indexStartPos + 4 + i] = (data[i] ^ bytesFormatted[indexStartPos + (i % 4)]);
                }
            }else{
                bytesFormatted.resize(indexStartPos);
                bytesFormatted.append(data,length);
            }
            this->send(bytesFormatted);
        }
    private:
        void recvMessageData(uint64_t length){
            if(m_Server){
                this->recv(4 + length,[this](const Share<Socket>& conn,StreamBuffer& data){
                    const char* mask = reinterpret_cast<char*>(data.data());
                    for(uint64_t i = 4; i < data.size(); i++){
                        data[i] ^= mask[i % 4];
                    }
                    //m_RecvWebSocketCallBack(this->shared_from_this(),)
                });
            }else{
                this->recv(length,[this](const Share<Socket>& conn,StreamBuffer& data){
                    //m_RecvWebSocketCallBack(this->shared_from_this(),)
                });
            }
        }
        void sendData(std::string& data);
        void recvData(uint64_t length,bool isContinue);
    private:
        bool m_Server;
        std::string m_Name;
        WebSocketHandle m_RecvWebSocketCallBack;
        Safe<Http::HttpRequestParser> m_RequestParser;
        Safe<Http::HttpResponseParser> m_ResponseParser;
        Safe<WebSocketServletDispatch>& m_ServletDispatch;
    };
}
}