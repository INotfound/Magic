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
        MagicPtr<HttpServletDispatch>& getHttpServletDispatch();
    protected:
        void accept() override;
        void handleFunc(std::shared_ptr<Session> session) override;
    private:
        void process(std::shared_ptr<HttpSession> session);
    private:
        MagicPtr<HttpServletDispatch> m_ServletDispatch;
    };
}
} 