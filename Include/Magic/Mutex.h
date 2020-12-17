/*
 * @File: Mutex.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:32:30
 * @LastEditTime: 2020-03-14 00:09:49
 */
#pragma once
#include "Core.h"
#include "Adapter.h"

namespace Magic{
    /**
     * @brief: 读写锁
     */
    class RWMutex : public Noncopyable {
    public:
        typedef ReadScopedLockImpl<RWMutex> ReadLock;
        typedef WriteScopedLockImpl<RWMutex> WriteLock;
        RWMutex();
        ~RWMutex();
        void readLock();
        void writeLock();
        void unlock();
    private:
        rwlock_t m_RWLock;
    };
    /**
     * @brief: 自旋锁
     */
    class Spinlock : Noncopyable {
    public:
        typedef ScopedLockImpl<Spinlock> Lock;

        Spinlock();

        ~Spinlock();

        void lock();

        void unlock();

    private:
        spinlock_t m_Mutex;
    };
    /**
     * @brief: 普通锁
     */
    class Mutex : public Noncopyable {
    public:
        typedef ScopedLockImpl<Mutex> Lock;
        Mutex();
        ~Mutex();
        void lock();
        void unlock();
    private:
        mutex_t m_Mutex;
    };
    /**
     * @brief: 信号量
     */
    class Semaphore : public Noncopyable{
    public:
        Semaphore(uint32_t count =0);

        ~Semaphore();

        void wait();

        void notify();
    private:
        sem_t m_Semaphore;
    };
}