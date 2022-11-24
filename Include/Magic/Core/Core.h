/*
 * @Author: INotFound
 * @Date: 2020-06-17 21:39:26
 * @LastEditTime: 2021-02-01 22:07:18
 */
#pragma once

#include <list>
#include <mutex>
#include <deque>
#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <functional>
#include <type_traits>
#include <unordered_map>

#ifndef Safe
    #define Safe std::shared_ptr
#endif

class Noncopyable{
public:
    Noncopyable() = default;

    ~Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;

    Noncopyable& operator=(const Noncopyable&) = delete;
};

template<typename T>
class ObjectWrapper{
public:
    explicit ObjectWrapper(T* self)
        :m_Inner(self){
    }

    T& operator*() const{
        return *m_Inner;
    }

    T* operator->() const{
        return m_Inner;
    }

private:
    T* m_Inner;
};

template<typename T>
class SingletonPtr:public Noncopyable{
public:
    static const Safe<T>& GetInstance(){
        static Safe<T> v(std::make_shared<T>());
        return v;
    }
};

template<typename T,class M,typename = typename std::enable_if<std::is_base_of<T,M>::value>::type>
class SingletonPtrEx:public Noncopyable{
public:
    static const Safe<T>& GetInstance(){
        static Safe<T> v(std::make_shared<M>());
        return v;
    }
};

template<typename T>
const void* CompiletimeIId(){
    return reinterpret_cast<const void*>(&CompiletimeIId<T>);
}

template<typename T>
struct Safe_Traits{
    typedef T Type;
};

template<typename T>
struct Safe_Traits<Safe<T>>{
    typedef T Type;
};

template<typename T>
struct Safe_Traits<const Safe<T>>{
    typedef T Type;
};

template<typename T>
struct Safe_Traits<const Safe<T>&>{
    typedef T Type;
};

template<typename T>
struct Safe_Traits<const Safe<T>&&>{
    typedef T Type;
};
