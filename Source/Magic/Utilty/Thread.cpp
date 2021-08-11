/*
 * @Author: INotFound
 * @Date: 2020-03-12 02:32:11
 * @LastEditTime: 2021-02-01 22:25:58
 */
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/Thread.h"

#include <utility>

namespace Magic{
    static thread_local std::string g_ThreadName = "UNKNOWN";

    Thread::~Thread() {
    }
    
    Thread::Thread(const std::string& threadName,std::function<void()> callback)
        :m_Id(-1)
        ,m_Name(threadName)
        ,m_Thread(&Thread::run,this)
        ,m_CallBack(std::move(callback)){
        MAGIC_DEBUG() << "Start thread: " << threadName;
        if(threadName.empty()){
            m_Name = "UNKNOWN";
        }
        if(!m_CallBack){
            MAGIC_ERROR() << "Thread-CallBack is null";
        }
        m_Mutex.notify();
    }

    void Thread::join() {
        m_Thread.join();
    }

    void Thread::detach(){
        m_Thread.detach();
    }

    int64_t Thread::getId(){
        return m_Id;
    }

    const std::string& Thread::GetName(){
        return g_ThreadName;
    }

    void Thread::SetName(const std::string& threadName){
        g_ThreadName = threadName;
    }
    
    void Thread::run(){
        m_Mutex.wait();
        SetName(m_Name);
        m_Id = Magic::GetThreadId();
        m_CallBack();
    }
}

