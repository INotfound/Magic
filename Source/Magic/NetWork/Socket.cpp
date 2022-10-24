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

    Socket::Socket(uint64_t heartBeatMs,uint64_t bufferSize,asio::io_context& context)
        :m_BufferSize(bufferSize)
        ,m_HeartBeatMs(heartBeatMs)
        ,m_ByteBlock(new char[m_BufferSize],std::default_delete<char[]>())
        ,m_Socket(std::make_shared<asio::ip::tcp::socket>(context)){
        m_StreamBuffer.reserve(m_BufferSize);
        m_ErrorCodeCallBack = [](const asio::error_code & err){
            MAGIC_WARN() << err.message();
        };
    }

    void Socket::close() {
        asio::error_code ignored;
        Mutex::Lock lock(m_Mutex);
        if(m_Socket->is_open()) {
            m_Socket->cancel(ignored);
            m_Socket->shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
        }
        m_Socket->close(ignored);
    }

    void Socket::setHeartBeatTime(uint64_t ms){
        m_HeartBeatMs = ms;
    }

    void Socket::runHeartBeat(const Safe<void>& life){
        auto self = this->shared_from_this();
        Safe<ITaskNode> taskNode = std::make_shared<FunctionTaskNode>([self,life](){
            if(self->getEntity() && self->m_HeartBeatCallBack)
                self->m_HeartBeatCallBack(self);
        });
        if(g_TimingWheel)
            g_TimingWheel->addTask(m_HeartBeatMs, taskNode);
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
    void Socket::send(const char* data,uint64_t length,const SendCallBack callback){
        auto sendCallBack = [this,callback](const asio::error_code &err, std::size_t /*length*/,const SendCallBack callback){
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            if(callback)
                callback();
        };
        Mutex::Lock lock(m_Mutex);
        if(!m_Socket->is_open()){
            return;
        }
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_write(*m_SslStream,asio::const_buffer(data,length),std::move(sendCallBack));
            return;
        }
    #endif
        asio::async_write(*m_Socket,asio::const_buffer(data,length),std::move(sendCallBack));
    }

    void Socket::send(const Safe<asio::streambuf>& stream,const SendCallBack callback){
        auto sendCallBack = [this,stream,callback](const asio::error_code &err, std::size_t /*length*/,const SendCallBack callback){
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            if(callback)
                callback();
        };
        Mutex::Lock lock(m_Mutex);
        if(!m_Socket->is_open()){
            return;
        }
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_write(*m_SslStream,*stream,std::move(sendCallBack));
            return;
        }
    #endif
        asio::async_write(*m_Socket,*stream,std::move(sendCallBack));
    }

    void Socket::recv(const RecvCallBack callBack){
        auto readCallBack = [this,callBack](const asio::error_code &err, std::size_t length,const RecvCallBack callback){
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
            callback(this->m_StreamBuffer);
        };
        Mutex::Lock lock(m_Mutex);
        if(!m_Socket->is_open()){
            return;
        }
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_read(*m_SslStream,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_at_least(1),std::move(readCallBack));
            return;
        }
    #endif
        asio::async_read(*m_Socket,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_at_least(1),std::move(readCallBack));
    }

    void Socket::recv(uint64_t size,const RecvCallBack callBack){
        auto readCallBack = [this,size,callBack](const asio::error_code &err, std::size_t length,const RecvCallBack callback){
            if(err) {
                m_ErrorCodeCallBack(err);
                return;
            }
            m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
            if(size > length){
                this->recv(size - length,callback);
                return;
            }
            callback(this->m_StreamBuffer);
        };
        Mutex::Lock lock(m_Mutex);
        if(!m_Socket->is_open()){
            return;
        }
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_read(*m_SslStream,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_at_least(size),std::move(readCallBack));
            return;
        }
    #endif
        asio::async_read(*m_Socket,asio::buffer(m_ByteBlock.get(),m_BufferSize),asio::transfer_at_least(size),std::move(readCallBack));
    }

    const Socket::ErrorCallBack& Socket::getErrorCodeCallBack() const{
        return m_ErrorCodeCallBack;
    }

    void Socket::setErrorCodeCallBack(const ErrorCallBack errorCallBack){
        m_ErrorCodeCallBack = std::move(errorCallBack);
    }

    void Socket::setHeartBeatCallBack(const HeartBeatCallBack heartBeatCallBack){
        m_HeartBeatCallBack = std::move(heartBeatCallBack);
    }
}
}
