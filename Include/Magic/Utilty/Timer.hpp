/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Timer.hpp
 * @Date           : 2023-07-03 18:30
 ******************************************************************************
 */
#pragma once

#include <functional>

#include "asio.hpp"
#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Thread.hpp"

namespace Magic{
    /**
     * @brief 定时器类
     */
    class Timer:public Noncopyable{
    public:
        ~Timer();

        /**
         * @brief 构造函数
         * @param name 定时器名称
         * @param tickMs 定时器执行时间(毫秒)
         * @param callBack 定时器回调执行函数
         */
        Timer(const StringView& name,uint32_t tickMs,std::function<void()> callBack);

        /**
         * @brief 运行定时器
         */
        void run();

        /**
         * @brief 暂停定时器
         */
        void stop();

    private:
        /**
         * @brief 定时器处理函数
         * @param err 错误码
         */
        void handle(const asio::error_code& err);

    private:
        std::string m_Name;
        Safe<Thread> m_Thread;
        uint32_t m_MilliSeconds;
        Safe<asio::steady_timer> m_Time;
        std::function<void()> m_CallBack;
        Safe<asio::io_context> m_IoContext;
        Safe<asio::executor_work_guard<asio::io_context::executor_type>> m_IoWork;
    };
}
