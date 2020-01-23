#pragma once
#include <list>
#include <vector>
#include <functional>
#include "Core.h"
#include "Fiber.h"
#include "Mutex.h"
#include "Thread.h"
namespace Magic{
    class Scheduler{
    private:
        class FiberOrThread{
        public:
            FiberOrThread();
            FiberOrThread(int64_t threadId,MagicPtr<Fiber> fiber);
            FiberOrThread(int64_t threadId,MagicPtr<Fiber> &fiber);
            FiberOrThread(int64_t threadId,std::function<void()> callBack);
            FiberOrThread(int64_t threadId,std::function<void()> &callback);
            void reset();
        public:
            int64_t m_threadId{-1};
            MagicPtr<Fiber> m_Fiber{};
            std::function<void()> m_CallBack{};
        };
    public:
        virtual ~Scheduler();
        Scheduler(uint64_t threadCount = 1,bool useSelf = true,const std::string& name = "");
        void start();
        void stop();
        
        template<class T>
        void add(T type,uint64_t threadId = 0){
           //是否需要通知,有任务则通知
            bool needTickle = false;
            {
                MutexLock lock(m_Mutex);
                needTickle = addNoLockFiberOrThread(threadId,type);
            }
            if(needTickle){
                tickle();
            }
        }
        template<class Iterater>
        void add(Iterater begin,Iterater end){
           //是否需要通知,有任务则通知
            bool needTickle = false;
            {
                MutexLock lock(m_Mutex);
                for(;begin != end;begin++){
                    needTickle = addNoLockFiberOrThread(0,begin) || needTickle;
                }
                
            }
            if(needTickle){
                tickle();
            }
        }

        static Scheduler* GetThis();
        static Fiber* GetMainFiber();
    protected:
        void run();
        virtual void tickle();
    private:
        template<class T>
        bool addNoLock(uint64_t threadId,T type){
            bool needTickle = m_FiberOrThreads.empty();
            MagicPtr<FiberOrThread> ft(new FiberOrThread{threadId,type});
            if(ft->m_Fiber || ft->m_CallBack){
                m_FiberOrThreads.push_back(ft);
            }
            return needTickle;
        }
    private:
        Mutex m_Mutex;
        std::string m_Name{};
        int64_t m_RootThreadId{};
        uint64_t m_ThreadCount{};
        MagicPtr<Fiber> m_RootFiber{};
        std::vector<int64_t> m_ThreadIds{};
        std::vector<Magic::Thread> m_Threads{};
        std::list<MagicPtr<FiberOrThread>> m_FiberOrThreads{};
    };
}