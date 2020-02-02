#pragma once
#include "Core.h"
#include "TcpServer.h"
#include "Http/HttpServlet.h"
#include "Http/HttpSession.h"

namespace Magic{
namespace Http{
    class HttpServer: public TcpServer{
    public:
        HttpServer(const std::string& addr,uint16_t port,uint32_t threadCount);
        Safe<HttpServletDispatch>& getHttpServletDispatch();
    protected:
        void accept() override;
        void handleFunc(Share<Session> session) override;
    private:
        void process(Share<HttpSession> session);
    private:
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
}
} 