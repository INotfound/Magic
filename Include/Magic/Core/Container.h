/*
 * @Author: INotFound
 * @Date: 2020-08-07 18:03:10
 * @LastEditTime: 2021-02-01 22:59:55
 */
#pragma once

#include <utility>

#include "Magic/Core/Core.h"
#include "Magic/Core/Except.h"

namespace Magic{
    class Container:public std::enable_shared_from_this<Container>{
        template<typename Return,typename... Args>
        using Function = Safe<Return>(Container::*)();
        template<typename Instance,typename... Args>
        using ClassMemberFunction = void (Instance::*)(Args...);

        class RegisteredType{
            friend class Container;
            using CreateFunc = std::function<Safe<void>()>;
            using MemberFunc = std::function<void(Safe<Container>&)>;
        public:
            template<typename T,typename... Args>
            RegisteredType& registerProperty(ClassMemberFunction<T,Args...> memberFunction){
                m_MemberFuncs.push_back([this,memberFunction](Safe<Container>& con){
                    ((*std::static_pointer_cast<T>(m_Object)).*memberFunction)(con->resolve<typename Safe_Traits<Args>::Type>()...);
                });
                return (*this);
            }

        private:
            RegisteredType(bool singelton,CreateFunc createFunc)
                :m_IsSingelton(singelton)
                ,m_Object(nullptr)
                ,m_CreateFunc(std::move(createFunc)){
            }

            void resolveMemberFunction(Safe<Container>& con){
                for(auto& callBack :m_MemberFuncs){
                    callBack(con);
                }
            }

        private:
            bool m_IsSingelton;
            Safe<void> m_Object;
            CreateFunc m_CreateFunc;
            std::list<MemberFunc> m_MemberFuncs;
        };

    public:
        template<typename T>
        bool contain(){
            const void* id = CompiletimeIId<T>();
            auto iter = m_RegisteredType.find(id);
            if(iter == m_RegisteredType.end())
                return false;
            return true;
        }

        template<typename T,typename... Args>
        RegisteredType& registerType(bool isSingleton = true){
            return this->registerTypeEx<T,T,Args...>(isSingleton);
        }

        template<typename T,typename M,typename... Args,typename = typename std::enable_if<std::is_constructible<M,Args...>::value
            && (std::is_same<T,M>::value || std::is_base_of<T,M>::value)>::type>
        RegisteredType& registerTypeEx(bool isSingleton = true){
            const void* id = CompiletimeIId<T>();
            const void* raw = CompiletimeIId<M>();
            if(std::is_same<T,M>::value && m_RegisteredType.find(id) != m_RegisteredType.end())
                throw Failure(std::string(typeid(T).name()) + " Is Multiple Registered!");
            Function<T,Args...> createFunc = &Container::invoke<T,M,typename Safe_Traits<Args>::Type...>;
            m_RegisteredType[id].emplace(raw,RegisteredType(isSingleton,[this,createFunc](){ return (this->*createFunc)(); }));
            return m_RegisteredType[id].at(raw);
        }

        template<typename T>
        RegisteredType& registerInstance(const Safe<T>& instance){
            const void* id = CompiletimeIId<T>();
            if(m_RegisteredType.find(id) != m_RegisteredType.end())
                throw Failure(std::string(typeid(T).name()) + " Is Multiple Registered!");
            RegisteredType registerObject(true,[](){ return Safe<void>(); });
            registerObject.m_Object = std::move(instance);
            m_RegisteredType[id].emplace(id,registerObject);
            return m_RegisteredType[id].at(id);
        }

        template<typename T,typename M,typename = typename std::enable_if<std::is_same<T,M>::value || std::is_base_of<T,M>::value>::type>
        RegisteredType& registerInstance(const Safe<M>& instance){
            const void* id = CompiletimeIId<T>();
            const void* raw = CompiletimeIId<M>();
            RegisteredType registerObject(true,[](){ return Safe<void>(); });
            registerObject.m_Object = std::move(instance);
            m_RegisteredType[id].emplace(raw,registerObject);
            return m_RegisteredType[id].at(raw);
        }

        template<typename T>
        Safe<T> resolve(){
            const void* id = CompiletimeIId<T>();

            auto iter = m_RegisteredType.find(id);
            if(iter == m_RegisteredType.end())
                throw Failure(std::string(typeid(T).name()) + " Is Not Registered!");
            auto rawIter = iter->second.begin();
            if(rawIter == iter->second.end())
                throw Failure(std::string(typeid(T).name()) + " Is Not Registered!");
            auto self = this->shared_from_this();
            auto& registeredType = rawIter->second;
            if(registeredType.m_IsSingelton){
                if(!registeredType.m_Object){
                    registeredType.m_Object = registeredType.m_CreateFunc();
                    registeredType.resolveMemberFunction(self);
                }
                return std::static_pointer_cast<T>(registeredType.m_Object);
            }
            auto object = registeredType.m_CreateFunc();
            registeredType.resolveMemberFunction(self);
            return std::static_pointer_cast<T>(object);
        }

        template<typename T,typename M,typename = typename std::enable_if<std::is_same<T,M>::value || std::is_base_of<T,M>::value>::type>
        Safe<T> resolve(){
            const void* id = CompiletimeIId<T>();
            const void* raw = CompiletimeIId<M>();
            auto iter = m_RegisteredType.find(id);
            if(iter == m_RegisteredType.end())
                throw Failure(std::string(typeid(T).name()) + " Is Not Registered!");
            auto rawIter = iter->second.find(raw);
            if(rawIter == iter->second.end())
                throw Failure(std::string(typeid(M).name()) + " Is Not Registered!");
            auto self = this->shared_from_this();
            auto& registeredType = iter->second.at(raw);
            if(registeredType.m_IsSingelton){
                if(!registeredType.m_Object){
                    registeredType.m_Object = registeredType.m_CreateFunc();
                    registeredType.resolveMemberFunction(self);
                }
                return std::static_pointer_cast<T>(registeredType.m_Object);
            }
            auto object = registeredType.m_CreateFunc();
            registeredType.resolveMemberFunction(self);
            return std::static_pointer_cast<T>(object);
        }

        template<typename T = void,typename = typename std::enable_if<std::is_void<T>::value>::type>
        void resolveAll(){
            auto self = this->shared_from_this();
            for(auto& v :m_RegisteredType){
                for(auto& registeredType :v.second){
                    if(registeredType.second.m_IsSingelton){
                        if(!registeredType.second.m_Object){
                            registeredType.second.m_Object = registeredType.second.m_CreateFunc();
                            registeredType.second.resolveMemberFunction(self);
                        }
                    }
                }
            }
        }

        template<typename T = void,typename = typename std::enable_if<!std::is_void<T>::value>::type>
        std::list<Safe<T>> resolveAll(){
            std::list<Safe<T>> objectList;
            auto self = this->shared_from_this();
            auto iter = m_RegisteredType.find(CompiletimeIId<T>());
            if(iter != m_RegisteredType.end()){
                for(auto& registeredType :iter->second){
                    if(registeredType.second.m_IsSingelton){
                        if(!registeredType.second.m_Object){
                            registeredType.second.m_Object = std::move(registeredType.second.m_CreateFunc());
                            registeredType.second.resolveMemberFunction(self);
                        }
                    }
                    objectList.push_back(std::static_pointer_cast<T>(registeredType.second.m_Object));
                }
            }
            return objectList;
        }

    private:
        template<typename T,typename M,typename... Args>
        Safe<T> invoke(){
            return std::make_shared<M>(this->resolve<Args>()...);
        }

    private:
        std::unordered_map<const void*,std::unordered_map<const void*,RegisteredType>> m_RegisteredType;
    };
}
