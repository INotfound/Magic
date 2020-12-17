/*
 * @File: WebSocketServer.h
 * @Author: INotFound
 * @Date: 2020-04-06 17:27:52
 * @LastEditTime: 2020-04-06 17:28:40
 */
#pragma once
#include "../Core.h"
#include "../TcpServer.h"

namespace Magic{
namespace Http{
    class WebSocketServer :public TcpServer{
    public:
        WebSocketServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs);
    protected:
        void accept() override;
        void handleFunc(const Share<Socket>& socket) override;
    };
}
}