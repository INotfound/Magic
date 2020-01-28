#include "IoPool.h"
#include "Macro.h"
#include "Thread.h"
namespace Magic{
    IoPool::~IoPool(){
    }
    IoPool::IoPool(uint32_t poolSize)
        :m_PoolSize{poolSize}{
        MAGIC_ASSERT(poolSize >= 0,"threadCount < 0 Or = 0");
        for(uint32_t i{0}; i<m_PoolSize; i++){
            m_IOService.push_back(MagicPtr<asio::io_context>{new asio::io_context});
        }
    }
    void IoPool::run(){
        std::vector<MagicPtr<Thread>> threads{};
        for(uint32_t i{0}; i<m_PoolSize; i++){
            threads.push_back(MagicPtr<Thread>{
                new Thread{"IoPool/"+std::to_string(i),
                    [this,i](){
                        m_IOService.at(i)->run();
                    }
                }
            });
        }
        for(uint32_t i{0}; i<threads.size(); i++){
            threads.at(i)->join();
            MAGIC_LOG(Magic::LogLevel::LogInfo) << "Free: "  << threads.at(i)->GetName();
        }
    }
    void IoPool::stop(){
        for(uint32_t i{0}; i<m_PoolSize; i++){
            m_IOService.at(i).reset();
        }
    }
    asio::io_context& IoPool::get(){
        asio::io_context& io = *m_IOService.at(m_Next).get();
        m_Next++;
        if(m_Next == m_PoolSize){
            m_Next = 0;
        }
        return io;
    }
}