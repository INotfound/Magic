/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:35:10
 * @LastEditTime: 2021-02-01 22:22:26
 */
#if defined(_WIN32) || defined(_WIN64)

#include "Magic/Utilty/Mutex.h"

namespace Magic{
    RWMutex::RWMutex() {
    #if _WIN32_WINNT >= 0x0600
        InitializeSRWLock(&m_RWLock.lock);
    #endif
    }

    RWMutex::~RWMutex() {
    }

    void RWMutex::readLock() {
    #if _WIN32_WINNT >= 0x0600
        AcquireSRWLockShared(&m_RWLock.lock);
    #endif
        m_RWLock.flag = WinRWLock::RW::Read;
    }

    void RWMutex::writeLock() {
    #if _WIN32_WINNT >= 0x0600
        AcquireSRWLockExclusive(&m_RWLock.lock);
    #endif
        m_RWLock.flag = WinRWLock::RW::Wirte;
    }
    
    void RWMutex::unlock() {
        switch (m_RWLock.flag){
            case WinRWLock::RW::Read:
            #if _WIN32_WINNT >= 0x0600
                ReleaseSRWLockShared(&m_RWLock.lock);
            #endif
                break;
            case WinRWLock::RW::Wirte:
            #if _WIN32_WINNT >= 0x0600
                ReleaseSRWLockExclusive(&m_RWLock.lock);
            #endif
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
