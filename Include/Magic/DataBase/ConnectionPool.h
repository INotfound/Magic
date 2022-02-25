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
    template<class T>
    class Connection;
    template<class T>
    class ConnectionPool;

    template<class T>
    class Connection{
        friend class ConnectionPool<T>;
    public:
        ~Connection(){
            if(m_Entity){
                m_Pool->restore(m_Entity);
            }
        }
        Connection(){}
        explicit operator bool() const{
            if(m_Entity){
                return true;
            }
            return false;
        }
        const Safe<T>& operator *() const{
            return m_Entity;
        }
    private:
        Connection(const Safe<T>& entity,const Safe<ConnectionPool<T>>& pool)
            :m_Entity(entity)
            ,m_Pool(pool){
        }
    private:
        Safe<T> m_Entity;
        Safe<ConnectionPool<T>> m_Pool;
    };

    template<class T>
    class ConnectionPool :public std::enable_shared_from_this<ConnectionPool<T>>{
        friend class Connection<T>;
    public:
        ConnectionPool(const Safe<Magic::Config>& configuration)
            :m_Count(configuration->at("DataBase.Connection.Count",1)){
        }

        Connection<T> getConnection(){
            Magic::Mutex::Lock locker(m_Mutex);
            if(m_IdleEntity.empty()){
                const Safe<T> conn = m_InitFunction();
                if(conn){
                    m_IdleEntity.push_back(conn);
                }else{
                    return Connection<T>();
                }
            }
            Safe<T> entity = m_IdleEntity.front();
            m_IdleEntity.pop_front();
            return Connection<T>(entity,this->shared_from_this());;
        }

        void initialize(const std::function<const Safe<T>(void)>& initFunc){
            m_InitFunction = std::move(initFunc);
        }
    private:
        void restore(const Safe<T>& entity){
            Magic::Mutex::Lock locker(m_Mutex);
            if(m_IdleEntity.size() == m_Count){
                return;
            }
            m_IdleEntity.push_back(entity);
        }
    private:
        uint32_t m_Count;
        Magic::Mutex m_Mutex;
        std::list<Safe<T>> m_IdleEntity;
        std::function<const Safe<T>(void)> m_InitFunction;
    };
}
}
