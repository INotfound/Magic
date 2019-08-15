#pragma once
#include <cstdint>
#include <pthread.h>
#include <semaphore.h>
#include "../Util/Noncopyable.h"

namespace Magic{

class Semaphore : public Noncopyable{

public:
    Semaphore(uint32_t count =0);

    ~Semaphore();

    void wait();

    void notify();

private:
    sem_t m_Semaphore;
};

template<class T>
class ScopedLockImpl{

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

class Mutex{
public:
    typedef ScopedLockImpl<Mutex> Lock;
    Mutex();
    ~Mutex();
private:

};

template<class T>
class ReadScopedLockImpl{
public:
    ReadScopedLockImpl(T& mutex)
        :m_Mutex(mutex),m_Locked(false){
        lock();
    }
    ~ReadScopedLockImpl(){
        unlock();
    }
private:
    void lock(){
        if(!m_Locked){
            m_Mutex.readLock();
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

template<class T>
class WriteScopedLockImpl{
public:
    WriteScopedLockImpl(T& mutex)
        :m_Mutex(mutex),m_Locked(false){
        lock();
    }
    ~WriteScopedLockImpl(){
        unlock();
    }
private:
    void lock(){
        if(!m_Locked){
            m_Mutex.writeLock();
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

class RWMutex{
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
