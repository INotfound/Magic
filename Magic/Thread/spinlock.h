#pragma once
#include <pthread.h>
#include "scopedlock.h"
#include "../Util/Noncopyable.h"
namespace Magic{

class Spinlock : Noncopyable {
public:
    typedef ScopedLockImpl<Spinlock> Lock;

    Spinlock() {
        pthread_spin_init(&m_Mutex, 0);
    }

    ~Spinlock() {
        pthread_spin_destroy(&m_Mutex);
    }

    void lock() {
        pthread_spin_lock(&m_Mutex);
    }

    void unlock() {
        pthread_spin_unlock(&m_Mutex);
    }
private:
    pthread_spinlock_t m_Mutex;
};

}