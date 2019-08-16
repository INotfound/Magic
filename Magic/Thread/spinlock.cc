#include "spinlock.h"

using namespace Magic;

Spinlock::Spinlock() {
    pthread_spin_init(&m_Mutex, 0);
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