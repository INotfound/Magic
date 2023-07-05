/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : IoPool.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once

#include "asio.hpp"
#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Config.hpp"
#include "Magic/Utilty/Thread.hpp"

namespace Magic{
namespace NetWork{
    /**
     * @brief 网络Io池类
     */
    class IoPool:public Noncopyable,public std::enable_shared_from_this<IoPool>{
    public:
        /**
         * @brief 析构函数
         */
        ~IoPool();

        /**
         * @brief 构造函数
         * @param configuration 配置器
         */
        explicit IoPool(const Safe<Config>& configuration);

        /**
         * @brief 网络Io池等待完成函数
         * @note 该函数在IoC配置后Class Application析构中默认调用.
         */
        void wait();

        /**
         * @brief 网络Io池终止函数
         */
        void stop();

        /**
         * @brief 外部模式
         * @warning IoC容器中配置文件中默认调用,非IoC环境请手动调用.
         */
        void externMode();

        /**
         * @brief 获取网络Io上下文函数
         * @return: 返回网络Io上下文
         */
        const Safe<asio::io_context>& get();

    private:
        uint32_t m_Next;
        std::mutex m_Mutex;
        uint32_t m_PoolSize;
        std::vector<Safe<Thread>> m_Threads;
        std::vector<Safe<asio::io_context>> m_IOService;
        std::vector<Safe<asio::executor_work_guard<asio::io_context::executor_type>>> m_IOServiceWork;
    };

    extern Safe<IoPool> g_IoPool;
}
}
