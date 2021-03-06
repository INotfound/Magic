/*
 * @File: Socket.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:11:54
 * @LastEditTime: 2020-09-30 21:17:50
 */
#pragma once
#include "asio.hpp"
#include "Magic/Core/Core.h"

namespace Magic{
namespace NetWork{
    /**
     * @brief: Socket类
     */
    class Socket :public std::enable_shared_from_this<Socket>{
    public:
        /// 数据流缓存
        typedef std::vector<char> StreamBuffer;
        /// 错误回调方法
        typedef std::function<void()> SendCallBack;
        /// 错误回调方法
        typedef std::function<void(const asio::error_code&)> ErrorCallBack;
        /// 数据接收方法
        typedef std::function<void(const Safe<Socket>&,StreamBuffer&)> RecvCallBack;
        /**
         * @brief: 析构函数
         */
        virtual ~Socket();
        /**
         * @brief: 构造函数
         * @param timeOutMs Socket超时时间(毫秒)A
         * @param bufferSize 缓存大小
         * @param context Asio的处理上下文
         */
        Socket(uint64_t timeOutMs,uint64_t bufferSize,asio::io_context& context);
        /**
         * @brief: 获取Socket的实体函数
         * @return: 返回Socket实体
         */
        const Safe<asio::ip::tcp::socket>& getEntity();
        /**
         * @brief: 发送数据函数
         * @param data 二进制或文本数据
         * @param length 长度
         * @param callback 发送完成后响应函数
         */
        void send(const char* data,uint64_t length,const SendCallBack& callback = nullptr);
        /**
         * @brief: 发送数据函数
         * @param stream Asio流式缓存数据
         * @param callback 发送完成后响应函数
         */
        void send(const Safe<asio::streambuf>& stream,const SendCallBack& callback = nullptr);
        /**
         * @brief: 接收数据函数
         * @param callBack 接收数据回调函数
         */
        void recv(const RecvCallBack& callBack);
        /**
         * @brief: 接收数据函数
         * @param size 指定接收大小
         * @param callBack 接收数据回调函数
         */
        void recv(uint64_t size,const RecvCallBack& callBack);
        /**
         * @brief: 设置回调函数
         * @param errorCallBack 错误码回调函数
         */
        void setErrorCodeCallBack(const ErrorCallBack& errorCallBack);
    private:
        uint64_t m_TimeOutMs;
        uint64_t m_BufferSize;
        Safe<char[]> m_ByteBlock;
        StreamBuffer m_StreamBuffer;
        ErrorCallBack m_ErrorCodeCallBack;
        Safe<asio::ip::tcp::socket> m_Socket;
    };
}
}
