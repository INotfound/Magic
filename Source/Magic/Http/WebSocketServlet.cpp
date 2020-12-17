#include <regex>
#include "Http/WebSocketServlet.h"
namespace Magic{
namespace Http{

    WebSocketServlet::WebSocketServlet(const std::string& name)
        :m_Name(name){
    }
    const std::string WebSocketServlet::getName() const{
        return m_Name;
    }

    WebSocketServletDispatch::WebSocketServletDispatch(){
    }
    void WebSocketServletDispatch::addWebSocketServlet(const std::string& name,Safe<WebSocketServlet>& servlet){
        RWMutexWriteLock lock(m_Mutex);
        m_Servlets.emplace(name,std::move(servlet));
    }
    // bool WebSocketServletDispatch::handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
    //     auto& servlet = this->getMatchedServlet(request->getPath());
    //     if(!servlet || ! m_DeafultServlet)
    //         return false;
    //     if(!servlet->handle(request,response))
    //         m_DeafultServlet->handle(request,response);
    //     return true;
    // }
    // const Safe<WebSocketServlet>& WebSocketServletDispatch::getMatchedServlet(const std::string& name){
    //     RWMutexWriteLock lock(m_Mutex);
    //     auto exactlyIter = m_Servlets.find(name);
    //     if(exactlyIter != m_Servlets.end()){
    //         return exactlyIter->second;
    //     }
    // }
}
}