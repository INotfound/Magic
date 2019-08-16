#include "rwMutex.h"
#include <stdexcept>
#include "../Log/Log.h"

using namespace Magic;

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

