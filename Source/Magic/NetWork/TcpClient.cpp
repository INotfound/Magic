#include "Magic/NetWork/TcpClient.h"


namespace Magic{
namespace NetWork{
    TcpClient::TcpClient(const std::string_view& ip,uint16_t port)
        :m_Port(port)
        ,m_IpAddress(ip.data(),ip.size()){
        m_Socket = std::make_shared<Socket>(1000,4096,m_IOService);
    }

    void TcpClient::run(){
        m_IOService.run();
    }

    void TcpClient::close(){
        m_Socket->close();
    }

    void TcpClient::recv(const Socket::RecvCallBack& callBack){
        m_Socket->recv(callBack);
    }

    void TcpClient::connect(const std::function<void()>& callback){
        asio::error_code errorCode;
        asio::ip::tcp::resolver resolver(m_IOService);
        auto results = resolver.resolve(m_IpAddress,AsString(m_Port),errorCode);
        if(errorCode){
            auto& errorCallBack = m_Socket->getErrorCodeCallBack();
            if(errorCallBack)
                errorCallBack(errorCode);
            return;
        }
        m_Socket->getEntity()->async_connect(results->endpoint(),[this,callback](const asio::error_code& errorCode){
            if(errorCode){
                if(errorCode == asio::error::eof || errorCode == asio::error::operation_aborted)
                    return;
                auto& errorCallBack = m_Socket->getErrorCodeCallBack();
                if(errorCallBack)
                    errorCallBack(errorCode);
                return;
            }
            m_Socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
            if(callback)
                callback();
        });
    }

    void TcpClient::setErrorCodeCallBack(const Socket::ErrorCallBack& errorCallBack){
        m_Socket->setErrorCodeCallBack(errorCallBack);
    }

    void TcpClient::send(const char* data,uint64_t length,const Socket::SendCallBack& callback){
        m_Socket->send(data,length,callback);
    }

    void TcpClient::send(const Safe<asio::streambuf>& stream,const Socket::SendCallBack& callback){
        m_Socket->send(stream,callback);
    }

}
}
