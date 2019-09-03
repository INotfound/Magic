#pragma once
#include <memory>
#include "../Define.h"
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
    static MagicPtr<T> GetInstance(){
        static MagicPtr<T> v(new T());
        return v;
    }
};

}
