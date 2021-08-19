/*
 * @File: HttpServer.h
 * @Author: INotFound
 * @Date: 2020-03-15 17:05:03
 * @LastEditTime: 2020-10-01 00:48:33
 */
#pragma once
#include "Magic/Core/Core.h"
#include "Magic/NetWork/TcpServer.h"
#include "Magic/NetWork/Http/HttpServlet.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief: Http服务端类
     */
    class HttpServer :public TcpServer{
    public:
        /**
         * @brief: 构造函数
         * @param pool 网络池
         * @param configuration 配置器
         */
        HttpServer(const Safe<IoPool>& pool,const Safe<TimingWheel>& timingWheel,const Safe<Config>& configuration);
        /**
         * @brief: 配置分发器后启动服务器
         * @param dispatch Servlet分发器
         */
        void setServletDispatch(const Safe<HttpServletDispatch>& dispatch);
    protected:
        /**
         * @brief: Socket接收函数
         */
        void accept() override;
        /**
         * @brief: Socket处理函数
         */
        void handleFunc(const Safe<Socket>& socket) override;
    private:
        std::string m_TempDirectory;
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
}
}
}
