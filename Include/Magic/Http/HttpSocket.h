/*
 * @file: HttpSocket.h
 * @Author: INotFound
 * @Date: 2020-03-15 17:48:05
 * @LastEditTime: 2020-03-18 23:02:54
 */
#pragma once
#include "Core.h"
#include "Macro.h"
#include "Socket.h"
#include "Http/Http.h"
#include "Http/HttpParser.h"
namespace Magic{
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
    private:
        /**
         * @brief: 请求头解析函数
         */
        void requestParser();
        /**
         * @brief: 响应头解析函数
         */
        void responseParser();
    private:
        HttpRecvBack m_RecvRequestCallBack;
        HttpRecvBack m_RecvResponseCallBack;
        Safe<HttpRequestParser> m_RequestParser;
        Safe<HttpResponseParser> m_ResponseParser;
    };
}
}