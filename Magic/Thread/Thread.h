#pragma once
#include <pthread.h>
#include <functional>
#include "../Util/Noncopyable.h"

namespace Magic{
class Thread : public Noncopyable{
public:
    Thread(const std::string& threadName,std::function<void()> callback);
    ~Thread();
    pid_t getId();
    const std::string& getName() const;
    void join();
private:
    static void* run(void* arg);
private:
    pid_t m_Id;
    pthread_t m_Pthread;
    std::string m_ThreadName;
    std::function<void()> m_Callback;
};
}
