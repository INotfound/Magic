/*
 * @Author: INotFound
 * @Date: 2020-03-12 02:32:11
 * @LastEditTime: 2021-02-01 22:25:58
 */
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/Thread.h"

namespace Magic{
    static thread_local std::string g_ThreadName = "UNKNOWN";

    Thread::~Thread() = default;

    Thread::Thread(const std::string& threadName,std::function<void()> callback)
        :m_Id(-1)
        ,m_Name(threadName)
        ,m_CallBack(std::move(callback)){
        MAGIC_INFO() << "Start Thread: " << threadName;
        if(threadName.empty()){
            m_Name = "UNKNOWN";
        }
        if(!m_CallBack){
            throw Failure("Thread CallBack Is NullPtr");
        }
        m_Thread = std::make_shared<std::thread>(&Thread::run,this);
    }

    void Thread::join(){
        if(m_Thread)
            m_Thread->join();
    }

    void Thread::detach(){
        if(m_Thread)
            m_Thread->detach();
    }

    int64_t Thread::getId() const{
        return m_Id;
    }

    const std::string& Thread::GetName(){
        return g_ThreadName;
    }

    void Thread::SetName(const std::string& threadName){
        g_ThreadName = threadName;
    }

    void Thread::run(){
        m_Id = Magic::GetThreadId();
        SetName(m_Name);
        m_CallBack();
    }
}

