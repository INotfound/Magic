#pragma once
#if defined(linux) || defined(__linux__)
#include <unistd.h>
#include <pthread.h>
#include <execinfo.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <sys/io.h>
typedef sem_t				sem_t;
typedef pthread_mutex_t		mutex_t;
typedef pthread_spinlock_t	spinlock_t;
typedef pthread_rwlock_t	rwlock_t;
typedef void*				plugin_t;
#endif


