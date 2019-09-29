#pragma once
#include <thread>
#include "Core.h"
#include "Util.h"
#include "Mutex.h"

namespace Magic{

class Thread{
    typedef Semaphore Mutex;
public:
    Thread(const std::string& threadName,const std::function<void()> callback);
	~Thread();
	void join();
    int64_t getId();
    static const std::string& GetName();
    static void SetName(const std::string& threadName);
protected:
    void run();
private:
    Mutex m_Mutex;
    int64_t m_Id = -1;
    std::string m_ThreadName;
    std::function<void()> m_CallBack;
	std::thread m_Thread;
};

}