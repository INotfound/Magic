/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpServlet.hpp
 * @Date           : 2023-07-03 18:28
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Http/Http.hpp"
#include "Magic/NetWork/Http/HttpSocket.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    template<typename T,typename Sign>
    class HasAfter{
        typedef char no[2];
        typedef char yes[1];
        template <typename U,U> struct TypeCheck;
        template <typename C> static no& Check(...);
        template <typename C> static yes& Check(TypeCheck<Sign, &C::after>*);
    public:
        enum{ value = sizeof(Check<T>(nullptr)) == sizeof(char) };
    };

    template<typename T,typename Sign>
    class HasBefore{
        typedef char no[2];
        typedef char yes[1];
        template <typename U,U> struct TypeCheck;
        template <typename C> static no& Check(...);
        template <typename C> static yes& Check(TypeCheck<Sign, &C::before>*);
    public:
        enum{ value = sizeof(Check<T>(nullptr)) == sizeof(char) };
    };

    template<typename Ret,typename... Args>
    std::tuple<Ret,Args...> FunctionChecker(std::function<Ret(Args...)>);

    /**
     * @brief Servlet类型枚举类
     */
    enum class HttpRouteType{
        Match,
        Normal
    };

    class HttpServletDispatch;

    using RouteHandle = std::function<void(const Safe<HttpSocket>&)>;
    using AspectHandle = std::function<bool(const Safe<HttpSocket>&)>;

    /**
     * @brief IHttpServlet类
     */
    class IHttpServlet:public Noncopyable{
        friend class HttpServletDispatch;

    public:
        template<typename T,typename... Args>
        using ClassMemberFunction = void (T::*)(const Safe<HttpSocket>&);
    public:
        virtual ~IHttpServlet();

        /**
         * @brief 添加路由处理函数
         * @param path 需要处理的路由路径
         * @param memberFunc 处理路由的成员函数
         */
        template<typename T,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc);

        /**
         * @brief 添加路由处理函数
         * @param path 需要处理的路由路径
         * @param memberFunc 处理路由的成员函数
         */
        template<typename T,typename ...Args,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc,Args ...args);

        /**
         * @brief 添加路由模糊匹配处理函数
         * @param path 需要处理的路由正则路径
         * @param memberFunc 处理路由的成员函数
         */
        template<typename T,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addMatchRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc);

        /**
         * @brief 添加路由模糊匹配处理函数
         * @param path 需要处理的路由正则路径
         * @param memberFunc 处理路由的成员函数
         */
        template<typename T,typename ...Args,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addMatchRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc,Args ...args);

    private:
        Safe<HttpServletDispatch> m_ServletDispatch;
    };

    /**
     * @brief HttpServlet分配器类
     */
    class HttpServletDispatch:public Noncopyable,public std::enable_shared_from_this<HttpServletDispatch>{
        /// Path | RouteHandle(RouteHandle) | Before(AspectHandle) | After(AspectHandle)
        using RouteMaps = std::unordered_map<std::string,std::tuple<RouteHandle,std::deque<AspectHandle>,std::deque<AspectHandle>>>;
    public:
        /**
         * @brief 处理函数
         * @param httpSocket Http连接对象
         */
        void handle(const Safe<HttpSocket>& httpSocket);

        /**
         * @brief 添加Servlet对象函数
         * @param servlet 控制器
         */
        void setHttpServlet(const Safe<IHttpServlet>& servlet);

        /**
         * @brief 添加路由
         * @param path Url子路径
         * @param routeType 路由类型
         * @param handle 处理函数
         */
        template<typename T,typename = typename std::enable_if<std::is_same<decltype(FunctionChecker(T())),decltype(FunctionChecker(RouteHandle()))>::value>::type>
        void addRoute(const Magic::StringView& path,HttpRouteType routeType,T handle);

        /**
         * @brief 添加路由
         * @param path Url子路径
         * @param routeType 路由类型
         * @param handle 处理函数
         * @param args 多个Aspect处理对象
         */
        template<typename T,typename ...Args,typename = typename std::enable_if<std::is_same<decltype(FunctionChecker(T())),decltype(FunctionChecker(RouteHandle()))>::value>::type>
        void addRoute(const Magic::StringView& path,HttpRouteType routeType,T handle,Args ...args);

        /**
         * @brief 添加全局的切片
         * @param aspect Aspect处理对象
         */
        template<typename T,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value || HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addGlobalAspect(const T& aspect);

        /**
         * @brief 添加全局的切片
         * @param aspect Aspect处理对象
         * @param args 多个Aspect处理对象
         */
        template<typename T,typename ...Args,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value || HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addGlobalAspect(const T& aspect,Args ...args);

    private:
        template<typename T,typename = typename std::enable_if<HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addGlobalAspectAfter(const T& aspect);

        template<typename T,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addGlobalAspectBefore(const T& aspect);

        template<typename T,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value || HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addAspect(const RouteMaps::iterator& iter,const T& aspect);

        template<typename T,typename = typename std::enable_if<HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addAspectAfter(const RouteMaps::iterator& iter,const T& aspect);

        template<typename T,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addAspectBefore(const RouteMaps::iterator& iter,const T& aspect);

        template<typename T,typename ...Args,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value || HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type>
        void addAspect(const RouteMaps::iterator& iter,const T& aspect,Args ...args);

        ////////////////////
        template<typename T>
        void addGlobalAspectAfter(const T&,typename std::enable_if<!HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type* = nullptr);

        template<typename T>
        void addGlobalAspectBefore(const T&,typename std::enable_if<!HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type* = nullptr);

        template<typename T>
        void addAspectAfter(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type* = nullptr);

        template<typename T>
        void addAspectBefore(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type* = nullptr);
        ////////////////////
    private:
        std::mutex m_Mutex;
        RouteMaps m_MatchRoutes;
        RouteMaps m_NormalRoutes;
        std::deque<AspectHandle> m_AspectAfters;
        std::deque<AspectHandle> m_AspectBefores;
    };

    template<typename T,typename>
    void IHttpServlet::addMatchRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc){
        if(m_ServletDispatch){
        #if __cplusplus >= 201402L
            RouteHandle handle = [this,memberFunc = std::move(memberFunc)](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #else
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #endif
            m_ServletDispatch->addRoute(path,HttpRouteType::Match,std::move(handle));
        }
    }

    template<typename T,typename ...Args,typename>
    void IHttpServlet::addMatchRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc,Args ...args){
        if(m_ServletDispatch){
        #if __cplusplus >= 201402L
            RouteHandle handle = [this,memberFunc = std::move(memberFunc)](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #else
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #endif
            m_ServletDispatch->addRoute(path,HttpRouteType::Match,std::move(handle),args...);
        }
    }

    template<typename T,typename>
    void IHttpServlet::addRoute(const Magic::StringView& path,ClassMemberFunction<T> memberFunc){
        if(m_ServletDispatch){
        #if __cplusplus >= 201402L
            RouteHandle handle = [this,memberFunc = std::move(memberFunc)](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #else
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #endif
            m_ServletDispatch->addRoute(path,HttpRouteType::Normal,std::move(handle));
        }
    }

    template<typename T,typename ...Args,typename>
    void IHttpServlet::addRoute(const Magic::StringView& path,IHttpServlet::ClassMemberFunction<T> memberFunc,Args... args){
        if(m_ServletDispatch){
        #if __cplusplus >= 201402L
            RouteHandle handle = [this,memberFunc = std::move(memberFunc)](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #else
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){ (reinterpret_cast<T*>(this)->*memberFunc)(httpSocket); };
        #endif
            m_ServletDispatch->addRoute(path,HttpRouteType::Normal,std::move(handle),args...);
        }
    }

    template<typename T,typename>
    void HttpServletDispatch::addRoute(const Magic::StringView& path,HttpRouteType routeType,T handle){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(routeType == HttpRouteType::Match){
            auto iter = m_MatchRoutes.find(std::string(path.data(),path.size()));
            if(iter == m_MatchRoutes.end()){
                m_MatchRoutes.emplace(std::string(path.data(),path.size()),std::make_tuple(std::move(handle),std::deque<AspectHandle>(),std::deque<AspectHandle>()));
            }
        }else if(routeType == HttpRouteType::Normal){
            auto iter = m_NormalRoutes.find(std::string(path.data(),path.size()));
            if(iter == m_NormalRoutes.end()){
                m_NormalRoutes.emplace(std::string(path.data(),path.size()),std::make_tuple(std::move(handle),std::deque<AspectHandle>(),std::deque<AspectHandle>()));
            }
        }
    }

    template<typename T,typename... Args,typename>
    void HttpServletDispatch::addRoute(const Magic::StringView& path,HttpRouteType routeType,T handle,Args ...args){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(routeType == HttpRouteType::Match){
            auto iter = m_MatchRoutes.find(std::string(path.data(),path.size()));
            if(iter == m_MatchRoutes.end()){
                iter = m_MatchRoutes.emplace(std::string(path.data(),path.size()),std::make_tuple(std::move(handle),std::deque<AspectHandle>(),std::deque<AspectHandle>())).first;
            }
            this->addAspect(iter,args...);
        }else if(routeType == HttpRouteType::Normal){
            auto iter = m_NormalRoutes.find(std::string(path.data(),path.size()));
            if(iter == m_NormalRoutes.end()){
                iter = m_NormalRoutes.emplace(std::string(path.data(),path.size()),std::make_tuple(std::move(handle),std::deque<AspectHandle>(),std::deque<AspectHandle>())).first;
            }
            this->addAspect(iter,args...);
        }
    }

    template<typename T,typename>
    void HttpServletDispatch::addGlobalAspect(const T& aspect){
        if(HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value){
            this->addGlobalAspectAfter(aspect);
        }
        if(HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value){
            this->addGlobalAspectBefore(aspect);
        }
    }

    template<typename T,typename... Args,typename>
    void HttpServletDispatch::addGlobalAspect(const T& aspect,Args ...args){
        this->addGlobalAspect(aspect);
        this->addGlobalAspect(args...);
    }

    template<typename T,typename>
    void HttpServletDispatch::addGlobalAspectAfter(const T& aspect){
        std::lock_guard<std::mutex> locker(m_Mutex);
        AspectHandle handle = [aspect](const Safe<HttpSocket>& httpSocket){ return aspect->after(httpSocket); };
        m_AspectAfters.push_front(std::move(handle));
    }

    template<typename T,typename>
    void HttpServletDispatch::addGlobalAspectBefore(const T& aspect){
        std::lock_guard<std::mutex> locker(m_Mutex);
        AspectHandle handle = [aspect](const Safe<HttpSocket>& httpSocket){ return aspect->before(httpSocket); };
        m_AspectBefores.push_back(std::move(handle));
    }

    template<typename T,typename>
    void HttpServletDispatch::addAspect(const RouteMaps::iterator& iter,const T& aspect){
        if(HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value){
            this->addAspectAfter(iter,aspect);
        }
        if(HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value){
            this->addAspectBefore(iter,aspect);
        }
    }

    template<typename T,typename>
    void HttpServletDispatch::addAspectAfter(const RouteMaps::iterator& iter,const T& aspect){
        AspectHandle handle = [aspect](const Safe<HttpSocket>& httpSocket){ return aspect->after(httpSocket); };
        std::get<2>(iter->second).push_front(std::move(handle));
    }

    template<typename T,typename>
    void HttpServletDispatch::addAspectBefore(const RouteMaps::iterator& iter,const T& aspect){
        AspectHandle handle = [aspect](const Safe<HttpSocket>& httpSocket){ return aspect->before(httpSocket); };
        std::get<1>(iter->second).push_back(std::move(handle));
    }

    template<typename T,typename... Args,typename>
    void HttpServletDispatch::addAspect(const RouteMaps::iterator& iter,const T& aspect,Args ...args){
        this->addAspect(iter,aspect);
        this->addAspect(iter,args...);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    void HttpServletDispatch::addGlobalAspectAfter(const T&,typename std::enable_if<!HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type*){
    }

    template<typename T>
    void HttpServletDispatch::addGlobalAspectBefore(const T&,typename std::enable_if<!HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type*){
    }

    template<typename T>
    void HttpServletDispatch::addAspectAfter(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasAfter<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type*){
    }

    template<typename T>
    void HttpServletDispatch::addAspectBefore(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasBefore<typename Safe_Traits<T>::Type,bool (Safe_Traits<T>::Type::*)(const Safe<HttpSocket>&)>::value>::type*){
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
