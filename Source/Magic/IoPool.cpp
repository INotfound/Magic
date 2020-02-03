#include "IoPool.h"
#include "Macro.h"
#include "Thread.h"
namespace Magic{
    IoPool::~IoPool(){
    }
    IoPool::IoPool(uint32_t poolSize)
        :m_Next(0),m_PoolSize(poolSize){
        MAGIC_ASSERT(poolSize >= 0,"threadCount < 0 Or = 0");
        for(uint32_t i = 0; i<m_PoolSize; i++){
            Safe<asio::io_context> io(new asio::io_context);
            m_IOServiceWork.push_back(Safe<asio::io_context::work>(new asio::io_context::work(*io)));
            m_IOService.push_back(std::move(io));
            
        }

    }
    void IoPool::run(){
        std::vector<Safe<Thread>> threads;
        for(uint32_t i = 0; i<m_PoolSize; i++){
            threads.push_back(Safe<Thread>{
                new Thread{"IoPool/"+std::to_string(i),
                    [this,i](){
                        m_IOService.at(i)->run();
                    }
                }
            });
        }
        for(uint32_t i = 0; i<threads.size(); i++){
            threads.at(i)->join();
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