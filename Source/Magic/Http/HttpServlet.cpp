/*
 * @File: HttpServlet.cpp
 * @Author: INotFound
 * @Date: 2020-03-15 17:45:01
 * @LastEditTime : 2020-05-08 20:17:21
 */
#include <regex>
#include "Macro.h"
#include "Http/HttpServlet.h"
namespace Magic{
namespace Http{

    HttpServlet::HttpServlet(const std::string& name)
        :m_Name(name){
    }
    const std::string HttpServlet::getName() const{
        return m_Name;
    }

    HttpServletDispatch::HttpServletDispatch()
        :HttpServlet("HttpServletDispatch"){
    }
    void HttpServletDispatch::setDeafultServlet(Safe<HttpServlet>& servlet){
        RWMutexWriteLock lock(m_Mutex);
        m_DeafultServlet = std::move(servlet);
    }

    void HttpServletDispatch::addHttpServlet(const std::string& path,Safe<HttpServlet>& servlet){
        RWMutexWriteLock lock(m_Mutex);
        m_Servlets.emplace(path,std::move(servlet));
    }
    void HttpServletDispatch::addGlobHttpServlet(const std::string& path,Safe<HttpServlet>& servlet){
        RWMutexWriteLock lock(m_Mutex);
        m_GlobServlets.emplace(path,std::move(servlet));
    }
    bool HttpServletDispatch::handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
        auto& servlet = this->getMatchedServlet(request->getPath());
        if(!m_DeafultServlet){
            MAGIC_DEBUG() << "DeafultServlet not set";
            return false;
        }
        if(servlet){
            if(!servlet->handle(request,response))
                m_DeafultServlet->handle(request,response);
        }
        return true;
    }
    const Safe<HttpServlet>& HttpServletDispatch::getMatchedServlet(const std::string& path){
        RWMutexWriteLock lock(m_Mutex);
        auto exactlyIter = m_Servlets.find(path);
        if(exactlyIter != m_Servlets.end()){
            return exactlyIter->second;
        }
        std::regex reg;
        auto globIter = m_GlobServlets.begin();
        auto globEnd = m_GlobServlets.end();
        for(; globIter != globEnd; globIter++){
            reg.assign(globIter->first);
            if(std::regex_match(path,reg)){
                return globIter->second;
            }
        }
        return m_DeafultServlet;
    }
}
}