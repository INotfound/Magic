#pragma once
#include <list>
#include <vector>

#include "Fiber.h"

#include "../Util/Macro.h"
#include "../Thread/Mutex.h"
#include "../Thread/Thread.h"

namespace Magic{

class FiberAndThread{
	friend class Scheduler;
public:
	FiberAndThread(Ptr<Fiber> &fiber,uint32_t thread);
	FiberAndThread(std::function<void()> func,uint32_t thread);
	void reset();

private:
	uint32_t m_ThreadId = -1;
	Ptr<Fiber> m_Fiber = nullptr;
	std::function<void()> m_CallBack;
};

class Scheduler{
	typedef Mutex MutexType; 
public:
	virtual ~Scheduler();
	Scheduler(uint32_t threads =1, bool useCaller =true,const std::string &name ="");
	const std::string& getName();
	void start();
	void stop();
	
	//name "add"
	template<class FiberOrCallBack>
	void scheduler(FiberOrCallBack fc,uint32_t thread = -1){
		bool need_tickle = false; 		{
			MutexType::Lock lock(m_Mutex);
			need_tickle = schedulerNoLock(fc,thread);
		}
		if(need_tickle){
			tickle();
		}
	}

	template<class InputIterator>
	void scheduler(InputIterator begin,InputIterator end){
		bool need_tickle = false;
		{
			MutexType::Lock lock(m_Mutex);
			while(begin != end){
				need_tickle = schedulerNoLock(*begin,-1) || need_tickle;
				begin++;
			}
		}
		if(need_tickle){
			tickle();
		}
	}

protected:
	virtual void tickle();//why name is tickkle?
	static Scheduler* GetThis();
	static Fiber* GetMainFiber();

private:
	template<class FiberOrCallBack>
	bool schedulerNoLock(FiberOrCallBack &fc,uint32_t thread = -1){
		bool need_tickle = m_Fibers.empty();
		Ptr<FiberAndThread> ft(fc,thread);
		if(ft->m_Fiber || ft->m_CallBack){
			m_Fibers.push_back(ft);
		}
		return need_tickle;
	}
protected:
	std::vector<int> m_ThreadIds;
	bool m_Stopping = true;
	bool m_AutoStop = false;
	uint32_t m_rootThread = 0;
	uint32_t m_ThreadCount = 0;
	uint32_t m_ActiveThreadCount = 0;
	uint32_t m_IdleThreadCount = 0;
private:
	MutexType m_Mutex;
	std::string m_Name;
	std::vector<Ptr<Thread>> m_Threads;
	std::list<Ptr<FiberAndThread>> m_Fibers;

};

}

