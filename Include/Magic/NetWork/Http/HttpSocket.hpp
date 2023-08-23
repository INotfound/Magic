/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpSocket.hpp
 * @Date           : 2023-07-03 18:28
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Socket.hpp"
#include "Magic/NetWork/Http/Http.hpp"
#include "Magic/NetWork/Http/MultiPart.hpp"
#include "Magic/NetWork/Http/WebSocket.hpp"
#include "Magic/NetWork/Http/HttpParser.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief HttpSocket类
     */
    class HttpSocket:public Noncopyable,public std::enable_shared_from_this<HttpSocket>{
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
         * @brief 获取MultiPart数据
         * @return MultiPart对象
         */
        const Safe<MultiPart>& getMultiPart() const;

        /**
         * @brief 获取Http请求
         * @return HttpRequest对象
         */
        const Safe<HttpRequest>& getRequest() const;

        /**
         * @brief 获取Http响应
         * @return HttpResponse对象
         */
        const Safe<HttpResponse>& getResponse() const;

        /**
         * @brief 接收请求头函数
         * @param callBack 接收回调函数
         * @warning 回调函数同HttpSocket生命周期一直存在！
         */
        void recvRequest(HttpRecvBack callBack);

        /**
         * @brief 接收响应头函数
         * @param callBack 接收回调函数
         * @warning 回调函数同HttpSocket生命周期一直存在！
         */
        void recvResponse(HttpRecvBack callBack);

        /**
        * @brief 设置临时文件目录路径
        * @param dirPath 临时文件目录路径
        */
        void setDirectory(const StringView& dirPath);

        /**
         * @brief 接收请求头函数
         * @param httpRequest 请求
         */
        void sendRequest(HttpRequest& httpRequest);

        /**
         * @brief 发送响应函数
         * @param httpResponse 响应
         */
        void sendResponse(HttpResponse& httpResponse);

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
         * @brief 数据流传输函数
         */
        void transferDataStream();

    private:
        Safe<Socket> m_Socket;
        Safe<IStream> m_Stream;
        uint64_t m_TotalLength;
        uint64_t m_CurrentLength;
        std::atomic_bool m_Death;
        Safe<WebSocket> m_WebSocket;
        Safe<MultiPart> m_MultiPart;
        uint64_t m_TotalTransferLength;
        uint64_t m_CurrentTransferLength;
        HttpRecvBack m_RecvRequestCallBack;
        HttpRecvBack m_RecvResponseCallBack;
        Safe<HttpRequestParser> m_RequestParser;
        Safe<HttpResponseParser> m_ResponseParser;
    };

    inline void operator<<(const Safe<HttpSocket>& socket,const Safe<HttpRequest>& request){
        socket->sendRequest(*request);
    }

    inline void operator<<(const Safe<HttpSocket>& socket,const Safe<HttpResponse>& response){
        socket->sendResponse(*response);
    }

    inline void operator<<(const Safe<HttpSocket>& socket,const ObjectWrapper<HttpRequest>& request){
        socket->sendRequest(*request);
    }

    inline void operator<<(const Safe<HttpSocket>& socket,const ObjectWrapper<HttpResponse>& response){
        socket->sendResponse(*response);
    }

    std::string GenerateHtml(const StringView& status,const StringView& title,const StringView& message);
}
}
}
