#include "Mutex.h"
#include <stdexcept>
#include "../Log/Log.h"

using namespace Magic;

Semaphore::Semaphore(uint32_t count){
    if(sem_init(&m_Semaphore,0,count)){
        throw std::logic_error("sem_init error!");
    }
}

Semaphore::~Semaphore(){
    sem_destroy(&m_Semaphore);
}

void Semaphore::wait(){
    int val;
    sem_getvalue(&m_Semaphore,&val);
    MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "sem value =" << val;
    if(sem_wait(&m_Semaphore)){
        throw std::logic_error("Semaphore wait error!"); 
    }
}

void Semaphore::notify(){
    if(sem_post(&m_Semaphore)){
        throw std::logic_error("Semaphore notify error!");
    }
}


RWMutex::RWMutex(){
    pthread_rwlock_init(&m_RWLock,nullptr);
}
RWMutex::~RWMutex(){
    pthread_rwlock_destroy(&m_RWLock);
}
void RWMutex::readLock(){
    pthread_rwlock_rdlock(&m_RWLock);
}
void RWMutex::writeLock(){
    pthread_rwlock_wrlock(&m_RWLock);
}
void RWMutex::unlock(){
    pthread_rwlock_unlock(&m_RWLock);
}

