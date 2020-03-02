#pragma once
#include "Core.h"
#include "TcpServer.h"
#include "Http/HttpServlet.h"

namespace Magic{
namespace Http{
    class HttpServer: public TcpServer{
    public:
        HttpServer(const std::string& addr,uint16_t port,uint32_t threadCount,uint64_t timeOutMs = 60000);
        const Safe<HttpServletDispatch>& getHttpServletDispatch();
    protected:
        void accept() override;
        void handleFunc(const Share<Socket>& socket) override;
    private:
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
}
} 