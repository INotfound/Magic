/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : SystemUtils.hpp
 * @Date           : 2023-07-03 18:27
 ******************************************************************************
 */
#pragma once
#include <stdint.h>
namespace Magic{
    /**
     * @brief 获取线程Id
     * @return: 返回线程Id
     */
    uint64_t GetThreadId();
    /**
     * @brief 获取当前时间
     * @return: 返回当前时间
     */
    uint64_t GetCurrentTimeMS();

    /**
     * @brief 获取当前时间
     * @return: 返回当前时间
     */
    uint64_t GetCurrentTimeUS();
    /**
     * @brief 判断是否为文件
     * @param path 文件路径
     * @return: True 是 False 否
     */
    bool IsFile(const Magic::StringView& path);
}