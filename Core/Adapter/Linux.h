#pragma once
#include <sys/ucontext.h>
#if defined(linux) || defined(__linux__)
#include <dlfcn.h>
#include <sys/io.h>
#include <unistd.h>
#include <pthread.h>
#include <execinfo.h>
#include <ucontext.h>
#include <semaphore.h>
#include <sys/syscall.h>


typedef sem_t				sem_t;
typedef pthread_mutex_t		mutex_t;
typedef pthread_spinlock_t	spinlock_t;
typedef pthread_rwlock_t	rwlock_t;
typedef void*				plugin_t;
typedef ucontext_t          fiber_t;
#define IS_FILE(Path)		access(Path,0)
#endif


