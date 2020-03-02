#include <regex>
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

    void HttpServletDispatch::addHttpServlet(const std::string& name,Safe<HttpServlet>& servlet){
        RWMutexWriteLock lock(m_Mutex);
        m_Servlets.emplace(name,std::move(servlet));
    }
    void HttpServletDispatch::addGlobHttpServlet(const std::string& name,Safe<HttpServlet>& servlet){
        RWMutexWriteLock lock(m_Mutex);
        m_GlobServlets.emplace(name,std::move(servlet));
    }
    bool HttpServletDispatch::handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
        auto& servlet = getMatchedServlet(request->getPath());
        if(!servlet || ! m_DeafultServlet)
            return false;
        if(!servlet->handle(request,response))
            m_DeafultServlet->handle(request,response);
        return true;
    }
    const Safe<HttpServlet>& HttpServletDispatch::getMatchedServlet(const std::string& name){
        RWMutexWriteLock lock(m_Mutex);
        auto exactlyIter = m_Servlets.find(name);
        if(exactlyIter != m_Servlets.end()){
            return exactlyIter->second;
        }
        std::regex reg;
        auto globIter = m_GlobServlets.begin();
        auto globEnd = m_GlobServlets.end();
        for(; globIter != globEnd; globIter++){
            reg.assign(globIter->first);
            if(std::regex_match(name,reg)){
                return globIter->second;
            }
        }
        return m_DeafultServlet;
    }
}
}