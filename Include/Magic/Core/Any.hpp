/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Any.hpp
 * @Date           : 2023-07-03 18:25
 ******************************************************************************
 */
#pragma once
#if __cplusplus >= 201703L
#include <any>
#else

#include <typeindex>
#include <type_traits>

#include "Magic/Core/Core.hpp"

/*
    form: qicosmos@purecpp.com
*/
namespace std{
    class any{
    public:
        any(void)
            :m_TypeIndex(std::type_index(typeid(void))){
        }

        any(const any& other)
            :m_Pointer(other.clone())
            ,m_TypeIndex(other.m_TypeIndex){
        }

        any(any&& other)
            :m_Pointer(std::move(other.m_Pointer))
            ,m_TypeIndex(other.m_TypeIndex){
        }

        //创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
        template<typename U,class = typename std::enable_if<!std::is_same<typename std::decay<U>::type,any>::value,U>::type>
        any(U&& value)
            :m_Pointer(new Type<typename std::decay<U>::type>(std::forward<U>(value)))
            ,m_TypeIndex(std::type_index(typeid(typename std::decay<U>::type))){
        }

        bool has_value() const noexcept{
            return !bool(m_Pointer);
        }

        template<class U>
        bool Is() const{
            return m_TypeIndex == std::type_index(typeid(U));
        }

        template<typename T>
        friend T& any_cast(const any& other);

        //将any转换为实际的类型
        // template<class U>
        // U& anyCast(){
        //     if (!Is<U>()){
        //         MAGIC_DEBUG() << "Can not cast " << typeid(U).name() << " to " << m_TypeIndex.name();
        //         throw std::bad_cast();
        //     }

        //     auto type = dynamic_cast<Type<U>*> (m_Pointer.get());
        //     return type->m_Value;
        // }

        any& operator=(const any& other){
            if(m_Pointer == other.m_Pointer)
                return *this;

            m_Pointer = other.clone();
            m_TypeIndex = other.m_TypeIndex;
            return *this;
        }

    private:
        class Base{
        public:
            virtual ~Base() = default;

            virtual Safe<Base> clone() const = 0;
        };

        template<typename T>
        class Type:public Base{
        public:
            template<typename U>
            Type(U&& value)
                : m_Value(std::forward<U>(value)){
            }

            Safe<Base> clone() const override{
                return Safe<Base>(new Type<T>(m_Value));
            }

            T m_Value;
        };

        Safe<Base> clone() const{
            if(m_Pointer != nullptr)
                return m_Pointer->clone();
            return nullptr;
        }

        Safe<Base> m_Pointer;
        std::type_index m_TypeIndex;
    };

    template<typename T>
    T& any_cast(const any& other){
        if(!other.Is<T>()){
            std::printf("Can Not Cast %s To %s",typeid(T).name(),other.m_TypeIndex.name());
            throw std::bad_cast();
        }
        auto type = dynamic_cast<any::Type<T>*> (other.m_Pointer.get());
        return type->m_Value;
    }
}
#endif
