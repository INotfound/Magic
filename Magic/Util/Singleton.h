#pragma once
#include <memory>

namespace Magic{

template <class T>
class Singleton{
public:
    static T* GetInstance(){
        static T v;
        return &v;
    }
};

template <class T>
class SingletonPtr{
public:
    static std::unique_ptr<T> GetInstance(){
        static std::unique_ptr<T> v(new T());
        return v;
    }
};

}