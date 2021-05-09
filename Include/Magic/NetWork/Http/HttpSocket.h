/*
 * @Author: INotFound
 * @Date: 2020-03-15 17:48:05
 * @LastEditTime: 2020-10-22 21:53:05
 */
#pragma once
#include "Magic/Core/Core.h"
#include "Magic/NetWork/Socket.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/MultiPart.h"
#include "Magic/NetWork/Http/HttpParser.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief: HttpSocket类 
     */
    class HttpSocket :public Socket{
    public:
        /// Http处理方法
        typedef std::function<void(const Safe<HttpRequest>&,const Safe<HttpResponse>&)> HttpRecvBack;
        /**
         * @brief: 构造函数
         * @param timeOutMs 超时时间
         * @param context  Asio的处理上下文
         */
        HttpSocket(uint64_t timeOutMs,asio::io_context& context);
        /**
         * @brief: 接收请求头函数
         * @param callback 接收回调函数
         */
        void recvRequest(const HttpRecvBack& callback);
        /**
         * @brief: 接收响应头函数
         * @param callback 接收回调函数
         */
        void recvResponse(const HttpRecvBack& callback);
        /**
         * @brief: 设置临时目录路径
         * @param dirPath 目录路径
         */
        void setTempDirectory(const std::string& dirPath);
    private:
        /**
         * @brief: 处理请求头函数
         */
        void handleRequest();
        /**
         * @brief: 处理请求头函数
         */
        void handleResponse();
        /**
         * @brief: 请求头解析函数
         */
        void requestParser();
        /**
         * @brief: 响应头解析函数
         */
        void responseParser();
        /**
         * @brief: MultiPart解析函数
         */
        void multiPartParser();
        /**
         * @brief: 文件流数据传输函数
         */
        void transferFileStream();
    private:
        MultiPart m_MultiPart;
        uint64_t m_TotalLength;
        uint64_t m_CurrentLength;
        std::fstream m_FileStream;
        Safe<char> m_StreamBuffer;
        uint64_t m_StreamBufferSize;
        std::string m_TempDirectory;
        uint64_t m_TotalTransferLength;
        uint64_t m_CurrentTransferLength;
        HttpRecvBack m_RecvRequestCallBack;
        HttpRecvBack m_RecvResponseCallBack;
        Safe<HttpRequestParser> m_RequestParser;
        Safe<HttpResponseParser> m_ResponseParser;
    };
}
}
}
