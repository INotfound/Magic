#pragma once

#include "Core.h"

namespace Magic{
    class Session {
    public:
        virtual ~Session();
        Session(asio::io_context& io);
        MagicPtr<Socket>& socket();
    protected:
        MagicPtr<Socket> m_Socket{};
    };
}