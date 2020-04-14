/*
 * @file: Thread.cpp
 * @Author: INotFound
 * @Date: 2020-03-12 02:32:11
 * @LastEditTime: 2020-03-14 00:14:59
 */
#include "Core.h"
#include "Macro.h"
#include "Thread.h"

using namespace Magic;

static thread_local Thread* g_Thread = nullptr;
static thread_local std::string g_ThreadName = "UNKNOW";

Thread::~Thread() {
}
Thread::Thread(const std::string& threadName,const std::function<void()>& callback)
    :m_Id(-1)
    ,m_Name(threadName)
    ,m_Thread(&Thread::run,this)
    ,m_CallBack(std::move(callback)){
    MAGIC_DEBUG() << "Start thread: " << threadName;
    if(threadName.empty()){
        m_Name = "UNKNOW";
    }
    if(!m_CallBack){
        MAGIC_ERROR() << "Thread-CallBack is null";
    }
    m_Mutex.wait();
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
    g_Thread = this;
    SetName(m_Name);
    m_Id = Magic::GetThreadId();
    m_Mutex.notify();
    m_CallBack();
}