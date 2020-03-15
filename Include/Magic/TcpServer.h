/*
 * @file: TcpServer.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:06:38
 * @LastEditTime: 2020-03-15 17:09:44
 */
#pragma once
#include "Core.h"
#include "IoPool.h"
#include "Socket.h"
namespace Magic{
    /**
     * @brief: TcpServer服务端类
     */
    class TcpServer{
    public:
        /**
         * @brief: 析构函数
         */
        virtual ~TcpServer();
        /**
         * @brief: 构造函数
         * @param addr Ip地址
         * @param port Ip端口
         * @param threadCount 线程数
         * @param timeOutMs Socket超时时间(毫秒)
         */
        TcpServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs);
        /**
         * @brief: TcpServer服务端运行函数
         */
        void run();
        /**
         * @brief: TcpServer服务端暂停函数
         */
        void stop();
    protected:
        /**
         * @brief: Socket接收函数
         */
        virtual void accept();
        /**
         * @brief: Socket处理函数
         */
        virtual void handleFunc(const Share<Socket>& socket) =0;
    protected:
        bool m_IsRun;
        uint64_t m_TimeOutMs;
        Safe<IoPool> m_IoPool;
        std::string m_Address;
        uint16_t m_NetworkPort;
        Safe<asio::ip::tcp::acceptor> m_Acceptor;
    };
}