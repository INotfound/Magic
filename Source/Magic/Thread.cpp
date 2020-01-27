#include "Thread.h"
#include "Util.h"
#include "Macro.h"

using namespace Magic;

static thread_local Thread* g_Thread{ nullptr };
static thread_local std::string g_ThreadName{ "UNKNOW" };

Thread::~Thread() {
}
Thread::Thread(const std::string& threadName,const std::function<void()> callback)
    :m_ThreadName{ threadName }
	,m_CallBack{ callback }
	,m_Thread{ &Thread::run,this }{
	MAGIC_LOG(LogLevel::LogDebug) << "Start thread: " << threadName;
    if(threadName.empty()){
        m_ThreadName = "UNKNOW";
    }
    if(!m_CallBack){
		MAGIC_LOG(LogLevel::LogError) << "Thread-CallBack is null";
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
    SetName(m_ThreadName);
    m_Id = Magic::GetThreadId();
    m_Mutex.notify();
	m_CallBack();
}