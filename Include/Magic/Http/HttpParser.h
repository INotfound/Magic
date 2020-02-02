#pragma once
#include "Core.h"
#include "Http.h"
#include "Util/http11_parser.h"
#include "Util/httpclient_parser.h"
namespace Magic{
namespace Http{
    class HttpRequestParser{
    public:
        HttpRequestParser();
        bool hasError();
        bool isFinished();
        void setError(bool val);
        uint64_t getContentLength();
        Safe<HttpRequest>& getData();
        uint32_t execute(char* data,uint32_t length);

    private:
        bool m_Error{};
        http_parser m_Parser;
        Safe<HttpRequest> m_Data;
    };
    class HttpResponseParser{
    public:
        HttpResponseParser();
        bool hasError();
        bool isFinished();
        void setError(bool val);
        uint32_t getContentLength();
        Safe<HttpResponse>& getData();
        uint32_t execute(char* data,uint32_t length);

    private:
        bool m_Error{};
        httpclient_parser m_Parser{};
        Safe<HttpResponse> m_Data{};
    };
}
}