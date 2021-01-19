/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:35:10
 * @LastEditTime: 2020-10-01 16:37:45
 */
#if defined(_WIN32) || defined(_WIN64)

#include "Core/Mutex.h"

namespace Magic{
    RWMutex::RWMutex() {
        InitializeSRWLock(&m_RWLock.lock);
    }

    RWMutex::~RWMutex() {
    }

    void RWMutex::readLock() {
        AcquireSRWLockShared(&m_RWLock.lock);
        m_RWLock.flag = WinRWLock::RW::Read;
    }

    void RWMutex::writeLock() {
        AcquireSRWLockExclusive(&m_RWLock.lock);
        m_RWLock.flag = WinRWLock::RW::Wirte;
    }
    
    void RWMutex::unlock() {
        switch (m_RWLock.flag){
            case WinRWLock::RW::Read:
                ReleaseSRWLockShared(&m_RWLock.lock);
                break;
            case WinRWLock::RW::Wirte:
                ReleaseSRWLockExclusive(&m_RWLock.lock);
                break;
        }
    }

    Spinlock::Spinlock() {
        InitializeCriticalSectionAndSpinCount(&m_Mutex, 0);
    }

    Spinlock::~Spinlock() {
        DeleteCriticalSection(&m_Mutex);
    }

    void Spinlock::lock() {
        EnterCriticalSection(&m_Mutex);
    }
    
    void Spinlock::unlock() {
        LeaveCriticalSection(&m_Mutex);
    }
    
    Mutex::Mutex() {
        m_Mutex = CreateMutex(nullptr, false, nullptr);
    }

    Mutex::~Mutex() {
        CloseHandle(m_Mutex);
    }

    void Mutex::lock() {
        WaitForSingleObject(m_Mutex, INFINITE);
    }

    void Mutex::unlock() {
        ReleaseMutex(m_Mutex);
    }

    Semaphore::Semaphore(uint32_t count) {
        m_Semaphore = CreateSemaphore(nullptr, count, 1, nullptr);
    }

    Semaphore::~Semaphore() {
        CloseHandle(m_Semaphore);
    }

    void Semaphore::wait() {
        WaitForSingleObject(m_Semaphore, INFINITE);
    }

    void Semaphore::notify() {
        ReleaseSemaphore(m_Semaphore, 1, nullptr);
    }
}
#endif