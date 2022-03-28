/*
 * @Author: INotFound
 * @Date: 2020-03-15 17:45:01
 * @LastEditTime: 2021-02-01 22:24:51
 */
#include <regex>

#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/Http/HttpServlet.h"

namespace Magic{
namespace NetWork{
namespace Http{
    IHttpServlet::~IHttpServlet() =default;

    void HttpServletDispatch::setHttpServlet(const Safe<IHttpServlet>& servlet){
        RWMutex::WriteLock lock(m_Mutex);
        servlet->m_ServletDispatch = this->shared_from_this();
    }

    void HttpServletDispatch::addRoute(const std::string& path,HttpRouteType type,RouteHandle handle){
        switch(type){
            case HttpRouteType::Match:
                m_MatchRoutes.emplace(path,handle);
                break;
            case HttpRouteType::Normal:
                m_NormalRoutes.emplace(path,handle);
                break;
        }
        MAGIC_INFO() << "HttpServlet Path: " << path << " Successfully Loaded";
    }

    void HttpServletDispatch::handle(const Safe<HttpSocket>& httpSocket,const Safe<HttpRequest>& httpRequest,const Safe<HttpResponse>& httpResponse){
        const auto& handle = this->getMatchedServlet(httpRequest->getPath());
        if(handle){
            handle(httpSocket,httpRequest,httpResponse);
        }else{
            httpResponse->setStatus(HttpStatus::NOT_FOUND);
            httpResponse->setBody("<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr><center>Magic/2.0.0</center></body></html>");
            httpSocket->sendResponse(httpResponse);
        };
    }

    const RouteHandle& HttpServletDispatch::getMatchedServlet(const std::string& path){
        RWMutex::ReadLock lock(m_Mutex);
        auto exactlyIter = m_NormalRoutes.find(path);
        auto exactlyEnd = m_NormalRoutes.end();
        if(exactlyIter != exactlyEnd){
            return exactlyIter->second;
        }
        std::regex reg;
        auto matchIter = m_MatchRoutes.begin();
        auto matchEnd = m_MatchRoutes.end();
        for(; matchIter != matchEnd; matchIter++){
            reg.assign(matchIter->first);
            if(std::regex_match(path,reg)){
                return matchIter->second;
            }
        }
        return m_EmptyRouteHandle;
    }
}

}
}
