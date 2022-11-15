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
        std::lock_guard<std::mutex> locker(m_Mutex);
        const auto& httpPath = httpSocket->getRequest()->getPath();
        auto exactlyIter = m_NormalRoutes.find(httpPath);
        auto exactlyEnd = m_NormalRoutes.end();
        if(exactlyIter != exactlyEnd){
            /// Global Before
            for(const auto& func : m_AspectBefores){
                if(!func(httpSocket))
                    return;
            }
            const auto& aspectAfters = std::get<2>(exactlyIter->second);
            const auto& aspectBefores = std::get<1>(exactlyIter->second);
            /// Before
            for(const auto& func : aspectAfters){
                if(!func(httpSocket))
                    return;
            }
            /// Handle
            std::get<0>(exactlyIter->second)(httpSocket);
            /// After
            for(const auto& func : aspectBefores){
                if(!func(httpSocket))
                    return;
            }
            /// Global After
            for(const auto& func : m_AspectAfters){
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
                /// Global Before
                for(const auto& func : m_AspectBefores){
                    if(!func(httpSocket))
                        return;
                }
                const auto& aspectAfters = std::get<2>(matchIter->second);
                const auto& aspectBefores = std::get<1>(matchIter->second);
                /// Before
                for(const auto& func : aspectAfters){
                    if(!func(httpSocket))
                        return;
                }
                /// Handle
                std::get<0>(matchIter->second)(httpSocket);
                /// After
                for(const auto& func : aspectBefores){
                    if(!func(httpSocket))
                        return;
                }
                /// Global After
                for(const auto& func : m_AspectAfters){
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
        std::lock_guard<std::mutex> locker(m_Mutex);
        servlet->m_ServletDispatch = this->shared_from_this();
    }
}
}
}
