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
    /**
     * @brief Servlet类型枚举类
     */
    enum class HttpRouteType{
        Match,
        Normal
    };
    class HttpServletDispatch;
    using RouteHandle = std::function<void(const Safe<HttpSocket>&,const Safe<HttpRequest>&,const Safe<HttpResponse>&)>;
    /**
     * @brief IHttpServlet类
     */
    class IHttpServlet{
        friend class HttpServletDispatch;
    public:
        template<typename T,typename... Args>
        using ClassMemberFunction = void(T::*)(const Safe<HttpSocket>&,const Safe<HttpRequest>&,const Safe<HttpResponse>&);
    public:
        virtual ~IHttpServlet();
        template<class T,typename = typename std::enable_if<std::is_base_of<IHttpServlet,T>::value>::type>
        void addRoute(const std::string& path,ClassMemberFunction<T> memberFunc,HttpRouteType type =HttpRouteType::Normal);
    private:
        Safe<HttpServletDispatch> m_ServletDispatch;
    };
    /**
     * @brief HttpServlet分配器类
     */
    class HttpServletDispatch :public std::enable_shared_from_this<HttpServletDispatch>{
    public:
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
        void addRoute(const std::string& path,HttpRouteType type,RouteHandle handle);
        /**
         * @brief 处理函数
         * @param request Http请求对象
         * @param response Http响应对象
         * @return: 返回True则成功，返回False则失败
         */
        void handle(const Safe<HttpSocket>& httpSocket,const Safe<HttpRequest>& httpRequest,const Safe<HttpResponse>& httpResponse);
    private:
        /**
         * @brief 获取匹配的Routed对应的处理函数
         * @param path Route的路径
         * @return: 返回Route对应的处理函数
         */
        const RouteHandle& getMatchedServlet(const std::string& path);
    private:
        RWMutex m_Mutex;
        RouteHandle m_EmptyRouteHandle;
        std::unordered_map<std::string,RouteHandle> m_MatchRoutes;
        std::unordered_map<std::string,RouteHandle> m_NormalRoutes;
    };

    template<class T, typename>
    void IHttpServlet::addRoute(const std::string &path, IHttpServlet::ClassMemberFunction<T> memberFunc,HttpRouteType type) {
        if(m_ServletDispatch){
            m_ServletDispatch->addRoute(path,type,std::bind(memberFunc,reinterpret_cast<T*>(this),std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        }
    }

}
}
}
