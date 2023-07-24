/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : TcpClient.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Socket.hpp"

namespace Magic{
namespace NetWork{
    class TcpClient:public Noncopyable{
    public:
        TcpClient(const StringView& ip,uint16_t port);

        /**
         * @brief 运行
         */
        void run();

        /**
         * @brief 关闭连接
         */
        void close();

        /**
         * @brief 异步接收数据
         * @param callBack
         */
        void recv(Socket::RecvCallBack callBack);

        /**
         * @brief 异步连接
         */
        void connect(std::function<void()> callBack);

        /**
         * @brief 设置错误代码回调函数
         * @param errorCallBack 错误码回调函数
         */
        void setErrorCodeCallBack(Socket::ErrorCallBack errorCallBack);

        /**
         * @brief 异步发送数据函数
         * @param data 二进制或文本数据
         * @param length 长度
         * @param callBack 发送完成后响应函数
         */
        void send(const IStream::BufferView& buffer,Socket::SendCallBack callBack = nullptr);

        /**
         * @brief 异步发送数据函数
         * @param stream Asio流式缓存数据
         * @param callBack 发送完成后响应函数
         */
        void send(const Safe<asio::streambuf>& stream,Socket::SendCallBack callBack = nullptr);

    private:
        uint16_t m_Port;
        Safe<Socket> m_Socket;
        std::string m_IpAddress;
        asio::io_context m_IOService;
    };
}
}