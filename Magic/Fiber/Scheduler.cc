#include "Scheduler.h"

using namespace Magic;

FiberAndThread::FiberAndThread(Ptr<Fiber> &fiber,uint32_t thread)
	: m_ThreadId(thread){
	m_Fiber = std::move(fiber);
}

FiberAndThread::FiberAndThread(std::function<void()> func,uint32_t thread)
	: m_ThreadId(thread){
	m_CallBack.swap(func);
}

void FiberAndThread::reset(){

}


Scheduler::~Scheduler(){

}

Scheduler::Scheduler(uint32_t threads, bool useCaller,const std::string &name)
	: m_Name(name){
	
}

const std::string& Scheduler::getName(){
	return m_Name;
}

void Scheduler::start(){

}

void Scheduler::stop(){

}

void Scheduler::tickle(){

}

Scheduler* Scheduler::GetThis(){
	return nullptr;
}
Fiber* Scheduler::GetMainFiber(){
	return nullptr;
}

