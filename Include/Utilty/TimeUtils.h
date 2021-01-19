/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:45:32
 * @LastEditTime: 2020-09-12 16:46:15
 */
#pragma once
#include <stdint.h>

namespace Magic{
    /**
     * @brief: 获取当前时间
     * @return: 返回当前时间
     */
    uint64_t GetCurrentTimeMS();
    /**
     * @brief: 获取当前时间
     * @return: 返回当前时间
     */
    uint64_t GetCurrentTimeUS();
}