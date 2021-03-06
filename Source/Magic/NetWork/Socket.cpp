/*
 * @Author: INotFound
 * @Date: 2020-03-13 22:22:28
 * @LastEditTime: 2020-09-30 21:18:59
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Socket.h"

namespace Magic{
namespace NetWork{
    Socket::~Socket(){
    }
    
    Socket::Socket(uint64_t timeOutMs,uint64_t bufferSize,asio::io_context& context)
        :m_TimeOutMs(timeOutMs)
        ,m_BufferSize(bufferSize)
        ,m_ByteBlock(new char[m_BufferSize],[](const char *pointer) {delete[] pointer;})
        ,m_Socket(std::make_shared<asio::ip::tcp::socket>(context)){
        m_StreamBuffer.reserve(m_BufferSize);
        m_ErrorCodeCallBack = [](const asio::error_code & err){
            MAGIC_DEBUG() << err.message();
        };
    }

    const Safe<asio::ip::tcp::socket>& Socket::getEntity(){
        return m_Socket;
    }

    void Socket::send(const char* data,uint64_t length,const SendCallBack& callback){
        auto self = this->shared_from_this();
        asio::async_write(*m_Socket
        ,asio::const_buffer(data,length)
        ,std::bind([this,self](const asio::error_code &err, std::size_t length,const SendCallBack& callback){
            if(err){
                m_ErrorCodeCallBack(err);
            }
            if(callback){
                callback();
            }
        },std::placeholders::_1,std::placeholders::_2,callback));
    }

    void Socket::send(const Safe<asio::streambuf>& stream,const SendCallBack& callback){
        auto self = this->shared_from_this();
        asio::async_write(*m_Socket
                ,*stream
                ,std::bind([this,self,stream](const asio::error_code &err, std::size_t length,const SendCallBack& callback){
                    if(err){
                        m_ErrorCodeCallBack(err);
                    }
                    if(callback){
                        callback();
                    }
                },std::placeholders::_1,std::placeholders::_2,callback));
    }

    void Socket::recv(const RecvCallBack& callBack){
        auto self = this->shared_from_this();
        asio::async_read(*m_Socket
            ,asio::buffer(m_ByteBlock.get(),m_BufferSize)
            ,asio::transfer_at_least(1)
            ,std::bind([this,self](const asio::error_code &err, std::size_t length,const RecvCallBack& callback){
                if(err){
                    m_ErrorCodeCallBack(err);
                }else{
                    m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
                    callback(self,this->m_StreamBuffer);
                }
            },std::placeholders::_1,std::placeholders::_2,callBack));
    }

    void Socket::recv(uint64_t size,const RecvCallBack& callBack){
        auto self = this->shared_from_this();
        asio::async_read(*m_Socket
            ,asio::buffer(m_ByteBlock.get(),m_BufferSize)
            ,asio::transfer_exactly(size)
            ,std::bind([this,self](const asio::error_code &err, std::size_t length,const RecvCallBack& callback){
                if(err){
                    m_ErrorCodeCallBack(err);
                }else{
                    m_StreamBuffer.insert(m_StreamBuffer.end(),m_ByteBlock.get(),m_ByteBlock.get() + length);
                    callback(self,this->m_StreamBuffer);
                }
            },std::placeholders::_1,std::placeholders::_2,callBack));
    }

    void Socket::setErrorCodeCallBack(const ErrorCallBack& errorCallBack){
        m_ErrorCodeCallBack = errorCallBack;
    }
}
}
