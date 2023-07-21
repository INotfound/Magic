/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : WebSocket.hpp
 * @Date           : 2023-07-03 18:28
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Socket.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief WebSocket类
     */
    class WebSocket:public Noncopyable,public std::enable_shared_from_this<WebSocket>{
        friend class HttpSocket;

    public:
        typedef std::function<void(const Safe<WebSocket>&)> DisconnectedCallBack;
        typedef std::function<void(const Safe<WebSocket>&,const Magic::StringView&)> RecvCallBack;
    public:
        ~WebSocket();

        /**
         * @brief WebSocket
         * @param mask 是否使用掩码,客户端置为 true|否则置为 false
         * @param socket Socket实体
         */
        WebSocket(bool mask,const Safe<Socket>& socket);

        /**
         * @brief 接收文本消息
         * @param callBack 接收文本消息回调函数
         */
        void recvTextMessage(RecvCallBack callBack);

        /**
         * @brief 接收二进制消息
         * @param callBack 接收二进制消息回调函数
         */
        void recvBinaryMessage(RecvCallBack callBack);

        /**
         * @brief 发送文本消息
         * @param message 文本消息
         */
        void sendTextMessage(const Magic::StringView& message);

        /**
         * @brief 发送二进制消息
         * @param message 二进制消息
         */
        void sendBinaryMessage(const Magic::StringView& message);

        /**
         * @brief 连接断开
         * @param callBack 连接断开回调函数
         * @warning 该函数回调处于其他线程中,需进行线程同步的操作
         */
        void disconnectedCallBack(DisconnectedCallBack callBack);

    private:
        /**
         * @brief 开始解析
         */
        void runAnalyse();

        /**
         * @brief 开始解析协议
         */
        void handleProtocol();

        /**
         * @brief 发送编码包体
         * @param opCode 操作码
         * @param rawData 原始数据
         */
        void sendEncodePackage(uint8_t opCode,const Magic::StringView& rawData);

        /**
         * @brief 解析数据体
         * @param mask 是否掩码
         * @param offset 当前数据偏移量
         * @param streamBuffer 数据内容缓存
         */
        void handleMaskPayload(bool mask,uint32_t offset,DataStream& streamBuffer);

    private:
        bool m_Fin;
        bool m_Mask;
        uint8_t m_OpCode;
        std::string m_RawData;
        Safe<Socket> m_Socket;
        std::atomic_bool m_Death;
        RecvCallBack m_TextMessageCallBack;
        RecvCallBack m_BinaryMessageCallBack;
        DisconnectedCallBack m_DisconnectedCallBack;
    };
}
}
}