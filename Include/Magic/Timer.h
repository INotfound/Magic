#pragma once
#include <functional>
#include "Core.h"
#include "Thread.h"
namespace Magic{
    class Timer{
    public:
        Timer(const std::string& name,uint32_t ms,std::function<void()> func);
        void run();
        void stop();
    private:
        void handle(const asio::error_code& err);
    private:
        std::string m_Name;
        Safe<Thread> m_Thread;
        uint32_t m_MilliSeconds;
        Safe<asio::io_context> m_Io;
        Safe<asio::steady_timer> m_Time;
        std::function<void()> m_CallBack;
        Safe<asio::io_context::work> m_IoWork;
    };
}