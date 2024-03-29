/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Core.hpp
 * @Date           : 2023-07-03 18:26
 ******************************************************************************
 */
#pragma once

#include <list>
#include <array>
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

#include "Magic/Core/Except.hpp"
#include "Magic/Core/StringView.hpp"
#include "Magic/Core/SystemUtils.hpp"

#ifndef Safe
    #define Safe std::shared_ptr
#endif

namespace Magic{
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
            if(!m_Inner)
                throw Failure("Access Null Pointer!");
            return *m_Inner;
        }

        T* operator->() const{
            if(!m_Inner)
                throw Failure("Access Null Pointer!");
            return m_Inner;
        }

    private:
        T* m_Inner = nullptr;
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
    constexpr const void* CompileTimeIId() noexcept{
        return reinterpret_cast<const void*>(&CompileTimeIId<T>);
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
}


