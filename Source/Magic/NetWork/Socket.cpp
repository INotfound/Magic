/*
 * @Author: INotFound
 * @Date: 2020-03-13 22:22:28
 * @LastEditTime: 2020-09-30 21:18:59
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Socket.h"

namespace Magic{
namespace NetWork{
    Socket::~Socket() =default;

    Socket::Socket(uint64_t timeOutMs,uint64_t bufferSize,asio::io_context& context,const Safe<TimingWheel>& timingWheel)
        :m_TimeOut(true)
        ,m_TimeOutMs(timeOutMs)
        ,m_BufferSize(bufferSize)
        ,m_ByteBlock(new char[m_BufferSize],std::default_delete<char[]>())
        ,m_TimingWheel(timingWheel)
        ,m_Socket(std::make_shared<asio::ip::tcp::socket>(context)){
        m_StreamBuffer.reserve(m_BufferSize);
        m_ErrorCodeCallBack = [](const asio::error_code & err){
            MAGIC_WARN() << err.message();
        };
    }

    void Socket::enableTimeOut(){
        auto self = this->shared_from_this();
        Safe<ITaskNode> taskNode = std::make_shared<FunctionTaskNode>([self](){
            if(self->m_TimeOut){
                if(self){
                    if(self->m_TimeOutCallBack){
                        self->m_TimeOutCallBack(self);
                    }
                    if(self->getEntity()->is_open()){
                        self->getEntity()->shutdown(asio::ip::tcp::socket::shutdown_both);
                    }
                    self->getEntity()->close();
                }
                return;
            }
            self->enableTimeOut();
            self->m_TimeOut = true;
        });
        m_TimingWheel->addTask(m_TimeOutMs,taskNode);
    }

    const Safe<asio::ip::tcp::socket>& Socket::getEntity(){
        return m_Socket;
    }
#ifdef OPENSSL
    const Safe<asio::ssl::stream<asio::ip::tcp::socket&>>& Socket::getSslEntity(){
        return m_SslStream;
    }

    void Socket::enableSsl(const Safe<asio::ssl::stream<asio::ip::tcp::socket&>>& sslStream){
        m_SslStream = sslStream;
    }
#endif
    void Socket::send(const char* data,uint64_t length,const SendCallBack& callback){
        auto self = this->shared_from_this();
        auto sendCallBack = std::bind([this,self](const asio::error_code &err, std::size_t length,const SendCallBack& callback){
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            m_TimeOut = false;
            if(callback){
                callback();
            }
        },std::placeholders::_1,std::placeholders::_2,callback);
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_write(*m_SslStream,asio::const_buffer(data,length),std::move(sendCallBack));
            return;
        }
    #endif
        asio::async_write(*m_Socket,asio::const_buffer(data,length),std::move(sendCallBack));
    }

    void Socket::send(const Safe<asio::streambuf>& stream,const SendCallBack& callback){
        auto self = this->shared_from_this();
        auto sendCallBack = std::bind([this,self,stream](const asio::error_code &err, std::size_t length,const SendCallBack& callback){
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            m_TimeOut = false;
            if(callback){
                callback();
            }
        },std::placeholders::_1,std::placeholders::_2,callback);
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_write(*m_SslStream,*stream,std::move(sendCallBack));
            return;
        }
    #endif
        asio::async_write(*m_Socket,*stream,std::move(sendCallBack));
    }

    void Socket::recv(const RecvCallBack& callBack){
        auto self = this->shared_from_this();
        auto readCallBack = std::bind([this,self](const asio::error_code &err, std::size_t length,const RecvCallBack& callback){
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            m_TimeOut = false;
            m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
            callback(self,this->m_StreamBuffer);
        },std::placeholders::_1,std::placeholders::_2,callBack);
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_read(*m_SslStream,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_at_least(1),std::move(readCallBack));
            return;
        }
    #endif
        asio::async_read(*m_Socket,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_at_least(1),std::move(readCallBack));
    }

    void Socket::recv(uint64_t size,const RecvCallBack& callBack){
        auto self = this->shared_from_this();
        auto readCallBack = std::bind([this,self](const asio::error_code &err, std::size_t length,const RecvCallBack& callback){
            if(err) {
                m_ErrorCodeCallBack(err);
                return;
            }
            m_TimeOut = false;
            m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
            callback(self,this->m_StreamBuffer);
        },std::placeholders::_1,std::placeholders::_2,callBack);
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_read(*m_SslStream,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_exactly(size),std::move(readCallBack));
            return;
        }
    #endif
        asio::async_read(*m_Socket,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_exactly(size),std::move(readCallBack));
    }

    void Socket::setErrorCodeCallBack(const ErrorCallBack& errorCallBack){
        m_ErrorCodeCallBack = errorCallBack;
    }

    void Socket::setTimeOutCallBack(const TimeOutCallBack& timeOutCallBack){
        m_TimeOutCallBack = timeOutCallBack;
    }
}
}
