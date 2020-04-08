/*
 * @file: HttpServlet.h
 * @Author: INotFound
 * @Date: 2020-02-19 16:07:06
 * @LastEditTime: 2020-04-06 20:15:13
 */
#pragma once
#include <unordered_map>
#include "Http.h"

#include "../Core.h"
#include "../Mutex.h"
namespace Magic{
namespace Http{
    /**
     * @brief: HttpServlet类
     */
    class HttpServlet{
    public:
        explicit HttpServlet(const std::string& path);
        const std::string getName() const;
        virtual bool handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response) =0;
    private:
        std::string m_Name;
    };
    /**
     * @brief: HttpServlet分配器类
     */
    class HttpServletDispatch :HttpServlet{
        typedef RWMutex::WriteLock RWMutexWriteLock;
    public:
        HttpServletDispatch();
        /**
         * @brief: 设置默认Servlet对象函数
         * @param servlet HttpServlet对象
         */
        void setDeafultServlet(Safe<HttpServlet>& servlet);
        /**
         * @brief: 添加Servlet对象函数
         * @param path Servlet的路径
         * @param servlet Servlet对象
         */
        void addHttpServlet(const std::string& path,Safe<HttpServlet>& servlet);
        /**
         * @brief: 添加全局Servlet对象函数
         * @param path Servlet的路径
         * @param servlet Servlet对象
         */
        void addGlobHttpServlet(const std::string& path,Safe<HttpServlet>& servlet);
        /**
         * @brief: 处理函数
         * @param request Http请求对象
         * @param response Http响应对象
         * @return: 返回True则成功，返回False则失败
         */
        bool handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response) override;
    private:
        /**
         * @brief: 获取模糊匹配的Servlet对象函数
         * @param path Servlet的路径
         * @return: 返回Servlet对象
         */
        const Safe<HttpServlet>& getMatchedServlet(const std::string& path);
    private:
        RWMutex m_Mutex;
        Safe<HttpServlet> m_DeafultServlet;
        std::unordered_map<std::string,Safe<HttpServlet>> m_Servlets;
        std::unordered_map<std::string,Safe<HttpServlet>> m_GlobServlets;
    };

}
}