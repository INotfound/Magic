#include "Http/HttpServlet.h"

namespace Magic{
namespace Http{

    HttpServlet::HttpServlet(const std::string& name)
        :m_Name{name}{
    }
    const std::string HttpServlet::getName() const{
        return m_Name;
    }

    HttpServletDispatch::HttpServletDispatch()
        :HttpServlet{"HttpServletDispatch"}{
    }
    void HttpServletDispatch::setDeafultServlet(MagicPtr<HttpServlet>& servlet){
        RWMutexWriteLock lock{m_Mutex};
        m_DeafultServlet = std::move(servlet);
    }
    MagicPtr<HttpServlet>& HttpServletDispatch::getMatchedServlet(const std::string& name){
        RWMutexWriteLock lock{m_Mutex};
        auto iter = m_Servlets.find(name);
        if(iter != m_Servlets.end()){
            return iter->second;
        }
        return m_DeafultServlet;
    }
    void HttpServletDispatch::addServlet(const std::string& name,MagicPtr<HttpServlet>& servlet){
        RWMutexWriteLock lock{m_Mutex};
        m_Servlets.emplace(name,std::move(servlet));
    }
    void HttpServletDispatch::handle(const std::shared_ptr<HttpSession>& session,MagicPtr<HttpRequest>& request,MagicPtr<HttpResponse>& response){
        auto& servlet = getMatchedServlet(request->getUrlPath());
        if(servlet){
            servlet->handle(session,request,response);
        }
    }
}
}