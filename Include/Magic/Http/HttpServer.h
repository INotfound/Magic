/*
 * @file: HttpServer.h
 * @Author: INotFound
 * @Date: 2020-03-15 17:05:03
 * @LastEditTime: 2020-04-06 19:53:44
 */
#pragma once
#include "HttpServlet.h"

#include "../Core.h"
#include "../TcpServer.h"

namespace Magic{
namespace Http{
    /**
     * @brief: Http服务端类
     */
    class HttpServer: public TcpServer{
    public:
        /**
         * @brief: 构造函数
         * @param addr Ip地址
         * @param port Ip端口
         * @param threadCount 线程数
         * @param timeOutMs Socket超时时间(毫秒)
         */
        HttpServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs = 60000);
        /**
         * @brief: 获取Servlet函数
         * @return: 返回Servlet
         */
        const Safe<HttpServletDispatch>& getHttpServletDispatch();
    protected:
        /**
         * @brief: Socket接收函数
         */
        void accept() override;
        /**
         * @brief: Socket处理函数
         */
        void handleFunc(const Share<Socket>& socket) override;
    private:
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
}
} 