#include "TcpServer.h"
#include "Macro.h"

namespace Magic{

    Session::Session(asio::io_context& io, uint32_t) 
        :m_Socket(io){

    }
    asio::ip::tcp::socket& Session::socket() {
        return m_Socket;
    }


    TcpServer::~TcpServer(){
    }
    TcpServer::TcpServer(const std::string& addr,uint16_t port,uint32_t threadCount)
        :m_Address{addr}
        ,m_NetworkPort{port}
        ,m_IoPool{new IoPool{threadCount}}{
        auto endpoint = asio::ip::tcp::endpoint{asio::ip::address::from_string(addr),port};
        m_Acceptor.reset(new asio::ip::tcp::acceptor{m_IoPool->get(),endpoint});
        m_Acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(1));
        m_Acceptor->listen();
    }
    void TcpServer::run(){
        if(m_Stop){
            return;
        }
        //TODO: ...
        accept();
        m_IoPool->run();
    }
    void TcpServer::stop(){
        //TODO: ...
        m_Stop = true;
    }
                
    char str[] = "HTTP/1.0 200 OK\r\n\r\n"
        "<html>hello from http server</html>";
    char strbuf[1024] = { 0 };
    void TcpServer::accept(){
        std::shared_ptr<Session> session{ new Session{m_IoPool->get(),20} };
        auto& socket = session->socket();
        m_Acceptor->async_accept(socket,[this, session = std::move(session)](const asio::error_code& err){
            MAGIC_LOG(Magic::LogLevel::LogInfo) << "accept";
            if(err){
                //TODO: ...
                MAGIC_LOG(Magic::LogLevel::LogWarn) << err.message();
                return;
            }
            
            session->socket().async_read_some(asio::buffer(strbuf,1024) ,[session = std::move(session)](const asio::error_code& err, size_t len) {
                printf("%llu", Magic::GetThreadId());
                auto& socket = session->socket();
                socket.async_write_some(asio::buffer(str), [session = std::move(session)](const asio::error_code& err, size_t len) {
                    if (err) {
                        //TODO: ...
                        MAGIC_LOG(Magic::LogLevel::LogWarn) << err.message();
                        return;
                    }
                    MAGIC_LOG(Magic::LogLevel::LogInfo) << "Post!";

                });
            });
            
            if(!m_Stop){
                accept();
            }
        });
    }

    void TcpServer::hanldeFunc(std::shared_ptr<asio::ip::tcp::socket> socket){
    }
}