/*
 * @Author: INotFound
 * @Date: 2020-03-15 17:14:06
 * @LastEditTime: 2021-02-01 22:21:19
 */
#pragma once
#include <unordered_map>

#include "Core/Any.h"
#include "Core/Core.h"
#include "Utilty/Mutex.h"
#include "Utilty/Config.h"
#include "Utilty/TimingWheel.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief: Http会话类
     */
    class HttpSession{
    public:
        /**
         * @brief: 析构函数
         */
        ~HttpSession();
        /**
         * @brief: 构造函数
         * @param autoGen 是否自动生成相关信息
         */
        HttpSession(bool autoGen =true);
        /**
         * @brief: 获取会话Id函数
         * @return: 返回会话Id
         */
        const std::string& getId();
        /**
         * @brief: 设置会话Id函数
         * @param id 会话Id
         */
        void setId(const std::string& id);
        /**
         * @brief: 获取最后一次使用时间函数
         * @return: 返回最后一次使用时间
         */
        uint64_t getLastAccessTime();
        /**
         * @brief: 设置最后一次使用时间函数
         * @param value 会话时间
         */
        void setLastAccessTime(uint64_t value);
        /**
         * @brief: 获取会话数据函数
         * @param key 数据Key
         * @return: 返回会话数据
         */
        template<class T>
        const T& getData(const std::string& key){
            RWMutex::ReadLock lock(m_Mutex);
            static const T empty = T();
            auto value = m_Data.find(key);
            if(value == m_Data.end()){
                return empty;
            }
            //Here may throw exceptions
            return std::any_cast<T>(value->second);
        }
        /**
         * @brief: 设置会话数据函数
         * @param key 数据Key
         * @param value 数据值
         */
        template<class T>
        void setData(const std::string& key,const T& value){
            RWMutex::WriteLock lock(m_Mutex);
            m_Data.emplace(key,value);
        }
    private:
        RWMutex m_Mutex;
        std::string m_Id;
        uint64_t m_LastAccessTime;
        std::unordered_map<std::string,std::any> m_Data;
    };
    /**
     * @brief: Http会话管理类
     */
    class HttpSessionManager :public std::enable_shared_from_this<HttpSessionManager>{
    public:
        /**
         * @brief: 构造函数
         */
        HttpSessionManager(const Safe<Config>& configuration);
        /**
         * @brief: 删除会话函数
         * @param id 会话Id
         */
        void del(const std::string& id);
        /**
         * @brief: 设置会话超时时间函数
         * @param timeOutMs 超时时间(毫秒)
         */
        void setTimeOut(uint64_t timeOutMs);
        /**
         * @brief: 添加会话函数
         * @param HttpSession 会话对象
         */
        void add(Safe<HttpSession>& httpSession);
        /**
         * @brief: 获取会话函数
         * @param key 会话Id
         * @return: 返回会话对象
         */
        const Safe<HttpSession>& get(const std::string& id);
        /**
         * @brief: 添加定时轮,处理过期的会话
         * @param timingWheel 定时轮
         */
        void setTimingWheel(const Safe<TimingWheel>& timingWheel);
    private:
        RWMutex m_Mutex;
        uint64_t m_TimeOutMs;
        Safe<TimingWheel> m_TimingWheel;
        std::unordered_map<std::string,Safe<HttpSession>> m_Data;
    };

}
}
}