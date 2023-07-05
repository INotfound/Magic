/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Socket.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Utilty/Logger.hpp"
#include "Magic/NetWork/Socket.hpp"

namespace Magic{
namespace NetWork{
    Socket::~Socket() = default;

    Socket::Socket(uint64_t heartBeatMs,uint64_t bufferSize,asio::io_context& context)
        :m_BufferSize(bufferSize)
        ,m_HeartBeatMs(heartBeatMs)
        ,m_ByteBlock(new char[m_BufferSize],std::default_delete<char[]>())
        ,m_Working(false)
        ,m_Socket(std::make_shared<asio::ip::tcp::socket>(context)){
        m_StreamBuffer.reserve(m_BufferSize);
        m_ErrorCodeCallBack = [](const asio::error_code& err){
            MAGIC_WARN() << err.message();
        };
    }

    void Socket::close(){
        asio::error_code ignored;
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(m_Socket->is_open()){
            m_Socket->cancel(ignored);
            m_Socket->shutdown(asio::ip::tcp::socket::shutdown_both,ignored);
        }
        m_Socket->close(ignored);
    }

    bool Socket::isWorking() const{
        return m_Working;
    }

    void Socket::runHeartBeat(Safe<void> life){
        auto self = this->shared_from_this();
    #if __cplusplus >= 201402L
        Safe<ITaskNode> taskNode = std::make_shared<FunctionTaskNode>([self = std::move(self),life = std::move(life)](){
    #else
        Safe<ITaskNode> taskNode = std::make_shared<FunctionTaskNode>([self,life](){
    #endif
            if(self->getEntity() && self->m_HeartBeatCallBack)
                self->m_HeartBeatCallBack(self);
        });
        if(g_TimingWheel){
            g_TimingWheel->addTask(m_HeartBeatMs,taskNode);
        }else{
            throw Failure("Heartbeat Detection Not Working Because IoPool Not Call ExternMode");
        }
    }

    const Safe<asio::ip::tcp::socket>& Socket::getEntity(){
        return m_Socket;
    }

#ifdef OPENSSL
    const Safe<asio::ssl::stream<asio::ip::tcp::socket&>>& Socket::getSslEntity(){
        return m_SslStream;
    }

    void Socket::enableSsl(const Magic::StringView& keyPath,const Magic::StringView& certPath){
        asio::ssl::context sslContext(asio::ssl::context::sslv23);
        if(!keyPath.empty() && !certPath.empty()){
            sslContext.set_options(asio::ssl::context::no_sslv2 | asio::ssl::context::single_dh_use | asio::ssl::context::default_workarounds);
            sslContext.use_certificate_chain_file(std::string(keyPath.data(),keyPath.size()));
            sslContext.use_private_key_file(std::string(certPath.data(),certPath.size()),asio::ssl::context::pem);
        }
        m_SslStream = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket&>>(*m_Socket,sslContext);
    }
#endif

    void Socket::recv(RecvCallBack callBack){
    #if __cplusplus >= 201402L
        auto readCallBack = [this,callBack = std::move(callBack)](const asio::error_code& err,std::size_t length){
    #else
        auto readCallBack = [this,callBack](const asio::error_code& err,std::size_t length){
    #endif
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
            if(callBack)
                callBack(this->m_StreamBuffer);
        };
        std::lock_guard<std::mutex> locker(m_Mutex);
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

    void Socket::recv(uint64_t size,RecvCallBack callBack){
    #if __cplusplus >= 201402L
        auto readCallBack = [this,size,callBack = std::move(callBack)](const asio::error_code& err,std::size_t length){
    #else
        auto readCallBack = [this,size,callBack](const asio::error_code& err,std::size_t length){
    #endif
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
            if(size > length){
                this->recv(size - length,callBack);
                return;
            }
            if(callBack)
                callBack(this->m_StreamBuffer);
        };
        std::lock_guard<std::mutex> locker(m_Mutex);
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

    void Socket::setErrorCodeCallBack(ErrorCallBack errorCallBack){
        m_ErrorCodeCallBack = std::move(errorCallBack);
    }

    void Socket::setHeartBeatCallBack(HeartBeatCallBack heartBeatCallBack){
        m_HeartBeatCallBack = std::move(heartBeatCallBack);
    }

    void Socket::send(const char* data,uint64_t length,SendCallBack callBack){
    #if __cplusplus >= 201402L
        auto sendCallBack = [this,callBack = std::move(callBack)](const asio::error_code& err,std::size_t /*length*/){
    #else
        auto sendCallBack = [this,callBack](const asio::error_code& err,std::size_t /*length*/){
    #endif
            m_Working = false;
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            if(callBack)
                callBack();
        };
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!m_Socket->is_open()){
            return;
        }
        m_Working = true;
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_write(*m_SslStream,asio::const_buffer(data,length),std::move(sendCallBack));
            return;
        }
    #endif
        asio::async_write(*m_Socket,asio::const_buffer(data,length),std::move(sendCallBack));
    }

    void Socket::send(const Safe<asio::streambuf>& stream,SendCallBack callBack){
    #if __cplusplus >= 201402L
        auto sendCallBack = [this,stream,callBack = std::move(callBack)](const asio::error_code& err,std::size_t /*length*/){
    #else
        auto sendCallBack = [this,stream,callBack](const asio::error_code& err,std::size_t /*length*/){
    #endif
            m_Working = false;
            if(err){
                m_ErrorCodeCallBack(err);
                return;
            }
            if(callBack)
                callBack();
        };
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!m_Socket->is_open()){
            return;
        }
        m_Working = true;
    #ifdef OPENSSL
        if(m_SslStream){
            asio::async_write(*m_SslStream,*stream,std::move(sendCallBack));
            return;
        }
    #endif
        asio::async_write(*m_Socket,*stream,std::move(sendCallBack));
    }
}
}
