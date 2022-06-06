/*
 * @Author: INotFound
 * @Date: 2020-03-13 22:32:30
 * @LastEditTime: 2021-02-01 22:16:02
 */
#pragma once
#include "Magic/Core/Core.h"
namespace Magic{
    class MutexImpl;
    class RWMutexImpl;
    class SpinLockImpl;
    class SemaphoreImpl;

    template<class T>
    class ScopedLockImpl :public Noncopyable{
    public:
        explicit ScopedLockImpl(T& mutex)
            :m_Mutex(mutex), m_Locked(false){
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

    template<class T>
    class ReadScopedLockImpl :public Noncopyable{
    public:
        explicit ReadScopedLockImpl(T& mutex)
            :m_Mutex(mutex), m_Locked(false){
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
    class WriteScopedLockImpl :public Noncopyable{
    public:
        explicit WriteScopedLockImpl(T& mutex)
            :m_Mutex(mutex), m_Locked(false){
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
    /**
     * @brief 普通锁
     */
    class Mutex :public Noncopyable{
    public:
        typedef ScopedLockImpl<Mutex> Lock;
    public:
        Mutex();
        void lock();
        void unlock();
    private:
        Safe<MutexImpl> m_Mutex;
    };
    /**
     * @brief 读写锁
     */
    class RWMutex :public Noncopyable{
    public:
        typedef ReadScopedLockImpl<RWMutex> ReadLock;
        typedef WriteScopedLockImpl<RWMutex> WriteLock;
    public:
        RWMutex();
        void unlock();
        void readLock();
        void writeLock();
    private:
        Safe<RWMutexImpl> m_RWLock;
    };
    /**
     * @brief 自旋锁
     */
    class SpinLock :public Noncopyable{
    public:
        typedef ScopedLockImpl<SpinLock> Lock;
    public:
        SpinLock();
        void lock();
        void unlock();
    private:
        Safe<SpinLockImpl> m_SpinLock;
    };
    /**
     * @brief 信号量
     */
    class Semaphore :public Noncopyable{
    public:
        explicit Semaphore(uint32_t count =0);

        void wait();

        void notify();
    private:
        Safe<SemaphoreImpl> m_Semaphore;
    };
}
