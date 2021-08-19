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
    IHttpServlet::IHttpServlet(const std::string& path,const HttpServletType& type)
        :m_Path(path)
        ,m_ServletType(type){
    }
    
    const std::string& IHttpServlet::getPath() const{
        return m_Path;
    }

    const HttpServletType& IHttpServlet::getType() const{
        return m_ServletType;
    }

    IHttpServlet::~IHttpServlet() {
    }

    HttpServletDispatch::HttpServletDispatch(){
    }

    void HttpServletDispatch::addHttpServlet(const Safe<IHttpServlet>& servlet){
        RWMutex::WriteLock lock(m_Mutex);
        switch (servlet->getType()){
            case HttpServletType::Normal:
                m_Servlets.emplace(servlet->getPath(),servlet);
                break;
            case HttpServletType::Global:
                m_GlobServlets.emplace(servlet->getPath(),servlet);
                break;
            default:
                m_DeafultServlet = servlet;
                break;
        }
        MAGIC_INFO() << "HttpServlet Path: " << servlet->getPath() << " Successfully Loaded";
    }

    void HttpServletDispatch::handle(const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
        if(!m_DeafultServlet){
            MAGIC_WARN() << "DeafultServlet Not Set";
        }
        auto& servlet = this->getMatchedServlet(request->getPath());
        if(servlet){
            if(!servlet->handle(request,response))
                m_DeafultServlet->handle(request,response);
        }
    }

    const Safe<IHttpServlet>& HttpServletDispatch::getMatchedServlet(const std::string& path){
        RWMutex::ReadLock lock(m_Mutex);
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
}
