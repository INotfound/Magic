/*
 * @Author: INotFound
 * @Date: 2020-02-19 16:07:06
 * @LastEditTime: 2021-02-01 22:21:11
 */
#pragma once
#include "Magic/Core/Core.h"
#include "Magic/Utilty/Mutex.h"
#include "Magic/NetWork/Http/Http.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief: Servlet类型枚举类
     */
    enum class HttpServletType{
        Normal,
        Global,
        Deafult,
    };
    /**
     * @brief: IHttpServlet类
     */
    class IHttpServlet{
    public:
        IHttpServlet(const std::string& path,const HttpServletType& type);
        const std::string& getPath() const;
        const HttpServletType& getType() const;
        virtual bool handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response) =0;
    private:
        std::string m_Path;
        HttpServletType m_ServletType;
    };
    /**
     * @brief: HttpServlet分配器类
     */
    class HttpServletDispatch{
    public:
        HttpServletDispatch();
        /**
         * @brief: 添加Servlet对象函数
         * @param path Servlet的路径
         * @param servlet Servlet对象
         */
        void addHttpServlet(const Safe<IHttpServlet>& servlet);
        /**
         * @brief: 处理函数
         * @param request Http请求对象
         * @param response Http响应对象
         * @return: 返回True则成功，返回False则失败
         */
        void handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response);
    private:
        /**
         * @brief: 获取模糊匹配的Servlet对象函数
         * @param path Servlet的路径
         * @return: 返回Servlet对象
         */
        const Safe<IHttpServlet>& getMatchedServlet(const std::string& path);
    private:
        RWMutex m_Mutex;
        Safe<IHttpServlet> m_DeafultServlet;
        std::unordered_map<std::string,Safe<IHttpServlet>> m_Servlets;
        std::unordered_map<std::string,Safe<IHttpServlet>> m_GlobServlets;
    };

}
}
}
