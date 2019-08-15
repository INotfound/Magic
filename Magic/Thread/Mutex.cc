#include "Mutex.h"

using namespace Magic;

Mutex::Mutex(){
    pthread_mutex_init(&m_Mutex,nullptr);
}
Mutex::~Mutex(){
    pthread_mutex_destroy(&m_Mutex);
}
void Mutex::lock(){
    pthread_mutex_lock(&m_Mutex);
}
void Mutex::unlock(){
    pthread_mutex_unlock(&m_Mutex);
}
