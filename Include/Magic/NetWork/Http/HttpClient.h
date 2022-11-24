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
    class HttpClient:public std::enable_shared_from_this<HttpClient>{
    public:
        ~HttpClient();

        explicit HttpClient(const std::string& url,uint64_t timeOutMs = 1000);

        void execute(const Safe<HttpRequest>& request);

        ObjectWrapper<HttpClient> onTimeOut(const std::function<void()>& callback);

        ObjectWrapper<HttpClient> onError(const std::function<void(const asio::error_code&)>& callBack);

        ObjectWrapper<HttpClient> onResponse(const std::function<void(const Safe<HttpResponse>&)>& callback);

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