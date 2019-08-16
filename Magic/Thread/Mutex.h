#pragma once
#include <cstdint>
#include <pthread.h>
#include "rwMutex.h"
#include "spinlock.h"
#include "semaphore.h"
#include "scopedlock.h"
#include "../Util/Noncopyable.h"

namespace Magic{

class Mutex : public Noncopyable{
public:
    typedef ScopedLockImpl<Mutex> Lock;
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
private:
    pthread_mutex_t m_Mutex;
};

}
