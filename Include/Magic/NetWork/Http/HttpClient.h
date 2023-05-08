/*
 * @File: TcpClient.h
 * @Author: INotFound
 * @Date: 2022-07-30 22:11:54
 * @LastEditTime: 2022-07-30 23:17:50
 */
#pragma once

#include "Magic/Core/Core.h"
#include "Magic/NetWork/Http/Uri.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpSocket.h"

namespace Magic{
namespace NetWork{
namespace Http{
    class HttpClient:public Noncopyable,public std::enable_shared_from_this<HttpClient>{
    public:
        ~HttpClient();

        /**
         * @brief HttpClient
         * @param url 地址
         * @param timeOutMs 超时时间,默认1000毫秒
         */
        explicit HttpClient(const std::string_view& url,uint64_t timeOutMs = 1000);

        /**
         * @brief 执行Http请求处理
         * @param request 请求头对象
         */
        void execute(Safe<HttpRequest>&& request);

        /**
         * @brief 超时函数
         * @param callBack 回调函数
         * @return HttpClient 对象
         */
        ObjectWrapper<HttpClient> onTimeOut(std::function<void()> callBack);

        /**
         * @brief 错误函数
         * @param callBack 回调函数
         * @return HttpClient 对象
         */
        ObjectWrapper<HttpClient> onError(std::function<void(const asio::error_code&)> callBack);

        /**
         * @brief 响应函数
         * @param callBack 回调函数
         * @return HttpClient 对象
         */
        ObjectWrapper<HttpClient> onResponse(std::function<void(const Safe<HttpResponse>&)> callBack);

    private:
        std::string m_Url;
        Safe<Socket> m_Socket;
        std::atomic_bool m_Death;
        std::atomic_bool m_Finish;
        Safe<asio::io_context> m_IOService;
        std::function<void()> m_TimeOutCallBack;
        std::function<void(const Safe<HttpResponse>&)> m_ResponseCallBack;
    };
}
}
}