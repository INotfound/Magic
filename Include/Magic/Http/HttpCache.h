/*
 * @file: HttpCache.h
 * @Author: INotFound
 * @Date: 2020-03-15 16:50:11
 * @LastEditTime: 2020-03-15 16:52:49
 */
#pragma once
#include <unordered_map>
#include "Core.h"
#include "Macro.h"
#include "Mutex.h"
#include "Http/Http.h"
namespace Magic{
namespace Http{
namespace Instance{
    /**
     * @brief: Http资源缓存类
     */
    class HttpCache{
    public:
        /**
         * @brief: 构造函数
         */
        HttpCache();
        /**
         * @brief: 初始化资源函数
         * @param resourcePath 初始化资源路径
         */
        void Init(const std::string& resourcePath);
        /**
         * @brief: 获取资源缓存函数
         * @param path 资源路径
         * @return: 返回资源内容缓存
         */
        const std::string& getCache(const std::string& path);
    private:
        bool m_IsInit;
        RWMutex m_Mutex;
        std::string m_ResourcePath;
        std::unordered_map<std::string,std::string> m_Resource;
    };
}
    typedef SingletonPtr<Instance::HttpCache> HttpStaticCache;
}
}
