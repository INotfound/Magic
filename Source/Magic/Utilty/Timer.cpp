/*
 * @File: Timer.cpp
 * @Author: INotFound
 * @Date: 2020-03-13 21:37:59
 * @LastEditTime: 2020-10-02 22:24:52
 */
#include <chrono>

#include "Magic/Utilty/Timer.h"

namespace Magic{
    Timer::~Timer() {
        m_Time->cancel();
    }

    Timer::Timer(const std::string& name,uint32_t tickMs,std::function<void()> callBack)
        :m_Name(name)
        ,m_MilliSeconds(tickMs)
        ,m_CallBack(std::move(callBack))
        ,m_IoContext(std::make_shared<asio::io_context>()){
        m_Time = std::make_shared<asio::steady_timer>(*m_IoContext);
    }

    void Timer::run(){
        m_IoWork = std::make_shared<asio::executor_work_guard<asio::io_context::executor_type>>(asio::make_work_guard(*m_IoContext));
        m_Time->expires_from_now(std::chrono::milliseconds(m_MilliSeconds));
        m_Thread = std::make_shared<Thread>(m_Name,[this](){
            m_Time->async_wait([this](const asio::error_code& err){this->handle(err);});
            m_IoContext->run();
        });
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
        m_Time->async_wait([this](const asio::error_code& err){this->handle(err);});
    }
}
