/*
 * @Author: INotFound
 * @Date: 2020-06-17 21:39:26
 * @LastEditTime: 2021-01-18 15:13:45
 */ 
#pragma once
#include <list>
#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <typeinfo>
#include <functional>
#include <type_traits>
#include <unordered_map>
#ifndef Safe
    #define Safe std::shared_ptr
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <io.h>
    #undef DELETE

    typedef struct{
        enum class RW {
            Read,
            Wirte
        };
        RW flag;
        SRWLOCK lock;
    }WinRWLock;

    typedef HANDLE               sem_t;
    typedef HANDLE               mutex_t;
    typedef WinRWLock            rwlock_t;
    typedef HINSTANCE            plugin_t;
    typedef CRITICAL_SECTION     spinlock_t;
    #define IS_FILE(Path)        _access(Path,0)
#endif
#if defined(linux) || defined(__linux__)
    #include <dirent.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/time.h>
    #include <execinfo.h>
    #include <semaphore.h>
    #include <sys/syscall.h>

    typedef sem_t                sem_t;
    typedef pthread_mutex_t      mutex_t;
    typedef pthread_spinlock_t   spinlock_t;
    typedef pthread_rwlock_t     rwlock_t;
    typedef void*                plugin_t;
    #define IS_FILE(Path)        access(Path,0)
#endif

class Noncopyable{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

template<class T>
class SingletonPtr{
public:
    static const Safe<T>& GetInstance(){
        static Safe<T> v(std::make_shared<T>());
        return v;
    }
};

template<class T>
const void* CompiletimeIId(){
    return reinterpret_cast<const void*>(&CompiletimeIId<T>);
}

template<class T> struct Safe_Traits{
    typedef void Type;
};

template<class T> struct Safe_Traits<Safe<T>>{
    typedef T Type;
};

template<class T> struct Safe_Traits<const Safe<T>&>{
    typedef T Type;
};