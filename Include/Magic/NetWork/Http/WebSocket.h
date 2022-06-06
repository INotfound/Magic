/*
 * @Author: INotFound
 * @Date: 2022-06-01 11:42:05
 * @LastEditTime: 2022-06-01 21:53:05
 */
#pragma once
#include "Magic/Core/Core.h"
#include "Magic/NetWork/Socket.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief: HttpSocket类
     */
    class HttpSocket;
    class WebSocket :public std::enable_shared_from_this<WebSocket>{
        friend class HttpSocket;
    public:
        enum class Status{
            Opened,
            Closed
        };
    public:
        typedef std::function<void(const std::string&)> RecvCallBack;
        ~WebSocket();
        WebSocket(bool mask,const Safe<Socket>& socket);
        Status getStatus() const;
        void sendTextMessage(const std::string& message);
        void sendBinaryMessage(const std::string& message);
        void recvTextMessage(const RecvCallBack& callBack);
        void recvBinaryMessage(const RecvCallBack& callBack);
    private:
        void runAnalyse();
        void sendEncodePackage(uint8_t opCode,const std::string& rawData);
        void handleMaskPayload(bool mask,uint32_t offset,Socket::StreamBuffer& streamBuffer);
    private:
        bool m_Fin;
        bool m_Mask;
        Status m_Status;
        uint8_t m_OpCode;
        std::string m_RawData;
        Safe<Socket> m_Socket;
        std::atomic_bool m_Death;
        RecvCallBack m_TextMessageCallBack;
        RecvCallBack m_BinaryMessageCallBack;

    };
}
}
}
