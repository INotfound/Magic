/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:13:41
 * @LastEditTime: 2021-02-01 22:21:42
 */
#if defined(linux) || defined(__linux__)
#include <stdexcept>

#include "Magic/Core/Adapter.h"
#include "Magic/Utilty/Mutex.h"

namespace Magic{
    class MutexImpl :public Noncopyable{
    public:
        MutexImpl(){
            pthread_mutex_init(&m_Mutex, nullptr);
        }

        ~MutexImpl(){
            pthread_mutex_destroy(&m_Mutex);
        }

        void lock(){
            pthread_mutex_lock(&m_Mutex);
        }

        void unlock(){
            pthread_mutex_unlock(&m_Mutex);
        }
    private:
        mutex_t m_Mutex;
    };

    class RWMutexImpl :public Noncopyable{
    public:
        RWMutexImpl(){
            pthread_rwlock_init(&m_RWLock, nullptr);
        }

        ~RWMutexImpl(){
            pthread_rwlock_destroy(&m_RWLock);
        }

        void unlock(){
            pthread_rwlock_unlock(&m_RWLock);
        }

        void readLock(){
            pthread_rwlock_rdlock(&m_RWLock);
        }

        void writeLock(){
            pthread_rwlock_wrlock(&m_RWLock);
        }
    private:
        rwlock_t m_RWLock;
    };

    class SpinLockImpl :public Noncopyable{
    public:
        SpinLockImpl(){
            pthread_spin_init(&m_SpinLock, PTHREAD_PROCESS_PRIVATE);
        }

        ~SpinLockImpl(){
            pthread_spin_destroy(&m_SpinLock);
        }

        void lock(){
            pthread_spin_lock(&m_SpinLock);
        }

        void unlock(){
            pthread_spin_unlock(&m_SpinLock);
        }
    private:
        spinlock_t m_SpinLock;
    };

    class SemaphoreImpl :public Noncopyable{
    public:
        explicit SemaphoreImpl(uint32_t count =0){
            if (sem_init(&m_Semaphore, 0, count)) {
                throw std::logic_error("Sem_init error!");
            }
        }

        ~SemaphoreImpl(){
            sem_destroy(&m_Semaphore);
        }

        void wait(){
            if (sem_wait(&m_Semaphore)) {
                throw std::logic_error("Semaphore wait error!");
            }
        }

        void notify(){
            if (sem_post(&m_Semaphore)) {
                throw std::logic_error("Semaphore notify error!");
            }
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
