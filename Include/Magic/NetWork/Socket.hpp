/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Socket.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once

#include "asio.hpp"

#ifdef OPENSSL
#include "asio/ssl.hpp"
#endif

#include <atomic>
#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/TimingWheel.hpp"

namespace Magic{
namespace NetWork{
    /**
     * @brief Socket类
     */
    class Socket:public Noncopyable,public std::enable_shared_from_this<Socket>{
    public:
        /// 数据流缓存
        typedef std::vector<char> StreamBuffer;
        /// 发送回调方法
        typedef std::function<void()> SendCallBack;
        /// 数据接收方法
        typedef std::function<void(StreamBuffer&)> RecvCallBack;
        /// 错误回调方法
        typedef std::function<void(const asio::error_code&)> ErrorCallBack;
        /// 心跳回调方法
        typedef std::function<void(const Safe<Socket>&)> HeartBeatCallBack;

        /**
         * @brief Socket
         */
        ~Socket();

        /**
         * @brief 构造函数
         * @param heartBeatMs Socket心跳检测时间(毫秒)
         * @param bufferSize 数据接收缓冲区大小
         * @param context Asio的上下文
         */
        Socket(uint64_t heartBeatMs,uint64_t bufferSize,asio::io_context& context);

        /**
         * @brief 关闭Socket
         */
        void close();

        /**
         * @brief 是否是正在工作
         * @return true 工作中 false 未工作
         */
        bool isWorking() const;

        /**
         * @brief 运行Socket心跳
         * @param life 需要被"保活"的对象
         * @see setHeartBeatCallBack 函数
         * @note 调用该函数后会定时调用 HeartBeatCallBack 回调函数,每调用一次都会延长 life 生命周期
         */
        void runHeartBeat(Safe<void> life);

        /**
         * @brief 获取Socket的实体函数
         * @return: 返回Socket实体对象
         */
        const Safe<asio::ip::tcp::socket>& getEntity();
    #ifdef OPENSSL
        /**
         * @brief 获取SslStream的实体函数
         * @return: 返回SslStream实体对象
         * @note 编译环境中存在 SSL 库才会有该函数
         */
        const Safe<asio::ssl::stream<asio::ip::tcp::socket&>>& getSslEntity();
        /**
         * @brief 启用SSL功能
         * @param keyPath 密钥路径
         * @param certPath 证书路径
         * @note 编译环境中存在 SSL 库才会有该函数
         */
        void enableSsl(const Magic::StringView& keyPath = "",const Magic::StringView& certPath = "");
    #endif
        /**
         * @brief 接收数据函数
         * @param callBack 接收数据回调函数
         * @warning 该函数不保证生命周期,需要注意！
         */
        void recv(RecvCallBack callBack);

        /**
         * @brief 接收数据函数
         * @param size 指定接收大小
         * @param callBack 接收数据回调函数
         * @warning 该函数不保证生命周期,需要注意！
         */
        void recv(uint64_t size,RecvCallBack callBack);

        /**
         * @brief 获取错误处理函数
         * @return 获取错误回调函数对象
         */
        const ErrorCallBack& getErrorCodeCallBack() const;

        /**
         * @brief 设置错误代码回调函数
         * @param errorCallBack 错误码回调函数
         */
        void setErrorCodeCallBack(ErrorCallBack errorCallBack);

        /**
         * @brief 设置超时回调函数
         * @param heartBeatCallBack 超时回调函数
         */
        void setHeartBeatCallBack(HeartBeatCallBack heartBeatCallBack);

        /**
         * @brief 发送数据函数
         * @param data 二进制或文本数据
         * @param length 发送数据的长度
         * @param callback 发送完成后响应函数
         * @warning 该函数不保证生命周期,需要注意！
         */
        void send(const char* data,uint64_t length,SendCallBack callBack = nullptr);

        /**
         * @brief 发送数据函数
         * @param stream Asio流式缓存数据
         * @param callback 发送完成后响应函数
         * @warning 该函数不保证生命周期,需要注意！
         */
        void send(const Safe<asio::streambuf>& stream,SendCallBack callBack = nullptr);

    private:
        std::mutex m_Mutex;
        uint64_t m_BufferSize;
        uint64_t m_HeartBeatMs;
        Safe<char> m_ByteBlock;
        std::atomic_bool m_Working;
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
