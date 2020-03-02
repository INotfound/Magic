#include "Timer.h"
#include <chrono>
namespace Magic{
    Timer::Timer(const std::string& name,uint32_t ms,std::function<void()> func)
        :m_Name(name)
        ,m_MilliSeconds(ms)
        ,m_CallBack(func){
        m_Io.reset(new asio::io_context);
        m_Time.reset(new asio::steady_timer(*m_Io));
    }
    void Timer::run(){
        m_IoWork.reset(new asio::io_context::work(*m_Io));
        m_Time->expires_from_now(std::chrono::milliseconds(m_MilliSeconds));
        m_Thread.reset(new Thread(m_Name,[this](){
            m_Time->async_wait(std::bind(&Timer::handle,this,std::placeholders::_1));
            m_Io->run();
        }));
        m_Thread->detach();
    }
    void Timer::stop(){
        m_Time->cancel();
        m_IoWork.reset();
    }
    void Timer::handle(const asio::error_code& err){
        if(err) return;
        m_CallBack();
        m_Time->expires_from_now(std::chrono::milliseconds(m_MilliSeconds));
        m_Time->async_wait(std::bind(&Timer::handle,this,std::placeholders::_1));
    }
}