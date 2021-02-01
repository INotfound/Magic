/*
 * @Author: INotFound
 * @Date: 2020-06-17 21:39:26
 * @LastEditTime: 2021-02-01 22:07:18
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