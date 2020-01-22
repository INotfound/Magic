#include "Scheduler.h"
#include "Fiber.h"
#include "Macro.h"
#include "Util.h"
#include <functional>

namespace Magic{
    
    static thread_local Scheduler*  g_Scheduler{nullptr};
    static thread_local Fiber*      g_SchedulerFiber{nullptr};

    Scheduler::FiberOrThread::FiberOrThread()
        :m_threadId{-1}{
    }
    Scheduler::FiberOrThread::FiberOrThread(int64_t threadId,MagicPtr<Fiber> fiber)
        :m_threadId{threadId}
        ,m_Fiber{std::move(fiber)}{
    }
    Scheduler::FiberOrThread::FiberOrThread(int64_t threadId,MagicPtr<Fiber> &fiber)
        :m_threadId{threadId}
        ,m_Fiber{std::move(fiber)}{
    }
    Scheduler::FiberOrThread::FiberOrThread(int64_t threadId,std::function<void()> callBack)
        :m_threadId{threadId}
        ,m_CallBack{std::move(callBack)}{
    }
    Scheduler::FiberOrThread::FiberOrThread(int64_t threadId,std::function<void()> &callback)
        :m_threadId{threadId}
        ,m_CallBack{std::move(callback)}{
    }
    void Scheduler::FiberOrThread::reset(){
        m_Fiber.reset();
        m_threadId = 0;
        m_CallBack = nullptr;
    }

    Scheduler::~Scheduler(){
        //MAGIC_ASSERT(, msg)
        if(GetThis() == this){
            g_Scheduler = nullptr;
        }
    }
    Scheduler::Scheduler(uint64_t threadCount,bool useSelf,const std::string& name)
        :m_Name{name}{
        MAGIC_ASSERT(threadCount > 0, "threadCount <= 0");
        if(useSelf){
            threadCount--;
            Thread::SetName(name);
            MAGIC_ASSERT(GetThis() == nullptr, "Already initialized");
            g_Scheduler = this;
            m_RootFiber.reset(new Fiber(std::bind(&Scheduler::run,this),true));
            g_SchedulerFiber = m_RootFiber.get();
            m_RootThreadId = Magic::GetThreadId();
            m_ThreadIds.push_back(m_RootThreadId);
        }else{
            m_RootThreadId = -1;
        }
        m_ThreadCount = threadCount;
    }
    void Scheduler::start(){

    }
    void Scheduler::stop(){
    }
    Scheduler* Scheduler::GetThis(){
        return g_Scheduler;
    }
    Fiber* Scheduler::GetMainFiber(){
        return g_SchedulerFiber;
    }
    void Scheduler::run(){
    }
    void Scheduler::tickle(){
    }
}