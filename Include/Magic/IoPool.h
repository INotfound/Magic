/*
 * @File: IoPool.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:57:15
 * @LastEditTime: 2020-03-14 01:40:18
 */
#pragma once
#include <atomic>
#include "Core.h"

namespace Magic{
    /**
     * @brief: 网络Io池类
     */
    class IoPool{
    public:
        /**
         * @brief: 析构函数
         * @note: 
         * @return: 
         */
        ~IoPool();
        /**
         * @brief: 构造函数
         * @param poolSize Io池大小
         */
        explicit IoPool(uint32_t poolSize);
        /**
         * @brief: 网络Io池运行函数
         */
        void run();
        /**
         * @brief: 网络Io池暂停函数
         */
        void stop();
        /**
         * @brief: 获取Io上下文函数
         * @return: 返回Io上下文
         */
        asio::io_context& get();
    private:
        uint32_t m_Next;
        uint32_t m_PoolSize;
        std::vector<Safe<asio::io_context>> m_IOService;
        std::vector<Safe<asio::io_context::work>> m_IOServiceWork;
    };
}