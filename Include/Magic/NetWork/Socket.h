/*
 * @File: Socket.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:11:54
 * @LastEditTime: 2020-09-30 21:17:50
 */
#pragma once
#include "asio.hpp"
#ifdef OPENSSL
#include "asio/ssl.hpp"
#endif
#include <atomic>
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Mutex.h"
#include "Magic/Utilty/TimingWheel.h"

namespace Magic{
namespace NetWork{
    /**
     * @brief Socket类
     */
    class Socket :public std::enable_shared_from_this<Socket>{
    public:
        /// 数据流缓存
        typedef std::vector<char> StreamBuffer;
        /// 错误回调方法
        typedef std::function<void()> SendCallBack;
        /// 数据接收方法
        typedef std::function<void(StreamBuffer&)> RecvCallBack;
        /// 错误回调方法
        typedef std::function<void(const asio::error_code&)> ErrorCallBack;
        /// 超时回调方法
        typedef std::function<void(const Safe<Socket>&)> HeartBeatCallBack;
        /**
         * @brief Socket
         */
        ~Socket();
        /**
         * @brief 构造函数
         * @param heartBeatMs Socket心跳检测时间(毫秒)
         * @param bufferSize 缓存大小
         * @param context Asio的处理上下文
         */
        Socket(uint64_t heartBeatMs,uint64_t bufferSize,asio::io_context& context);
        /**
         * @brief 关闭Socket
         */
        void close();
        /**
         * @brief
         * @param ms
         */
        void setHeartBeatTime(uint64_t ms);
        /**
         * @brief 运行Socket心跳
         */
        void runHeartBeat(const Safe<void>& life);
        /**
         * @brief 获取Socket的实体函数
         * @return: 返回Socket实体
         */
        const Safe<asio::ip::tcp::socket>& getEntity();
    #ifdef OPENSSL
        /**
         * @brief 获取SslStream的实体函数
         * @return: 返回SslStream实体
         */
        const Safe<asio::ssl::stream<asio::ip::tcp::socket&>>& getSslEntity();
        /**
         * @brief 启用SSL功能
         * @param sslStream ssl流对象
         */
        void enableSsl(const Safe<asio::ssl::stream<asio::ip::tcp::socket&>>& sslStream);
    #endif
        /**
         * @brief 发送数据函数
         * @param data 二进制或文本数据
         * @param length 长度
         * @param callback 发送完成后响应函数
         * @warning 回调函数不会保证生命周期,需要注意！
         */
        void send(const char* data,uint64_t length,const SendCallBack callback = nullptr);
        /**
         * @brief 发送数据函数
         * @param stream Asio流式缓存数据
         * @param callback 发送完成后响应函数
         * @warning 回调函数不会保证生命周期,需要注意！
         */
        void send(const Safe<asio::streambuf>& stream,const SendCallBack callback = nullptr);
        /**
         * @brief 接收数据函数
         * @param callBack 接收数据回调函数
         * @warning 回调函数不会保证生命周期,需要注意！
         */
        void recv(const RecvCallBack callBack);
        /**
         * @brief 接收数据函数
         * @param size 指定接收大小
         * @param callBack 接收数据回调函数
         * @warning 回调函数不会保证生命周期,需要注意！
         */
        void recv(uint64_t size,const RecvCallBack callBack);
        /**
         * @brief 获取错误处理函数
         * @return
         */
        const ErrorCallBack& getErrorCodeCallBack() const;
        /**
         * @brief 设置错误代码回调函数
         * @param errorCallBack 错误码回调函数
         */
        void setErrorCodeCallBack(const ErrorCallBack errorCallBack);
        /**
         * @brief 设置超时回调函数
         * @param heartBeatCallBack 超时回调函数
         */
        void setHeartBeatCallBack(const HeartBeatCallBack heartBeatCallBack);
    private:
        Mutex m_Mutex;
        uint64_t m_BufferSize;
        uint64_t m_HeartBeatMs;
        Safe<char> m_ByteBlock;
        StreamBuffer m_StreamBuffer;
        ErrorCallBack m_ErrorCodeCallBack;
        Safe<asio::ip::tcp::socket> m_Socket;
        HeartBeatCallBack m_HeartBeatCallBack;
    #ifdef OPENSSL
        Safe<asio::ssl::stream<asio::ip::tcp::socket&>> m_SslStream;
    #endif
    };
}
}
