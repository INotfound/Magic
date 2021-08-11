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
            m_Pool->restore(m_Entity);
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
        friend class FunctionTaskNode;
    public:
        ConnectionPool(const Safe<Magic::Config>& configuration,const Safe<Magic::TimingWheel>& timingWheel)
            :m_TimeOut(false)
            ,m_TimingWheel(timingWheel){
            uint32_t count = configuration->at("DataBase.Connection.Count",1);
            std::string dataBaseUrl = configuration->at<std::string>("DataBase.Connection.Url","");
            for (uint32_t i = 0; i < count; i++){
                Safe<T> conn = std::make_shared<T>(dataBaseUrl);
                m_IdleEntity.push_back(conn);
            }
        }

        Connection<T> getConnection(){
            if(m_IdleEntity.empty()){
                MAGIC_DEBUG() << "Idle Entity Empty!";
                Safe<Magic::ITaskNode> taskNode = std::make_shared<Magic::FunctionTaskNode>([this](){
                    MAGIC_WARN() << "Get Idle Entity TimeOut!";
                    this->m_TimeOut = true;
                });
                m_TimingWheel->addTask(100,taskNode);
                while(m_IdleEntity.empty() && !m_TimeOut);
                if(m_TimeOut){
                    return Connection<T>();
                }
                MAGIC_DEBUG() << "Idle Entity No TimeOut!";
            }
            Safe<T> entity;
            {
                Magic::Mutex::Lock locker(m_Mutex);
                m_TimeOut = false;
                entity = m_IdleEntity.front();
                m_IdleEntity.pop_front();
            }
            return Connection<T>(entity,this->shared_from_this());;
        }

        void initialize(std::function<void(const Safe<T>& entity)> func){
            for(auto& val : m_IdleEntity){
                func(val);
            }
        }
    private:
        void restore(const Safe<T>& entity){
            Magic::Mutex::Lock locker(m_Mutex);
            m_TimeOut = false;
            m_IdleEntity.push_back(entity);
        }
    private:
        Magic::Mutex m_Mutex; 
        std::atomic_bool m_TimeOut;
        std::list<Safe<T>> m_IdleEntity;
        Safe<Magic::TimingWheel> m_TimingWheel;
    };
}
}
