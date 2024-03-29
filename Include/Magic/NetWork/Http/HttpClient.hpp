/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpClient.hpp
 * @Date           : 2023-07-03 18:27
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Http/Uri.hpp"
#include "Magic/NetWork/Http/Http.hpp"
#include "Magic/NetWork/Http/HttpSocket.hpp"

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
        explicit HttpClient(const StringView& url,uint64_t timeOutMs = 1000);

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