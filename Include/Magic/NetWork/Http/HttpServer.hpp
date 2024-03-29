/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpServer.hpp
 * @Date           : 2023-07-03 18:28
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/TcpServer.hpp"
#include "Magic/NetWork/Http/HttpServlet.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief Http服务端类
     */
    class HttpServer:public TcpServer{
    public:
        /**
         * @brief 构造函数
         * @param pool 网络池
         * @param configuration 配置器
         */
        HttpServer(const Safe<IoPool>& pool,const Safe<Config>& configuration);

        /**
         * @brief 配置分发器后启动服务器
         * @param dispatch Servlet分发器
         */
        void setServletDispatch(const Safe<HttpServletDispatch>& dispatch);

    protected:
        /**
         * @brief Socket接收函数
         */
        void accept() override;

        /**
         * @brief Socket处理函数
         */
        void handleFunc(const Safe<Socket>& socket) override;

    private:
        bool m_EnableSsl;
        std::string m_KeyFile;
        std::string m_CertFile;
        std::string m_UploadDirectory;
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
}
}
}
