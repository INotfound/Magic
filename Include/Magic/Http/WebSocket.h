/*
 * @file: WebSocket.h
 * @Author: INotFound
 * @Date: 2020-03-18 22:45:10
 * @LastEditTime: 2020-04-06 19:52:12
 */
#pragma once
#include "Http.h"
#include "HttpParser.h"
#include "WebSocketMessage.h"
#include "WebSocketServlet.h"

#include "../Macro.h"
#include "../Socket.h"
#include "../Crypto.h"

namespace Magic{
namespace Http{

    class WebSocket :public Socket{
    public:
        typedef std::function<void(const Share<WebSocket>)> OpenCallBack;
        typedef std::function<void(const Share<WebSocket>,const Safe<WebSocketMessage>&)> RecvMessageCallBack;
        WebSocket(uint64_t timeOutMs,asio::io_context& context);
        void accept();
        void onOpen(const OpenCallBack& callBack){
            m_OpenWebSocketCallBack = std::move(callBack);
        }
        void onMessage(const RecvMessageCallBack& callBack){
            m_RecvWebSocketCallBack = std::move(callBack);
            this->recvMessage();
        }

        void recvMessage(){
            this->recv(2,[this](const Share<Socket>& conn,StreamBuffer& data){
                WebSocketMessageType msgType;
                const char* frame = data.data();
                //bool fin = (frame[0] >> 7) & 0x01;
                unsigned char opCode = frame[0] & 0x0F;
                bool isMask = (frame[1] >> 7) & 0x01;
                uint16_t length = frame[1] & 0x7F;
                switch(opCode){
                    case 0:
                        msgType = WebSocketMessageType::CONTINUE;
                        break;
                    case 1:
                        msgType = WebSocketMessageType::TEXT;
                        break;
                    case 8:
                        msgType = WebSocketMessageType::CLOSE;
                        break;
                    case 9:
                        msgType = WebSocketMessageType::PING;
                        break;
                    case 0xA:
                        msgType = WebSocketMessageType::PONG;
                        break;
                    default:
                        MAGIC_DEBUG() << "OpCode error code: " << (uint16_t)opCode;
                        return;
                }
                if(!isMask && m_Server){
                    MAGIC_DEBUG() << "Mask error";
                }
                MAGIC_DEBUG() << length << " xxx: " << data.size();
                data.clear();
                if(length == 126){
                    this->recv(sizeof(uint16_t),[this,msgType](const Share<Socket>& conn,StreamBuffer& data){
                        uint64_t len = *reinterpret_cast<uint16_t*>(data.data());
                        data.clear();
                        this->recvMessageData(len,msgType);
                    });
                }else if(length == 127){
                    this->recv(sizeof(uint64_t),[this,msgType](const Share<Socket>& conn,StreamBuffer& data){
                        uint64_t len = *reinterpret_cast<uint64_t*>(data.data());
                        data.clear();
                        this->recvMessageData(len,msgType);
                    });
                }else{
                    uint64_t len = length;
                    this->recvMessageData(len,msgType);
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
        void recvMessageData(uint64_t length,WebSocketMessageType type){
            if(m_Server){
                this->recv(4 + length,[this,type](const Share<Socket>& conn,StreamBuffer& data){
                    const char* mask = reinterpret_cast<char*>(data.data());
                    for(uint64_t i = 4; i < data.size(); i++){
                        data[i] ^= mask[i % 4];
                    }
                    Safe<WebSocketMessage> msg(new WebSocketMessage(data.data() + 4,data.size() - 4,type));
                    data.clear();
                    auto self = std::static_pointer_cast<WebSocket>(conn);
                    m_RecvWebSocketCallBack(self,msg);
                    this->recvMessage();
                });
            }else{
                this->recv(length,[this,type](const Share<Socket>& conn,StreamBuffer& data){
                    Safe<WebSocketMessage> msg(new WebSocketMessage(data.data(),data.size(),type));
                    data.clear();
                    auto self = std::static_pointer_cast<WebSocket>(conn);
                    m_RecvWebSocketCallBack(self,msg);
                    this->recvMessage();
                });
            }
        }
    private:
        bool m_Server;
        std::string m_Name;
        OpenCallBack m_OpenWebSocketCallBack;
        RecvMessageCallBack m_RecvWebSocketCallBack;
        Safe<Http::HttpRequestParser> m_RequestParser;
        Safe<Http::HttpResponseParser> m_ResponseParser;
    };
}
}