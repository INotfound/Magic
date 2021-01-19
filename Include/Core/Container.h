/*
 * @Author: INotFound
 * @Date: 2020-08-07 18:03:10
 * @LastEditTime: 2021-01-18 15:09:11
 */
#pragma once
#include "Core/Core.h"

namespace Magic{
    #ifdef PERFORMANCE
        static std::chrono::high_resolution_clock::time_point g_Time; 
    #endif
    class Container :public std::enable_shared_from_this<Container>{
        template<typename Return,typename... Args>
        using Function = Safe<Return>(Container::*)();
        template<typename Instance,typename... Args>
        using ClassMemberFunction = void(Instance::*)(Args...);
        
        class RegisteredType {
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
            RegisteredType(const void* type, bool singelton, CreateFunc createFunc)
                :m_Type(type)
                ,m_IsSingelton(singelton)
                ,m_Object(nullptr)
                ,m_CreateFunc(createFunc){
            }
            void resolveMemberFunction(Safe<Container>& con){
                for(auto& callBack :m_MemberFuncs){
                    callBack(con);
                }
            }
        private:
            const void* m_Type;
            bool m_IsSingelton;
            Safe<void> m_Object;
            CreateFunc m_CreateFunc;
            std::list<MemberFunc> m_MemberFuncs;
        };

    public:
        template<typename T,typename... Args>
        RegisteredType& registerType(bool isSingelton = true){
            return this->registerTypeEx<T,T,Args...>(isSingelton);
        }

        template<typename T,typename M,typename... Args,typename = typename std::enable_if<std::is_same<T,M>::value || std::is_base_of<T,M>::value>::type>
        RegisteredType& registerTypeEx(bool isSingelton = true){
            const void* id = CompiletimeIId<T>();
            if(std::is_same<T,M>::value && m_RegisteredType.find(id) != m_RegisteredType.end())
                throw std::logic_error(std::string(typeid(T).name()) + " Is Multiple Registered!!!");
            Function<T,Args...> createFunc = &Container::invoke<T,M,Args...>;
            m_RegisteredType[id].push_back(RegisteredType(id,isSingelton,[this,createFunc](){return (this->*createFunc)();}));
            return m_RegisteredType[id].back();
        }

        template<typename T>
        RegisteredType& registerInstance(const Safe<T>& instance){
            const void* id = CompiletimeIId<T>();
            if(m_RegisteredType.find(id) != m_RegisteredType.end())
                throw std::logic_error(std::string(typeid(T).name()) + " Is Multiple Registered!!!");
            RegisteredType registerObject(id,true,[](){return Safe<void>();});
            registerObject.m_Object = std::move(instance);
            m_RegisteredType[id].push_back(registerObject);
            return m_RegisteredType[id].back();
        }

        template<typename T,typename M>
        RegisteredType& registerInstance(const Safe<M>& instance){
            const void* id = CompiletimeIId<T>();
            RegisteredType registerObject(id,true,[](){return Safe<void>();});
            registerObject.m_Object = std::move(instance);
            m_RegisteredType[id].push_back(registerObject);
            return m_RegisteredType[id].back();
        }

        template<typename T>
        Safe<T> resolve() {
            const void* id = CompiletimeIId<T>();
            auto self = this->shared_from_this();
            auto iter = m_RegisteredType.find(id);
            if(iter == m_RegisteredType.end())
                throw std::logic_error(std::string(typeid(T).name()) + " Is Not Registered!!!");
            auto& registeredType = iter->second.front();
            if(registeredType.m_IsSingelton){
                if(!registeredType.m_Object){
                    registeredType.m_Object = std::move(registeredType.m_CreateFunc());
                    registeredType.resolveMemberFunction(self);
                #ifdef PERFORMANCE
                    auto time = std::chrono::high_resolution_clock::now();
                    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(time - g_Time).count();
                    std::string unit = (elapsedTime > 10000000) ? "S" : "Ms";
                    unit = (elapsedTime > 1000) ? "Ms" : "μs";
                    elapsedTime = (elapsedTime > 1000) ? elapsedTime/1000 : elapsedTime;
                    elapsedTime = (elapsedTime > 1000) ? elapsedTime/1000 : elapsedTime;
                    std::printf("@Class Elapsed Time: %ld(%s)\n    [%s]\n",elapsedTime,unit.c_str(),typeid(T).name());
                    g_Time = std::chrono::high_resolution_clock::now();
                #endif
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
                    if(registeredType.m_IsSingelton){
                        if(!registeredType.m_Object){
                            registeredType.m_Object = std::move(registeredType.m_CreateFunc());
                            registeredType.resolveMemberFunction(self);
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
                    if(registeredType.m_IsSingelton){
                        if(!registeredType.m_Object){
                            registeredType.m_Object = std::move(registeredType.m_CreateFunc());
                            registeredType.resolveMemberFunction(self);
                        #ifdef PERFORMANCE
                            auto time = std::chrono::high_resolution_clock::now();
                            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(time - g_Time).count();
                            std::string unit = (elapsedTime > 10000000) ? "S" : "Ms";
                            unit = (elapsedTime > 1000) ? "Ms" : "μs";
                            elapsedTime = (elapsedTime > 1000) ? elapsedTime/1000 : elapsedTime;
                            elapsedTime = (elapsedTime > 1000) ? elapsedTime/1000 : elapsedTime;
                            std::printf("@Class Elapsed Time: %ld(%s)\n    [%s]\n",elapsedTime,unit.c_str(),typeid(T).name());
                            g_Time = std::chrono::high_resolution_clock::now();
                        #endif
                        }
                    }
                    objectList.push_back(std::static_pointer_cast<T>(registeredType.m_Object));
                }
            }
            return objectList;
        }
    private:
        template<typename T, typename M, typename... Args>
        Safe<T> invoke(){
        #ifdef PERFORMANCE
            g_Time = std::chrono::high_resolution_clock::now();
        #endif
            return std::make_shared<M>(this->resolve<Args>()...); 
        }
    private:
        std::unordered_map<const void*,std::list<RegisteredType>> m_RegisteredType;
    };
}