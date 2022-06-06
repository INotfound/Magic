/*
 * @Author: INotFound
 * @Date: 2020-09-29 17:17:06
 * @LastEditTime: 2021-02-01 22:20:38
 */
#pragma once
#include "asio.hpp"
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Config.h"

namespace Magic{
namespace NetWork{
    /**
     * @brief 网络Io池类
     */
    class IoPool{
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
         * @brief 网络Io池运行函数
         */
        void run();
        /**
         * @brief 网络Io池暂停函数
         */
        void stop();
        /**
         * @brief 获取网络Io上下文函数
         * @return: 返回网络Io上下文
         */
        asio::io_context& get();
    private:
        uint32_t m_Next;
        uint32_t m_PoolSize;
        std::vector<Safe<asio::io_context>> m_IOService;
        std::vector<Safe<asio::executor_work_guard<asio::io_context::executor_type>>> m_IOServiceWork;
    };
}
}
