#include "Socket.h"
namespace Magic{
    Socket::~Socket(){
    }
    Socket::Socket(uint64_t bufferSize,uint64_t timeOutMs,asio::io_context& context)
        :m_IsTimeOut(false)
        ,m_TimeOutMs(timeOutMs)
        ,m_TimeOutState(State::Ready)
        ,m_Socket(new asio::ip::tcp::socket(context)){
        m_Data.reserve(bufferSize);
        TimingWheel::GetInstance()->run();
    }
    void Socket::clearData(){
        m_Data.clear();
    }
    void Socket::useTimeOut(){
        if(m_IsTimeOut){
            return;
        }
        m_IsTimeOut = true;
        Safe<ITaskNode> node(new SocketTimeOutTask(this->shared_from_this()));
        TimingWheel::GetInstance()->addTask(m_TimeOutMs,node);
    }
    void Socket::updateTimeOut(){
        m_TimeOutState = State::Reset;
    }
    std::vector<char>& Socket::getData(){
        return m_Data;
    }
    const Safe<asio::ip::tcp::socket>& Socket::getEntity() const{
        return m_Socket;
    }
    Socket::SocketTimeOutTask::SocketTimeOutTask(const Share<Socket>& socket)
        :m_Socket(socket){
    }
    void Socket::SocketTimeOutTask::notify(){
        m_Socket->timeOut();
    }
    void Socket::timeOut(){
        if(m_TimeOutState == State::Ready){
            m_Socket->close();
        }else if(m_TimeOutState == State::Reset){
            m_TimeOutState = State::Ready;
            Safe<ITaskNode> node(new SocketTimeOutTask(this->shared_from_this()));
            TimingWheel::GetInstance()->addTask(m_TimeOutMs,node);
        }
    }
}