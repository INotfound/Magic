/*
 * @Author: INotFound
 * @Date: 2020-03-04 23:43:46
 * @LastEditTime: 2020-09-30 21:19:07
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/Thread.h"
#include "Magic/NetWork/IoPool.h"


namespace Magic{
namespace NetWork{
    IoPool::~IoPool() =default;

    IoPool::IoPool(const Safe<Config>& configuration)
        :m_Next(0)
        ,m_PoolSize(configuration->at<uint32_t>("NetWork.IoPool.Size",2)){
        for(uint32_t i = 0; i<m_PoolSize; i++){
            Safe<asio::io_context> io = std::make_shared<asio::io_context>();
            m_IOServiceWork.push_back(std::make_shared<asio::executor_work_guard<asio::io_context::executor_type>>(asio::make_work_guard(*io)));
            m_IOService.push_back(std::move(io));
        }
    }
    
    void IoPool::run(){
        std::vector<Safe<Thread>> threads;
        for(uint32_t i = 0; i<m_PoolSize; i++){
            threads.push_back(std::make_shared<Thread>("IoPool/"+std::to_string(i),
                [this,i](){
                    m_IOService.at(i)->run();
                }
            ));
        }
        for(auto& thread : threads){
            thread->join();
        }
    }

    void IoPool::stop(){
        for(uint32_t i = 0; i<m_PoolSize; i++){
            m_IOServiceWork.at(i).reset();
        }
    }
    
    asio::io_context& IoPool::get(){
        asio::io_context& io = *m_IOService.at(m_Next);
        m_Next++;
        if(m_Next == m_PoolSize){
            m_Next = 0;
        }
        return io;
    }
}
}
