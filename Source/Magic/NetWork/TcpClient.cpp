/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : TcpClient.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/NetWork/TcpClient.hpp"


namespace Magic{
namespace NetWork{
    TcpClient::TcpClient(const Magic::StringView& ip,uint16_t port)
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

    void TcpClient::recv(Socket::RecvCallBack callBack){
        m_Socket->recv(callBack);
    }

    void TcpClient::connect(std::function<void()> callBack){
        asio::error_code errorCode;
        asio::ip::tcp::resolver resolver(m_IOService);
        auto results = resolver.resolve(m_IpAddress,AsString(m_Port),errorCode);
        if(errorCode){
            auto& errorCallBack = m_Socket->getErrorCodeCallBack();
            if(errorCallBack)
                errorCallBack(errorCode);
            return;
        }
    #if __cplusplus >= 201402L
        m_Socket->getEntity()->async_connect(results->endpoint(),[this,callBack = std::move(callBack)](const asio::error_code& errorCode){
    #else
        m_Socket->getEntity()->async_connect(results->endpoint(),[this,callBack](const asio::error_code& errorCode){
    #endif
            if(errorCode){
                if(errorCode == asio::error::eof || errorCode == asio::error::operation_aborted)
                    return;
                auto& errorCallBack = m_Socket->getErrorCodeCallBack();
                if(errorCallBack)
                    errorCallBack(errorCode);
                return;
            }
            m_Socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
            if(callBack)
                callBack();
        });
    }

    void TcpClient::setErrorCodeCallBack(Socket::ErrorCallBack errorCallBack){
        m_Socket->setErrorCodeCallBack(std::move(errorCallBack));
    }

    void TcpClient::send(const IStream::BufferView& buffer,Socket::SendCallBack callBack){
        m_Socket->send(buffer,std::move(callBack));
    }

    void TcpClient::send(const Safe<asio::streambuf>& stream,Socket::SendCallBack callBack){
        m_Socket->send(stream,std::move(callBack));
    }

}
}
