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
#include "Magic/NetWork/Http/WebSocket.h"
#include "Magic/NetWork/Http/HttpParser.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief HttpSocket类
     */
    class HttpSocket:public std::enable_shared_from_this<HttpSocket>{
    public:
        /// Http处理方法
        typedef std::function<void(const Safe<HttpSocket>&)> HttpRecvBack;

        ~HttpSocket();

        /**
         * @brief 构造函数
         * @param timeOutMs 超时时间
         * @param context  Asio的处理上下文
         */
        explicit HttpSocket(const Safe<Socket>& socket);

        /**
         * @brief 启动心跳
         */
        void runHeartBeat();

        /**
        * @brief 设置临时目录路径
        * @param dirPath 目录路径
        */
        void setDirectory(const std::string& dirPath);

        /**
         * 获取Http请求
         * @return HttpRequest
         */
        const Safe<HttpRequest>& getRequest() const;

        /**
         * 获取Http响应
         * @return HttpResponse
         */
        const Safe<HttpResponse>& getResponse() const;

        /**
         * @brief 接收请求头函数
         * @param callback 接收回调函数
         * @warning 回调函数同HttpSocket生命周期一直存在！
         */
        void recvRequest(const HttpRecvBack& callback);

        /**
         * @brief 接收响应头函数
         * @param callback 接收回调函数
         * @warning 回调函数同HttpSocket生命周期一直存在！
         */
        void recvResponse(const HttpRecvBack& callback);

        /**
         * @brief 接收请求头函数
         * @param httpRequest 请求
         */
        void sendRequest(const Safe<HttpRequest>& httpRequest);

        /**
         * @brief 发送响应函数
         * @param httpResponse 响应
         */
        void sendResponse(const Safe<HttpResponse>& httpResponse);

        /**
         * @brief 升级为WebSocket
         * @param request 请求头
         * @param response 响应体
         * @param mask 是否使用掩码,客户端置为 true 服务端置为 false
         * @warning 当升级失败时候，返回 Safe<WebSocket> 会为 nullptr
         * @return Http转化的WebSocket实体
         */
        const Safe<WebSocket>& upgradeWebSocket(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response,bool mask = false);

    private:
        /**
         * @brief 处理请求头函数
         */
        void handleRequest();

        /**
         * @brief 处理请求头函数
         */
        void handleResponse();

        /**
         * @brief 请求头解析函数
         */
        void requestParser();

        /**
         * @brief 响应头解析函数
         */
        void responseParser();

        /**
         * @brief MultiPart解析函数
         */
        void multiPartParser();

        /**
         * @brief 文件流数据传输函数
         */
        void transferFileStream();

    private:
        Safe<Socket> m_Socket;
        uint64_t m_TotalLength;
        uint64_t m_CurrentLength;
        std::atomic_bool m_Death;
        Safe<char> m_StreamBuffer;
        std::ifstream m_FileStream;
        Safe<WebSocket> m_WebSocket;
        Safe<MultiPart> m_MultiPart;
        uint64_t m_StreamBufferSize;
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
