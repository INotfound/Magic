/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Thread.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Logger.hpp"
#include "Magic/Utilty/Thread.hpp"

namespace Magic{
    static thread_local std::string g_ThreadName = "UNKNOWN";

    Thread::~Thread() = default;

    Thread::Thread(const Magic::StringView& threadName,std::function<void()> callBack)
        :m_Id(-1)
        ,m_Name(threadName.data(),threadName.size())
        ,m_CallBack(std::move(callBack)){
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

    Magic::StringView Thread::GetName(){
        return g_ThreadName;
    }

    void Thread::SetName(const Magic::StringView& threadName){
        g_ThreadName = std::string(threadName.data(),threadName.size());
    }

    void Thread::run(){
        m_Id = Magic::GetThreadId();
        SetName(m_Name);
        m_CallBack();
    }
}

