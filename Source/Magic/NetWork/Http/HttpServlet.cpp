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

    void HttpServletDispatch::handle(const Safe<HttpSocket>& httpSocket){
        const auto& httpPath = httpSocket->getRequest()->getPath();
        RWMutex::ReadLock lock(m_Mutex);
        auto exactlyIter = m_NormalRoutes.find(httpPath);
        auto exactlyEnd = m_NormalRoutes.end();
        if(exactlyIter != exactlyEnd){
            const auto& vectorAfter = std::get<2>(exactlyIter->second);
            const auto& vectorBefore = std::get<1>(exactlyIter->second);
            for(const auto& func : vectorBefore){
                if(!func(httpSocket))
                    return;
            }
            std::get<0>(exactlyIter->second)(httpSocket);
            for(const auto& func : vectorAfter){
                if(!func(httpSocket))
                    return;
            }
            return;
        }

        auto matchIter = m_MatchRoutes.begin();
        auto matchEnd = m_MatchRoutes.end();
        std::regex reg;
        for(; matchIter != matchEnd; matchIter++){
            reg.assign(matchIter->first);
            if(std::regex_match(httpPath,reg)){
                const auto& vectorAfter = std::get<2>(matchIter->second);
                const auto& vectorBefore = std::get<1>(matchIter->second);
                for(const auto& func : vectorBefore){
                    if(!func(httpSocket))
                        return;
                }
                std::get<0>(matchIter->second)(httpSocket);
                for(const auto& func : vectorAfter){
                    if(!func(httpSocket))
                        return;
                }
                return;
            }
        }
        const auto& response = httpSocket->getResponse();
        response->setStatus(HttpStatus::NOT_FOUND);
        response->setBody("<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr><center>Magic/2.0.0</center></body></html>");
        httpSocket->sendResponse(response);

    }

    void HttpServletDispatch::setHttpServlet(const Safe<IHttpServlet>& servlet){
        RWMutex::WriteLock lock(m_Mutex);
        servlet->m_ServletDispatch = this->shared_from_this();
    }
}
}
}
