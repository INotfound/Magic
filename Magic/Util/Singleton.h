#pragma once
#include <memory>
#include "Macro.h"
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
    static Ptr<T> GetInstance(){
        static Ptr<T> v(new T());
        return v;
    }
};

}
