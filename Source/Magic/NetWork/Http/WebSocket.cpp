/*
 * @Author: INotFound
 * @Date: 2022-06-1 11:42:05
 * @LastEditTime: 2022-06-1 21:53:05
 */
#include <random>
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Http/WebSocket.h"

namespace Magic{
namespace NetWork{
namespace Http{
    template<class T,class B>
    inline T ByteToType(const B* src){
        T value = 0;
        for(uint32_t i = 0;i < sizeof(T);i++){
            value = value << 8 | src[i];
        }
        return value;
    }

    WebSocket::~WebSocket() =default;

    WebSocket::WebSocket(bool mask,const Safe<Socket>& socket)
        :m_Mask(mask)
        ,m_OpCode(0x0)
        ,m_Socket(socket)
        ,m_Death(false){

        m_Socket->setHeartBeatCallBack([this](const Safe<Socket>& socket){
            if(m_Death){
                m_DisconnectedCallBack(this->shared_from_this());
                socket->close();
                return;
            }
            m_Death = true;
            this->sendEncodePackage(0x9,"Magic");
            socket->runHeartBeat(this->shared_from_this());
        });

        m_Socket->setErrorCodeCallBack([this](const asio::error_code & err){
            m_Death = true;
        #ifdef WIN32
            if(err.value() == WSAECONNABORTED) return;
        #endif
            if(err == asio::error::eof || err == asio::error::operation_aborted)
                return;
            MAGIC_WARN() << err.message();
        });
    }

    void WebSocket::sendTextMessage(const std::string& message){
        this->sendEncodePackage(0x1,message);
    }

    void WebSocket::sendBinaryMessage(const std::string& message){
        this->sendEncodePackage(0x2,message);
    }

    void WebSocket::recvTextMessage(const RecvCallBack& callBack){
        m_TextMessageCallBack = callBack;
    }

    void WebSocket::recvBinaryMessage(const RecvCallBack& callBack){
        m_BinaryMessageCallBack = callBack;
    }

    void WebSocket::disconnectedCallBack(const DisconnectedCallBack& callBack){
        m_DisconnectedCallBack = callBack;
    }

    void WebSocket::runAnalyse(){
        this->handleProtocol();
    }

    void WebSocket::handleProtocol(){
        if(!m_Socket)
            return;
        auto self = this->shared_from_this();
        m_Socket->recv([this,self](Socket::StreamBuffer& streamBuffer){
            if(streamBuffer.size() >= 2) {
                uint32_t offset = 2;
                const uint8_t * data = reinterpret_cast<uint8_t*>(streamBuffer.data());
                uint8_t one = data[0];
                uint8_t two = data[1];
                m_OpCode = one & 0x0F;
                uint8_t length = two & (~0x80);
                m_Fin = ((one >> 7) & 0x01) == 0x01 ? true : false;
                bool mask = ((two >> 7) & 0x01) == 1 ? true : false;

                if(length <= 125) {
                    if (streamBuffer.size() < (offset + length)) {
                        this->handleProtocol();
                    } else {
                        this->handleMaskPayload(mask, offset, streamBuffer);
                    }
                }else if(length == 126) {
                    length = 2;
                    offset += length;
                    if(streamBuffer.size() < offset) {
                        this->handleProtocol();
                    }else {
                        auto size = ByteToType<uint16_t>(data + 2);
                        if(streamBuffer.size() < (offset + size + (m_Mask ? 4 : 0))) {
                            this->handleProtocol();
                        }else{
                            this->handleMaskPayload(mask, offset, streamBuffer);
                        }
                    }
                }else if(length == 127) {
                    length = 8;
                    offset += length;
                    if(streamBuffer.size() < offset) {
                        this->handleProtocol();
                    }else {
                        auto size = ByteToType<uint64_t>(data + 2);
                        if (streamBuffer.size() < (offset + size + (m_Mask ? 4 : 0))) {
                            this->handleProtocol();
                        } else {
                            this->handleMaskPayload(mask, offset, streamBuffer);
                        }
                    }
                }else{
                    m_Death = true;
                }
            }
        });
    }

    void WebSocket::sendEncodePackage(uint8_t opCode,const std::string& rawData){
        char package[14] = {0};
        uint8_t packageLength = 0;
        uint64_t length = rawData.size();
        package[0] = 128 + opCode;

        if(m_Mask)
            package[1] = 0x80;

        if(length < 126){
            package[1] |= static_cast<char>(length) & 0x7F;
            packageLength = 2;
        }else if(length <= UINT16_MAX){
            package[1] |= 0x7E;
            package[2] = static_cast<char>(length >> 8) & 0xFF;
            package[3] = static_cast<char>(length >> 0) & 0xFF;
            packageLength = 4;
        }else{
            package[1] |= 0x7F;
            package[2] = static_cast<char>(length >> 56) & 0xFF;
            package[3] = static_cast<char>(length >> 48) & 0xFF;
            package[4] = static_cast<char>(length >> 40) & 0xFF;
            package[5] = static_cast<char>(length >> 32) & 0xFF;
            package[6] = static_cast<char>(length >> 24) & 0xFF;
            package[7] = static_cast<char>(length >> 16) & 0xFF;
            package[8] = static_cast<char>(length >> 8) & 0xFF;
            package[9] = static_cast<char>(length >> 0) & 0xFF;
            packageLength = 10;
        }

        Safe<asio::streambuf> streamBuffer = std::make_shared<asio::streambuf>();
        std::ostream stream(streamBuffer.get());
        const char* data = rawData.data();
        if(m_Mask){
            std::random_device randomDevice;
            std::default_random_engine defaultRandomEngine(randomDevice());
            std::uniform_int_distribution<uint32_t> uniformIntDistribution(0x80000000, UINT32_MAX);
            uint32_t maskKey = uniformIntDistribution(defaultRandomEngine);
            package[packageLength] = static_cast<char>(maskKey >> 24) & 0xFF;
            package[packageLength + 1] = static_cast<char>(maskKey >> 16) & 0xFF;
            package[packageLength + 2] = static_cast<char>(maskKey >> 8) & 0xFF;
            package[packageLength + 3] = static_cast<char>(maskKey >> 0) & 0xFF;
            packageLength += 4;
            stream.write(package,packageLength);

            if(length > 0){
                uint32_t maskIdx = 0;
                uint8_t packageMask = 0;
                uint8_t packageData = 0;
                for(uint32_t i = 0;i < length;i++){
                    if(maskIdx == 4)
                        maskIdx = 0;
                    packageData = data[i];
                    packageMask = package[packageLength + maskIdx];
                    stream.put(static_cast<char>(((~packageMask)&packageData) | (packageMask&(~packageData))));
                    maskIdx++;
                }
                m_Socket->send(streamBuffer);
            }
        }else{
            stream.write(package,packageLength);
            if(length > 0){
                for(uint64_t i = 0;i < length;i++){
                    stream.put(data[i]);
                }
                m_Socket->send(streamBuffer);
            }
        }
    }

    void WebSocket::handleMaskPayload(bool mask,uint32_t offset,Socket::StreamBuffer& streamBuffer){
        char* data = streamBuffer.data() + offset;

        if(mask){
            offset += 4;
            uint64_t length = 0;
            char maskData[4] = {0};

            std::memcpy(maskData,data,4);
            data = streamBuffer.data() + offset;
            length = streamBuffer.size() - offset;
            for(uint64_t i = 0; i < length; i++) {
                data[i] ^= maskData[i % 4];
            }
        }

        m_RawData.append(streamBuffer.begin() + offset,streamBuffer.end());
        streamBuffer.clear();

        if(!m_Fin){
            this->handleProtocol();
            return;
        }

        switch (m_OpCode) {
        case 0x0: /// Contine
            break;
        case 0x1: /// Text
            if(m_TextMessageCallBack)
                m_TextMessageCallBack(this->shared_from_this(),m_RawData);
            break;
        case 0x2: /// Binary
            if(m_BinaryMessageCallBack)
                m_BinaryMessageCallBack(this->shared_from_this(),m_RawData);
            break;
        case 0x8: /// Closed
            m_Death = true;
            break;
        case 0x9: /// Ping
            /// Send Pong
            this->sendEncodePackage(0xA,m_RawData);
            break;
        case 0xA: /// Pong
            m_Death = false;
            break;
        }

        m_RawData.clear();
        this->handleProtocol();
    }
}
}
}