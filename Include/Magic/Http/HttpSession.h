#pragma once
#include "Core.h"
#include "Session.h"
namespace Magic{
namespace Http{
    class HttpSession :public Session {
    public:
        ~HttpSession();
        HttpSession(asio::io_context& io);
    };
}
}