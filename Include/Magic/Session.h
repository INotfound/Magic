#pragma once

#include "Core.h"

namespace Magic{
    class Session {
    public:
        virtual ~Session();
        Session(asio::io_context& io);
        Safe<Socket>& socket();
    protected:
        Safe<Socket> m_Socket;
    };
}