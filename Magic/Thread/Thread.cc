#include "Thread.h"
#include <stdexcept>
#include "../Log/Log.h"
#include "../Util/Util.h"


using namespace Magic;

static thread_local Thread* g_thread;
static thread_local std::string g_threadName = "UNKNOW";
static auto& g_log = MAGIC_LOG_NAME("system");

Thread::Thread(const std::string& threadName,std::function<void()> callback)
    :m_Id(0)
    ,m_Pthread(0)
    ,m_ThreadName(threadName)
    ,m_Callback(callback)
{
    if(threadName.empty()){
        m_ThreadName = "UNKNOW";
    }
    auto err = pthread_create(&m_Pthread,nullptr,&Thread::run,this);
    if(err){
        MAGIC_LOG_DEBUG(g_log) << "pthread_create thread failed! err_code=" << err
                                          <<" thread_name=" << m_ThreadName;
        throw std::logic_error("pthread_create error");
    }
}

Thread::~Thread(){
    if(m_Pthread) {
        pthread_detach(m_Pthread);
    }
}

pid_t Thread::getId(){
    return m_Id;
}

const std::string& Thread::getName() const{
    return m_ThreadName;
}

void Thread::join(){
    if(m_Pthread){
        auto err = pthread_join(m_Pthread,nullptr);
        if(err){
            MAGIC_LOG_DEBUG(g_log) << "pthread_join thread failed! err_code=" << err
                                              <<" thread_name=" << m_ThreadName;
            throw std::logic_error("pthread_join error");
        }
        m_Pthread = 0;
    }
}

void* Thread::run(void*arg){
    Thread* thread = reinterpret_cast<Thread*>(arg);
    thread->m_Id = getThreadId();
    g_thread = thread;
    g_threadName = thread->getName();
    return nullptr;
}
