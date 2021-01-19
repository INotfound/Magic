/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:13:41
 * @LastEditTime: 2020-09-28 16:27:13
 */
#if defined(linux) || defined(__linux__)
#include <stdexcept>

#include "Core/Mutex.h"

namespace Magic{
    RWMutex::RWMutex() {
        pthread_rwlock_init(&m_RWLock, nullptr);
    }
    RWMutex::~RWMutex() {
        pthread_rwlock_destroy(&m_RWLock);
    }
    void RWMutex::readLock() {
        pthread_rwlock_rdlock(&m_RWLock);
    }
    void RWMutex::writeLock() {
        pthread_rwlock_wrlock(&m_RWLock);
    }
    void RWMutex::unlock() {
        pthread_rwlock_unlock(&m_RWLock);
    }

    Spinlock::Spinlock() {
        pthread_spin_init(&m_Mutex, PTHREAD_PROCESS_PRIVATE);
    }
    Spinlock::~Spinlock() {
        pthread_spin_destroy(&m_Mutex);
    }
    void Spinlock::lock() {
        pthread_spin_lock(&m_Mutex);
    }
    void Spinlock::unlock() {
        pthread_spin_unlock(&m_Mutex);
    }

    Mutex::Mutex() {
        pthread_mutex_init(&m_Mutex, nullptr);
    }
    Mutex::~Mutex() {
        pthread_mutex_destroy(&m_Mutex);
    }
    void Mutex::lock() {
        pthread_mutex_lock(&m_Mutex);
    }
    void Mutex::unlock() {
        pthread_mutex_unlock(&m_Mutex);
    }

    Semaphore::Semaphore(uint32_t count) {
        if (sem_init(&m_Semaphore, 0, count)) {
            throw std::logic_error("Sem_init error!");
        }
    }
    Semaphore::~Semaphore() {
        sem_destroy(&m_Semaphore);
    }
    void Semaphore::wait() {
        if (sem_wait(&m_Semaphore)) {
            throw std::logic_error("Semaphore wait error!");
        }
    }
    void Semaphore::notify() {
        if (sem_post(&m_Semaphore)) {
            throw std::logic_error("Semaphore notify error!");
        }
    }
}
#endif