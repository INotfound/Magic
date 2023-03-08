/*
 * @Author: INotFound
 * @Date: 2021-01-08 13:41:01
 * @LastEditTime: 2021-01-12 08:08:19
 */
#pragma once

#include "Magic/Core/Core.h"
#include "Magic/Utilty/Config.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/TimingWheel.h"

namespace Magic{
namespace DataBase{
    template<typename T>
    class Connection;

    template<typename T>
    class ConnectionPool;

    template<typename T>
    class Connection:public Noncopyable{
        friend class ConnectionPool<T>;
    public:
        ~Connection(){
            if(m_Entity){
                m_Pool->restore(std::move(m_Entity));
            }
        }

        Connection() = default;

        explicit operator bool() const{
            if(m_Entity){
                return true;
            }
            return false;
        }

        T& operator*() const{
            return *m_Entity;
        }

    private:
        Connection(const Safe<T>&& entity,const Safe<ConnectionPool<T>>& pool)
            :m_Entity(entity)
            ,m_Pool(pool){
        }

    private:
        Safe<T> m_Entity;
        Safe<ConnectionPool<T>> m_Pool;
    };

    template<typename T>
    class ConnectionPool:public Noncopyable,public std::enable_shared_from_this<ConnectionPool<T>>{
        friend class Connection<T>;
    public:
        explicit ConnectionPool(const Safe<Magic::Config>& configuration)
            :m_Count(configuration->at("DataBase.Connection.Count",2)){
        }

        /**
         * @brief 从连接池中获取单个连接
         * @return Connection<T>单个连接
         * @warning Use Exception!
         */
        Connection<T> getConnection(){
            std::lock_guard<std::mutex> locker(m_Mutex);
            if(m_IdleEntity.empty()){
                Safe<T> connection;
                if(m_CreateFunction)
                    connection = m_CreateFunction();
                if(connection)
                    return Connection<T>(std::move(connection),this->shared_from_this());
                throw Failure("Connection Pool No Idle Entity!");
            }
            Safe<T> entity = m_IdleEntity.front();
            m_IdleEntity.pop_front();
            return Connection<T>(std::move(entity),this->shared_from_this());;
        }

        void initialize(const std::function<const Safe<T>(void)>& initFunc){
            std::lock_guard<std::mutex> locker(m_Mutex);
            m_CreateFunction = std::move(initFunc);
            for(uint32_t i = 0;i < m_Count;i++){
                const Safe<T> connection = m_CreateFunction();
                if(connection){
                    m_IdleEntity.push_back(connection);
                }
            }
        }

    private:
        void restore(const Safe<T>&& entity){
            std::lock_guard<std::mutex> locker(m_Mutex);
            m_IdleEntity.emplace_back(entity);
        }

    private:
        uint32_t m_Count;
        std::mutex m_Mutex;
        std::deque<Safe<T>> m_IdleEntity;
        std::function<const Safe<T>(void)> m_CreateFunction;
    };
}
}
