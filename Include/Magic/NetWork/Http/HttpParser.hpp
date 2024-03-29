/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpParser.hpp
 * @Date           : 2023-07-03 18:27
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Http/Http.hpp"

struct http_parser;
struct httpclient_parser;

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief Http请求头解析类
     */
    class HttpRequestParser:public Noncopyable{
    public:
        /**
         * @brief 构造函数
         */
        HttpRequestParser();

        /**
         * @brief 解析重置函数
         */
        void reset();

        /**
         * @brief 获取错误函数
         * @return: 返回True则发生了错误，返回False则未发生错误
         */
        bool hasError();

        /**
         * @brief 获取是否解析完成函数
         * @return: 返回True则解析完成，返回false则解析未完成
         */
        bool isFinished();

        /**
         * @brief 设置是否具有错误函数
         * @param val 是否错误
         */
        void setError(bool val);

        /**
         * @brief 获取主体正文长度函数
         * @return: 返回主体正文长度
         */
        uint64_t getContentLength();

        /**
         * @brief 获取解析后的请求对象函数
         * @return: 获取数据解析后的请求对象
         */
        const Safe<HttpRequest>& getData();

        /**
         * @brief 执行Http解析函数
         * @param data 需要解析的数据,未解析完的数据将进行向前偏移
         * @param length 解析数据的长度
         * @return: 获取已经解析的长度
         */
        uint32_t execute(char* data,uint32_t length);

    private:
        bool m_Error;
        Safe<HttpRequest> m_Data;
        Safe<http_parser> m_Parser;
    };

    /**
     * @brief Http响应头解析类
     */
    class HttpResponseParser:public Noncopyable{
    public:
        /**
         * @brief 构造函数
         */
        HttpResponseParser();

        /**
         * @brief 解析重置函数
         */
        void reset();

        /**
         * @brief 获取错误函数
         * @return: 返回True则发生了错误，返回False则未发生错误
         */
        bool hasError();

        /**
         * @brief 获取是否解析完成函数
         * @return: 返回True则解析完成，返回false则解析未完成
         */
        bool isFinished();

        /**
         * @brief 设置是否具有错误函数
         * @param val 是否错误
         */
        void setError(bool val);

        /**
         * @brief 获取主体正文长度函数
         * @return: 返回主体正文长度
         */
        uint32_t getContentLength();

        /**
         * @brief 获取解析后的请求对象函数
         * @return: 获取数据解析后的请求对象
         */
        const Safe<HttpResponse>& getData();

        /**
         * @brief 执行Http解析函数
         * @param data 需要解析的数据,未解析完的数据将进行向前偏移
         * @param length 解析数据的长度
         * @return: 获取已经解析的长度
         */
        uint32_t execute(char* data,uint32_t length);

    private:
        bool m_Error;
        Safe<HttpResponse> m_Data;
        Safe<httpclient_parser> m_Parser;
    };
}
}
}
