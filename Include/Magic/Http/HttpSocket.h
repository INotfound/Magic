#pragma once
#include "Core.h"
#include "Macro.h"
#include "Socket.h"
#include "Http/Http.h"
#include "Http/HttpParser.h"
namespace Magic{
namespace Http{
    class HttpSocket :public Socket{
    public:
        typedef std::function<void(const Safe<HttpRequest>&,const Safe<HttpResponse>&)> HttpHandle;
        HttpSocket(uint64_t timeOutMs,asio::io_context& context);
        void Go(HttpHandle callBack);
    private:
        void handle();
        void handleBody();
        void handleHeader();
        void handleResponse();
    private:
        HttpHandle m_CallBack;
        Safe<HttpRequestParser> m_RequestParser;
        Safe<HttpResponseParser> m_ResponseParser;
    };
}
}