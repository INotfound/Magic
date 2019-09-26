#include "Thread.h"
#include "Util.h"
#include "Macro.h"

using namespace Magic;

static thread_local Thread* g_Thread =nullptr;
static thread_local std::string g_ThreadName ="UNKNOW";
static auto& g_Log = MAGIC_LOG_ROOT();

Thread::Thread(const std::string& threadName,std::function<void()> callback)
    :thread(&Thread::run,this)
    ,m_ThreadName(threadName)
    ,m_CallBack(callback){
        if(threadName.empty()){
            m_ThreadName = "UNKNOW";
        }
    if(!m_CallBack){
        MAGIC_LOG_ERROR(g_Log) << "Thread-CallBack is null";
    }
    m_Mutex.wait();
    MAGIC_LOG_DEBUG(g_Log) << "~~~~DEBUG~~~~";
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
    SetName(m_ThreadName);
    m_Id = Magic::GetThreadId();
    MAGIC_LOG_DEBUG(g_Log) << "~~~~TEST~~~~";
    m_Mutex.notify();
    m_CallBack();
}