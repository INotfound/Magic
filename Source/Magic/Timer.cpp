/*
 * @file: Timer.cpp
 * @Author: INotFound
 * @Date: 2020-03-13 21:37:59
 * @LastEditTime: 2020-03-14 00:14:52
 */
#include "Timer.h"
#include <chrono>
namespace Magic{
    Timer::Timer(const std::string& name,uint32_t tickMs,const std::function<void()>& callBack)
        :m_Name(name)
        ,m_MilliSeconds(tickMs)
        ,m_CallBack(std::move(callBack)){
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