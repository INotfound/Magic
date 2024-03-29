/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Thread.hpp
 * @Date           : 2023-07-03 18:30
 ******************************************************************************
 */
#pragma once

#include <thread>
#include <condition_variable>

#include "Magic/Core/Core.hpp"

namespace Magic{
    /**
     * @brief 线程类
     */
    class Thread:public Noncopyable{
    public:
        /**
         * @brief 析构函数
         */
        ~Thread();

        /**
         * @brief 构造函数
         * @param threadName 线程名称
         * @param callBack 回调函数
         */
        Thread(const StringView& threadName,std::function<void()> callBack);

        /**
         * @brief 等待线程执行函数
         */
        void join();

        /**
         * @brief 分离线程执行函数
         */
        void detach();

        /**
         * @brief 获取线程Id函数
         * @return: 返回线程Id,若返回-1,即线程未初始化
         */
        int64_t getId() const;

        /**
         * @brief 获取线程名称
         * @return: 返回线程名,若返回空,即线程未初始化
         */
        static StringView GetName();

        /**
         * @brief 设置线程名称
         * @param threadName 需设置的线程名称
         */
        static void SetName(const StringView& threadName);

    protected:
        /**
         * @brief 线程内部回调函数
         */
        void run();

    private:
        int64_t m_Id;
        std::string m_Name;
        Safe<std::thread> m_Thread;
        std::function<void()> m_CallBack;
    };
}
