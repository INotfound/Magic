#include "Scheduler.h"

using namespace Magic;

static thread_local Scheduler* g_scheduler = nullptr;
static thread_local Fiber*	g_fiber = nullptr;

FiberAndThread::FiberAndThread(Ptr<Fiber> &fiber,uint32_t thread)
	: m_ThreadId(thread){
	m_Fiber = std::move(fiber);
}

FiberAndThread::FiberAndThread(std::function<void()> func,uint32_t thread)
	: m_ThreadId(thread){
	m_CallBack.swap(func);
}

void FiberAndThread::reset(){
	m_Fiber = nullptr;
	m_CallBack = nullptr;
	m_ThreadId = -1;
}


Scheduler::~Scheduler(){
	MAGIC_ASSERT(m_Stopping,"The Scheduler not stop");
	if(GetThis() == this){
		g_scheduler = nullptr;
	}
}

Scheduler::Scheduler(uint32_t threads, bool useCaller,const std::string &name)
	: m_Name(name)
{
	MAGIC_ASSERT(threads > 0,"thread < 0");
	if(useCaller){
		Magic::Fiber::Init();
		threads--;
		MAGIC_ASSERT(GetThis() == nullptr,"Scheduler not nullptr");
		g_scheduler = this;
		m_RootFiber.reset(new Fiber(std::bind(&Scheduler::run,this),true));
		g_fiber = m_RootFiber.get();
		m_RootThread = Magic::GetThreadId();
		m_ThreadIds.push_back(m_RootThread);
	}
	m_ThreadCount = threads;
}

const std::string& Scheduler::getName(){
	return m_Name;
}

void Scheduler::start(){
	MutexType::Lock lock(m_Mutex);
	if(!m_Stopping){
		return;
	}
	m_Stopping = false;
	MAGIC_ASSERT(m_Threads.empty(),"Threads not emppty");
	m_Threads.resize(m_ThreadCount);
	for(size_t i = 0; i < m_ThreadCount;i++){
		m_Threads[i].reset(new Thread(
			m_Name + "_" + std::to_string(i),
			std::bind(&Scheduler::run,this)
			));
		m_ThreadIds.push_back(m_Threads[i]->getId());
	}
}

void Scheduler::stop(){
	m_AutoStop = true;
	if(m_RootFiber 
		&& m_ThreadCount == 0 
		&& (m_RootFiber->getState() == Fiber::TERM
		|| m_RootFiber->getState() == Fiber::INIT)){
			m_Stopping =true;
			
			if(stopping()){
				return;
			}
	}

	if(m_RootThread !=-1){
		MAGIC_ASSERT(GetThis() == this,"Not self");
	}else{
		MAGIC_ASSERT(GetThis() != this,"Is self");
	}

	m_Stopping = true;
	for(size_t i = 0; i<m_ThreadCount;i++){
		tickle();
	}
	if(m_RootFiber){
		tickle();//notify
		if(!stopping()){
			m_RootFiber->toCall();
		}			
	}
}

void Scheduler::idle(){

}

void Scheduler::run(){
	this->setThis();
	if(Magic::GetThreadId() != m_RootThread){
		g_fiber = Fiber::Init().get();
	}
	Ptr<Fiber> idleFiber(new Fiber(&Scheduler::idle,false));
	Ptr<Fiber> callBackFiber;
	FiberAndThread fiberAndThread;
	while(iter != m_Fiber.end()){
		{
			bool tickle = false;
			auto iter = m_Fiber.begin();
			MutexType::Lock lock(m_Mutex);
			if(iter->m_ThreadId != -1 && iter->m_ThreadId != Magic::GetThreadId()){
				iter++;
				tickle = true;
				continue;
			}
			if(iter->m_Fiber && iter->m_Fiber->getState() == Fiber::EXEC){
				iter++;
				continue;
			}
			fiberAndThread = *iter;
			auto& temp = iter;
			iter++;
			m_Fiber.erase(temp);
			m_ActiveThreadCount++;
			break;
		}

	}


}

void Scheduler::setThis(){
	g_scheduler = this;
}

void Scheduler::tickle(){
}

bool Scheduler::stopping(){
	return true;
}

Scheduler* Scheduler::GetThis(){
	return g_scheduler;
}
Fiber* Scheduler::GetMainFiber(){
	return g_fiber;
}

