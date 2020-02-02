#include "Http/HttpSession.h"

namespace Magic{
namespace Http{
    HttpSession::~HttpSession(){
        m_Socket->close();
    }
    HttpSession::HttpSession(asio::io_context& io) 
        :Session{io}{
    }
}
}