/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : TcpServer.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Config.hpp"
#include "Magic/NetWork/IoPool.hpp"
#include "Magic/NetWork/Socket.hpp"

namespace Magic{
namespace NetWork{
    /**
     * @brief TcpServer服务端类
     */
    class TcpServer:public Noncopyable{
    public:
        /**
         * @brief 析构函数
         */
        virtual ~TcpServer();

        /**
         * @brief 构造函数
         * @brief 设置网络Io池
         * @param configuration 配置器
         */
        TcpServer(const Safe<IoPool>& pool,const Safe<Config>& configuration);

        /**
         * @brief TcpServer服务端运行函数
         */
        void run();

        /**
         * @brief TcpServer服务端暂停函数
         */
        void stop();

    protected:
        /**
         * @brief Socket接收函数
         */
        virtual void accept();

        /**
         * @brief Socket处理函数
         */
        virtual void handleFunc(const Safe<Socket>& socket) = 0;

    protected:
        bool m_IsRun;
        uint64_t m_TimeOutMs;
        Safe<IoPool> m_IoPool;
        std::string m_Address;
        uint16_t m_NetworkPort;
        Safe<asio::ip::tcp::acceptor> m_Acceptor;
    };
}
}
