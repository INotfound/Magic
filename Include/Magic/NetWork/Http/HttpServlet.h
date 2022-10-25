/*
 * @Author: INotFound
 * @Date: 2020-02-19 16:07:06
 * @LastEditTime: 2021-02-01 22:21:11
 */
#pragma once
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Mutex.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpSocket.h"

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
        enum { value = sizeof(Check<T>(nullptr)) == sizeof(char) };
    };
    template<typename T,typename Sign>
    class HasBefore{
        typedef char no[2];
        typedef char yes[1];
        template <typename U,U> struct TypeCheck;
        template <typename C> static no& Check(...);
        template <typename C> static yes& Check(TypeCheck<Sign, &C::before>*);
    public:
        enum { value = sizeof(Check<T>(nullptr)) == sizeof(char) };
    };
    template <typename Ret, typename... Args>
    std::tuple<Ret, Args...> FunctionChecker(std::function<Ret(Args...)>);
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
    class IHttpServlet{
        friend class HttpServletDispatch;
    public:
        template<typename T,typename... Args>
        using ClassMemberFunction = void(T::*)(const Safe<HttpSocket>&);
    public:
        virtual ~IHttpServlet();
        template<typename T,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addRoute(const std::string& path,ClassMemberFunction<T> memberFunc);
        template<typename T,typename ...Args,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addRoute(const std::string& path,ClassMemberFunction<T> memberFunc,Args ...args);
        template<typename T,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addMatchRoute(const std::string& path,ClassMemberFunction<T> memberFunc);
        template<typename T,typename ...Args,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addMatchRoute(const std::string& path,ClassMemberFunction<T> memberFunc,Args ...args);
    private:
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
    /**
     * @brief HttpServlet分配器类
     */
    class HttpServletDispatch :public std::enable_shared_from_this<HttpServletDispatch>{
        /// Path RouteHandle(RouteHandle) Before(AspectHandle) After(AspectHandle)
        using RouteMaps = std::unordered_map<std::string,std::tuple<RouteHandle,std::deque<AspectHandle>,std::deque<AspectHandle>>>;
    public:
        /**
         * @brief 处理函数
         * @param request Http请求对象
         * @param response Http响应对象
         * @return: 返回True则成功，返回False则失败
         */
        void handle(const Safe<HttpSocket>& httpSocket);
        /**
         * @brief 添加Servlet对象函数
         * @param path Servlet的路径
         * @param servlet Servlet对象
         */
        void setHttpServlet(const Safe<IHttpServlet>& servlet);
        /**
         * @param path Url 子路径
         * @param type 路由类型
         * @param handle 处理函数
         */
        template<typename T,typename = typename std::enable_if<std::is_same<decltype(FunctionChecker(T())),decltype(FunctionChecker(RouteHandle()))>::value>::type>
        void addRoute(const std::string& path,HttpRouteType routeType,T handle);
        template<typename T,typename ...Args,typename = typename std::enable_if<std::is_same<decltype(FunctionChecker(T())),decltype(FunctionChecker(RouteHandle()))>::value>::type>
        void addRoute(const std::string& path,HttpRouteType routeType,T handle,Args ...args);
    private:
        /**
         * @brief 获取匹配的Routed对应的处理函数
         * @param path Route的路径
         * @return: 返回Route对应的处理函数
         */
        const RouteHandle& getMatchedServlet(const std::string& path);
        /**
         * @param iter 切片迭代器
         * @param aspect  需要添加的切片
         */
        template<typename T,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value || HasAfter<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type>
        void addAspect(const RouteMaps::iterator& iter,const T& aspect);
        template<typename T,typename = typename std::enable_if<HasAfter<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type>
        void addAspectAfter(const RouteMaps::iterator& iter,const T& aspect);
        template<typename T,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type>
        void addAspectBefore(const RouteMaps::iterator& iter,const T& aspect);
        template<typename T,typename ...Args,typename = typename std::enable_if<HasBefore<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value || HasAfter<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type>
        void addAspect(const RouteMaps::iterator& iter,const T& aspect,Args ...args);
        ////////////////////
        template<typename T>
        void addAspectAfter(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasAfter<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type* =nullptr);
        template<typename T>
        void addAspectBefore(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasBefore<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type* =nullptr);
    private:
        RWMutex m_Mutex;
        RouteMaps m_MatchRoutes;
        RouteMaps m_NormalRoutes;
        RouteHandle m_EmptyRouteHandle;
    };

    template<typename T,typename>
    void IHttpServlet::addMatchRoute(const std::string& path,ClassMemberFunction<T> memberFunc){
        if(m_ServletDispatch){
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){(reinterpret_cast<T*>(this)->*memberFunc)(httpSocket);};
            m_ServletDispatch->addRoute(path,HttpRouteType::Match,handle);
        }
    }

    template<typename T,typename ...Args,typename>
    void IHttpServlet::addMatchRoute(const std::string &path, ClassMemberFunction<T> memberFunc,Args ...args) {
        if(m_ServletDispatch){
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){(reinterpret_cast<T*>(this)->*memberFunc)(httpSocket);};
            m_ServletDispatch->addRoute(path,HttpRouteType::Match,handle,args...);
        }
    }

    template<typename T,typename>
    void IHttpServlet::addRoute(const std::string& path,ClassMemberFunction<T> memberFunc){
        if(m_ServletDispatch){
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){(reinterpret_cast<T*>(this)->*memberFunc)(httpSocket);};
            m_ServletDispatch->addRoute(path,HttpRouteType::Normal,handle);
        }
    }

    template<typename T,typename ...Args,typename>
    void IHttpServlet::addRoute(const std::string &path, IHttpServlet::ClassMemberFunction<T> memberFunc,Args... args) {
        if(m_ServletDispatch){
            RouteHandle handle = [this,memberFunc](const Safe<HttpSocket>& httpSocket){(reinterpret_cast<T*>(this)->*memberFunc)(httpSocket);};
            m_ServletDispatch->addRoute(path,HttpRouteType::Normal,handle,args...);
        }
    }

    template<typename T,typename>
    void HttpServletDispatch::addRoute(const std::string& path,HttpRouteType routeType,T handle){
        if(routeType == HttpRouteType::Match){
            auto iter = m_MatchRoutes.find(path);
            if(iter == m_MatchRoutes.end()){
                m_MatchRoutes.emplace(path,std::make_tuple(handle,std::deque<AspectHandle>(),std::deque<AspectHandle>()));
            }
        }else if(routeType == HttpRouteType::Normal){
            auto iter = m_NormalRoutes.find(path);
            if(iter == m_NormalRoutes.end()){
                m_NormalRoutes.emplace(path,std::make_tuple(handle,std::deque<AspectHandle>(),std::deque<AspectHandle>()));
            }
        }
    }

    template<typename T,typename... Args,typename>
    void HttpServletDispatch::addRoute(const std::string& path,HttpRouteType routeType,T handle,Args ...args){
        if(routeType == HttpRouteType::Match){
            auto iter = m_MatchRoutes.find(path);
            if(iter == m_MatchRoutes.end()){
                iter = m_MatchRoutes.emplace(path,std::make_tuple(handle,std::deque<AspectHandle>(),std::deque<AspectHandle>())).first;
            }
            this->addAspect(iter,args...);
        }else if(routeType == HttpRouteType::Normal){
            auto iter = m_NormalRoutes.find(path);
            if(iter == m_NormalRoutes.end()){
                iter = m_NormalRoutes.emplace(path,std::make_tuple(handle,std::deque<AspectHandle>(),std::deque<AspectHandle>())).first;
            }
            this->addAspect(iter,args...);
        }
    }

    template<typename T,typename>
    void HttpServletDispatch::addAspect(const RouteMaps::iterator& iter,const T& aspect){
        if(HasAfter<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value){
            this->addAspectAfter(iter,aspect);
        }
        if(HasBefore<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value){
            this->addAspectBefore(iter,aspect);
        }
    }

    template<typename T,typename>
    void HttpServletDispatch::addAspectAfter(const RouteMaps::iterator& iter,const T& aspect){
        AspectHandle handle = [aspect](const Safe<HttpSocket>& httpSocket){return aspect->after(httpSocket);};
        std::get<2>(iter->second).push_front(handle);
    }

    template<typename T,typename>
    void HttpServletDispatch::addAspectBefore(const RouteMaps::iterator& iter,const T& aspect){
        AspectHandle handle = [aspect](const Safe<HttpSocket>& httpSocket){return aspect->before(httpSocket);};
        std::get<1>(iter->second).push_back(handle);
    }

    template<typename T,typename... Args,typename>
    void HttpServletDispatch::addAspect(const RouteMaps::iterator& iter,const T& aspect,Args ...args){
        this->addAspect(iter,aspect);
        this->addAspect(iter,args...);
    }

    template<typename T>
    void HttpServletDispatch::addAspectAfter(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasAfter<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type*){
    }
    template<typename T>
    void HttpServletDispatch::addAspectBefore(const RouteMaps::iterator&,const T&,typename std::enable_if<!HasBefore<typename Safe_Traits<T>::Type,bool(Safe_Traits<T>::Type::*)(const Safe<HttpSocket>& httpSocket)>::value>::type*){
    }
}
}
}
