#pragma once
#include <cstdint>
#include <pthread.h>
#include "semaphore.h"
#include "rwMutex.h"
#include "../Util/Noncopyable.h"

namespace Magic{


template<class T>
class ScopedLockImpl : public Noncopyable{
public:
    ScopedLockImpl(T& mutex)
        :m_Mutex(mutex),m_Locked(false){
        lock();
    }
    ~ScopedLockImpl(){
        unlock();
    }
private:
    void lock(){
        if(!m_Locked){
            m_Mutex.lock();
            m_Locked = true;
        }
    }
    void unlock(){
        if(m_Locked){
            m_Mutex.unlock();
            m_Locked = false;
        }
    }
private:
    T& m_Mutex;
    bool m_Locked;
};

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
