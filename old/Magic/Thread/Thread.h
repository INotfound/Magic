#pragma once
#include <atomic>
#include <pthread.h>
#include <functional>
#include "Mutex.h"
#include "../Util/Noncopyable.h"

namespace Magic{

class Thread : public Noncopyable{
public:
    Thread(const std::string& threadName,std::function<void()> callback);
    ~Thread();
    pid_t getId();
    void join();
    static const std::string& GetName();
	static void SetName(const std::string& threadName);
private:
    static void* Run(void* arg);
private:
    pid_t m_Id =-1;
    pthread_t m_Pthread =0;
    Semaphore m_Semaphore;
    std::string m_ThreadName;
    std::function<void()> m_Callback;
};
}
