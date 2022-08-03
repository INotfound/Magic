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
    class HttpSocket;
    /**
     * @brief WebSocket类
     */
    class WebSocket :public std::enable_shared_from_this<WebSocket>{
        friend class HttpSocket;
    public:
        typedef std::function<void(const Safe<WebSocket>&)> DisconnectedCallBack;
        typedef std::function<void(const Safe<WebSocket>&,const std::string&)> RecvCallBack;
    public:
        ~WebSocket();
        /**
         * @brief 构建实体
         * @param mask 是否使用掩码,客户端置为 true 否则置为 false
         * @param socket Tcp Socket实体
         */
        WebSocket(bool mask,const Safe<Socket>& socket);
        /**
         * @brief 发送文本消息
         * @param message 文本消息
         */
        void sendTextMessage(const std::string& message);
        /**
         * @brief 发送二进制消息
         * @param message 二进制消息
         */
        void sendBinaryMessage(const std::string& message);
        /**
         * @brief 接收文本消息
         * @param callBack 接收文本消息回调函数
         */
        void recvTextMessage(const RecvCallBack& callBack);
        /**
         * @brief 接收二进制消息
         * @param callBack 接收二进制消息回调函数
         */
        void recvBinaryMessage(const RecvCallBack& callBack);
        /**
         * @brief 连接断开
         * @param callBack 连接断开回调函数
         * @warning 该函数回调处于其他线程中,需进行线程同步的操作
         */
        void disconnectedCallBack(const DisconnectedCallBack& callBack);
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
        void sendEncodePackage(uint8_t opCode,const std::string& rawData);
        /**
         * @brief 解析数据体
         * @param mask 是否掩码
         * @param offset 当前数据偏移量
         * @param streamBuffer 数据内容缓存
         */
        void handleMaskPayload(bool mask,uint32_t offset,Socket::StreamBuffer& streamBuffer);
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