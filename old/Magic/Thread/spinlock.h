#pragma once
#include <pthread.h>
#include "scopedlock.h"
#include "../Util/Noncopyable.h"
namespace Magic{

class Spinlock : Noncopyable {
public:
    typedef ScopedLockImpl<Spinlock> Lock;

    Spinlock();

    ~Spinlock();

    void lock();

    void unlock();

private:
    pthread_spinlock_t m_Mutex;
};

}