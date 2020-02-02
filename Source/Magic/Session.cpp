#include "Session.h"

namespace Magic{
    Session::~Session(){
        m_Socket->close();
    }
    Session::Session(asio::io_context& io){
        m_Socket.reset(new Socket{io});
    }
    Safe<Socket>& Session::socket() {
        return m_Socket;
    }
}