#pragma once
#include <pthread.h>
#include "scopedlock.h"
#include "../Util/Noncopyable.h"

namespace Magic {


class RWMutex : public Noncopyable{
public:
    typedef ReadScopedLockImpl<RWMutex> ReadLock;
    typedef WriteScopedLockImpl<RWMutex> WriteLock;

    RWMutex();
    ~RWMutex();
    void readLock();
    void writeLock();
    void unlock();
private:
    pthread_rwlock_t m_RWLock;
};
}
