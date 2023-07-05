/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : IoPool.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Utilty/Logger.hpp"
#include "Magic/NetWork/IoPool.hpp"

namespace Magic{
namespace NetWork{
    Safe<IoPool> g_IoPool;

    IoPool::~IoPool() = default;

    IoPool::IoPool(const Safe<Config>& configuration)
        :m_Next(0)
        ,m_PoolSize(configuration->at<uint32_t>("NetWork.IoPool.Size",0)){
        if(m_PoolSize == 0){
            m_PoolSize = std::thread::hardware_concurrency();
        }
        for(uint32_t i = 0;i < m_PoolSize;i++){
            Safe<asio::io_context> io = std::make_shared<asio::io_context>();
            m_IOServiceWork.push_back(std::make_shared<asio::executor_work_guard<asio::io_context::executor_type>>(asio::make_work_guard(*io)));
            m_IOService.push_back(std::move(io));
        }
        for(uint32_t i = 0;i < m_PoolSize;i++){
            m_Threads.push_back(std::make_shared<Thread>("IoPool/" + std::to_string(i),[this,i](){
                m_IOService.at(i)->run();
            }));
        }
    }

    void IoPool::wait(){
        for(auto& thread : m_Threads){
            thread->join();
        }
    }

    void IoPool::stop(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        for(uint32_t i = 0;i < m_PoolSize;i++){
            m_IOServiceWork.at(i).reset();
        }
    }

    void IoPool::externMode(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!g_IoPool)
            g_IoPool = this->shared_from_this();
    }

    const Safe<asio::io_context>& IoPool::get(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        const auto& io = m_IOService.at(m_Next);
        m_Next++;
        if(m_Next == m_PoolSize){
            m_Next = 0;
        }
        return io;
    }
}
}
