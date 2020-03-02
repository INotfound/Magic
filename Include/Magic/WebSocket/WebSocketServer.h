#pragma once
#include "Core.h"
#include "TcpServer.h"

namespace Magic{
namespace WebSocket{
    class WebSocketServer :public TcpServer{
    public:
        WebSocketServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs);
    protected:
        void accept() override;
        void handleFunc(const Share<Socket>& socket) override;
    private:
        void handleShake(const Share<Socket>& socket);
        void handleWSFrame(const Share<Socket>& socket);
        void handleWSFrameData(const Share<Socket>& socket,bool isContinue,uint64_t length);
    };
}
}