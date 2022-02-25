/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:35:10
 * @LastEditTime: 2021-02-01 22:22:26
 */
#if defined(_WIN32) || defined(_WIN64)
#include "Magic/Core/Adapter.h"
#include "Magic/Utilty/Mutex.h"

namespace Magic{
    class MutexImpl :public Noncopyable{
    public:
        MutexImpl(){
            m_Mutex = CreateMutex(nullptr, false, nullptr);
        }

        ~MutexImpl(){
            CloseHandle(m_Mutex);
        }

        void lock(){
            WaitForSingleObject(m_Mutex, INFINITE);
        }

        void unlock(){
            ReleaseMutex(m_Mutex);
        }
    private:
        mutex_t m_Mutex;
    };

    class RWMutexImpl :public Noncopyable{
    public:
        RWMutexImpl(){
        #if _WIN32_WINNT >= 0x0600
            InitializeSRWLock(&m_RWLock.lock);
        #endif
        }

        ~RWMutexImpl(){
        }

        void unlock(){
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

        void readLock(){
        #if _WIN32_WINNT >= 0x0600
            AcquireSRWLockShared(&m_RWLock.lock);
        #endif
            m_RWLock.flag = WinRWLock::RW::Read;
        }

        void writeLock(){
        #if _WIN32_WINNT >= 0x0600
            AcquireSRWLockExclusive(&m_RWLock.lock);
        #endif
            m_RWLock.flag = WinRWLock::RW::Wirte;
        }
    private:
        rwlock_t m_RWLock;
    };

    class SpinLockImpl :public Noncopyable{
    public:
        SpinLockImpl(){
            InitializeCriticalSectionAndSpinCount(&m_SpinLock, 0);
        }

        ~SpinLockImpl(){
            DeleteCriticalSection(&m_SpinLock);
        }

        void lock(){
            EnterCriticalSection(&m_SpinLock);
        }

        void unlock(){
            LeaveCriticalSection(&m_SpinLock);
        }
    private:
        spinlock_t m_SpinLock;
    };

    class SemaphoreImpl :public Noncopyable{
    public:
        explicit SemaphoreImpl(uint32_t count =0){
            m_Semaphore = CreateSemaphore(nullptr, count, 1, nullptr);
        }

        ~SemaphoreImpl(){
            CloseHandle(m_Semaphore);
        }

        void wait(){
            WaitForSingleObject(m_Semaphore, INFINITE);
        }

        void notify(){
            ReleaseSemaphore(m_Semaphore, 1, nullptr);
        }
    private:
        sem_t m_Semaphore;
    };

    Mutex::Mutex()
        :m_Mutex(std::make_shared<MutexImpl>()){
    }

    void Mutex::lock() {
        m_Mutex->lock();
    }

    void Mutex::unlock() {
        m_Mutex->unlock();
    }

    RWMutex::RWMutex()
        :m_RWLock(std::make_shared<RWMutexImpl>()){
    }

    void RWMutex::unlock() {
        m_RWLock->unlock();
    }
    void RWMutex::readLock() {
        m_RWLock->readLock();
    }
    void RWMutex::writeLock() {
        m_RWLock->writeLock();
    }

    SpinLock::SpinLock()
        :m_SpinLock(std::make_shared<SpinLockImpl>()){
    }

    void SpinLock::lock() {
        m_SpinLock->lock();
    }
    void SpinLock::unlock() {
        m_SpinLock->unlock();
    }

    Semaphore::Semaphore(uint32_t count)
        :m_Semaphore(std::make_shared<SemaphoreImpl>(count)){
    }

    void Semaphore::wait() {
        m_Semaphore->wait();
    }

    void Semaphore::notify() {
        m_Semaphore->notify();
    }
}
#endif
