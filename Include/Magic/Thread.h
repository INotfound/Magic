#pragma once
#include <thread>
#include "Core.h"
#include "Mutex.h"

namespace Magic{

class Thread{
    typedef Semaphore Mutex;
public:
    ~Thread();
    Thread(const std::string& threadName,const std::function<void()> callback);
    void join();
    void detach();
    int64_t getId();
    static const std::string& GetName();
    static void SetName(const std::string& threadName);
protected:
    void run();
private:
    int64_t                 m_Id;
    std::string             m_Name;
    Mutex                   m_Mutex;
    std::thread             m_Thread;
    std::function<void()>   m_CallBack;

};

}